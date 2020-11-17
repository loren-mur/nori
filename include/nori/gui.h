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

#if !defined(__NORI_GUI_H)
#define __NORI_GUI_H

#include <nori/common.h>
#include <nanogui/screen.h>
#include <nori/render.h>

NORI_NAMESPACE_BEGIN

class NoriScreen : public nanogui::Screen {
public:
    NoriScreen(ImageBlock &block);
    virtual ~NoriScreen();

    void drawContents();

    virtual bool keyboardEvent(int key, int scancode, bool press, int modifiers) override;
    virtual void dropEvent(const std::vector<std::string> &filenames) override;

    void openXML(const std::string & filename);
    void openEXR(const std::string & filename);

private:
    ImageBlock &m_block;
    nanogui::GLShader *m_shader = nullptr;
    nanogui::Slider *m_slider = nullptr;
    nanogui::ProgressBar *m_progressBar = nullptr;
    uint32_t m_texture = 0;
    float m_scale = 1.f;
    Widget *panel = nullptr;

    RenderThread m_renderThread;
};

NORI_NAMESPACE_END

#endif /* __NORI_GUI_H */
