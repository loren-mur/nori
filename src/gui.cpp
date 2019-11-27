/*
    This file is part of Nori, a simple educational ray tracer

    Copyright (c) 2015 by Wenzel Jakob, Romain Prévost

    Nori is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    Nori is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <nori/gui.h>
#include <nori/block.h>
#include <nori/parser.h>
#include <nori/bitmap.h>
#include <nanogui/glutil.h>
#include <nanogui/label.h>
#include <nanogui/slider.h>
#include <nanogui/button.h>
#include <nanogui/layout.h>
#include <nanogui/entypo.h>
#include <nanogui/progressbar.h>

#include <filesystem/resolver.h>

NORI_NAMESPACE_BEGIN

#define PANEL_HEIGHT 48

NoriScreen::NoriScreen(ImageBlock &block)
 : nanogui::Screen(block.getSize() + Vector2i(0, PANEL_HEIGHT), "Nori", false),
   m_block(block), m_renderThread(m_block)
{
    using namespace nanogui;

    /* Add some UI elements to adjust the exposure value */
    panel = new Widget(this);
    panel->setLayout(new BoxLayout(BoxLayout::Horizontal, BoxLayout::Middle, 10, 10));
    Button * buttonOpen = new Button(panel, "", ENTYPO_ICON_FOLDER);
    buttonOpen->setCallback(
            [this]() {
                using FileType = std::pair<std::string,std::string>;
                std::vector<FileType> filetypes = { FileType("xml", "Nori Scene File"), FileType("exr", "EXR Image File") };
                std::string filename = nanogui::file_dialog(filetypes, false);
                dropEvent({ filename });
            }
    );
    buttonOpen->setTooltip("Open XML or EXR file (Ctrl+O)");
    m_progressBar = new ProgressBar(panel);
    m_progressBar->setFixedSize(Vector2i(200,30));
    Button * buttonStop = new Button(panel, "", ENTYPO_ICON_STOP);
    buttonStop->setCallback(
            [this]() {
                m_renderThread.stopRendering();
            }
    );
    buttonStop->setTooltip("Abort rendering (Ctrl+Z)");
    new Label(panel, "Exposure: ", "sans-bold");
    m_slider = new Slider(panel);
    m_slider->setValue(0.5f);
    m_slider->setFixedWidth(120);
    m_slider->setCallback(
        [&](float value) {
            m_scale = std::pow(2.f, (value - 0.5f) * 20);
        }
    );

    panel->setSize(block.getSize());
    performLayout(mNVGContext);

    panel->setPosition(
        Vector2i((mSize.x() - panel->size().x()) / 2, 0));

    /* Simple gamma tonemapper as a GLSL shader */
    m_shader = new GLShader();
    m_shader->init(
        "Tonemapper",

        /* Vertex shader */
        "#version 330\n"
        "in vec2 position;\n"
        "out vec2 uv;\n"
        "void main() {\n"
        "    gl_Position = vec4(position.x*2-1, position.y*2-1, 0.0, 1.0);\n"
        "    uv = vec2(position.x, 1-position.y);\n"
        "}",

        /* Fragment shader */
        "#version 330\n"
        "uniform sampler2D source;\n"
        "uniform float scale;\n"
        "in vec2 uv;\n"
        "out vec4 out_color;\n"
        "float toSRGB(float value) {\n"
        "    if (value < 0.0031308)\n"
        "        return 12.92 * value;\n"
        "    return 1.055 * pow(value, 0.41666) - 0.055;\n"
        "}\n"
        "void main() {\n"
        "    vec4 color = texture(source, uv);\n"
        "    color *= scale / color.w;\n"
        "    out_color = vec4(toSRGB(color.r), toSRGB(color.g), toSRGB(color.b), 1);\n"
        "}"
    );

    MatrixXu indices(3, 2); /* Draw 2 triangles */
    indices.col(0) << 0, 1, 2;
    indices.col(1) << 2, 3, 0;

    MatrixXf positions(2, 4);
    positions.col(0) << 0, 0;
    positions.col(1) << 1, 0;
    positions.col(2) << 1, 1;
    positions.col(3) << 0, 1;

    m_shader->bind();
    m_shader->uploadIndices(indices);
    m_shader->uploadAttrib("position", positions);

    /* Allocate texture memory for the rendered image */
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    drawAll();
    setVisible(true);
}

NoriScreen::~NoriScreen() {
    glDeleteTextures(1, &m_texture);
    delete m_shader;
}

void NoriScreen::drawContents() {
    /* Reload the partially rendered image onto the GPU */
    m_block.lock();
    int borderSize = m_block.getBorderSize();
    const Vector2i &size = m_block.getSize();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, m_block.cols());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size.x(), size.y(),
            0, GL_RGBA, GL_FLOAT, (uint8_t *) m_block.data() +
            (borderSize * m_block.cols() + borderSize) * sizeof(Color4f));
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    m_block.unlock();

    m_progressBar->setValue(m_renderThread.getProgress());

    glViewport(0, 0, mPixelRatio*size[0], mPixelRatio*size[1]);
    m_shader->bind();
    m_shader->setUniform("scale", m_scale);
    m_shader->setUniform("source", 0);
    m_shader->drawIndexed(GL_TRIANGLES, 0, 2);
    glViewport(0, 0, mFBSize[0], mFBSize[1]);
}

void NoriScreen::dropEvent(const std::vector<std::string> &filenames) {
    if(filenames.size() > 0) {
        std::string filename = filenames[0];
        filesystem::path path(filename);

        if (path.extension() == "xml") {
            /* Render the XML scene file */
            openXML(filename);
        } else if (path.extension() == "exr") {
            /* Alternatively, provide a basic OpenEXR image viewer */
            openEXR(filename);
        } else {
            cerr << "Error: unknown file \"" << filename
            << "\", expected an extension of type .xml or .exr" << endl;
        }
    }
}

bool NoriScreen::keyboardEvent(int key, int scancode, bool press, int modifiers) {
    if(press && key == GLFW_KEY_O && modifiers & GLFW_MOD_CONTROL) {
        using FileType = std::pair<std::string,std::string>;
        std::vector<FileType> filetypes = { FileType("xml", "Nori Scene File"), FileType("exr", "EXR Image File") };
        std::string filename = nanogui::file_dialog(filetypes, false);
        dropEvent({ filename });
        return true;
    }
    if(press && key == GLFW_KEY_Z && modifiers & GLFW_MOD_CONTROL) {
        m_renderThread.stopRendering();
        return true;
    }

    return nanogui::Screen::keyboardEvent(key,scancode,press,modifiers);
}

void NoriScreen::openXML(const std::string &filename) {

    if(m_renderThread.isBusy()) {
        cerr << "Error: rendering in progress, you need to wait until it's done" << endl;
        return;
    }

    try {

        m_renderThread.renderScene(filename);

        m_block.lock();
        Vector2i bsize = m_block.getSize();
        m_block.unlock();

        Vector2i wsize = bsize + Vector2i(0, PANEL_HEIGHT);
        glfwSetWindowSize(glfwWindow(), wsize.x(), wsize.y());
        panel->setSize(bsize);
        performLayout(mNVGContext);

        panel->setPosition(
                Vector2i((wsize.x() - panel->size().x()) / 2, 0));

    } catch (const std::exception &e) {
        cerr << "Fatal error: " << e.what() << endl;
    }

}

void NoriScreen::openEXR(const std::string &filename) {

    if(m_renderThread.isBusy()) {
        cerr << "Error: rendering in progress, you need to wait until it's done" << endl;
        return;
    }

    Bitmap bitmap(filename);

    m_block.lock();
    m_block.init(Vector2i(bitmap.cols(), bitmap.rows()), nullptr);
    m_block.fromBitmap(bitmap);
    Vector2i bsize = m_block.getSize();
    m_block.unlock();

    Vector2i wsize = bsize + Vector2i(0, PANEL_HEIGHT);
    glfwSetWindowSize(glfwWindow(),wsize.x(),wsize.y());
    panel->setSize(bsize);
    performLayout(mNVGContext);

    panel->setPosition(
            Vector2i((wsize.x() - panel->size().x()) / 2, 0));
}

NORI_NAMESPACE_END
