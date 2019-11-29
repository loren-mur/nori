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

#include <nori/block.h>
#include <nori/gui.h>
#include <filesystem/path.h>

int main(int argc, char **argv) {
    using namespace nori;

    try {
        nanogui::init();

        // Open the UI with a dummy image
        ImageBlock block(Vector2i(720, 720), nullptr);
        NoriScreen *screen = new NoriScreen(block);

        // if file is passed as argument, handle it
        if (argc == 2) {
            std::string filename = argv[1];
            filesystem::path path(filename);

            if (path.extension() == "xml") {
                /* Render the XML scene file */
                screen->openXML(filename);
            } else if (path.extension() == "exr") {
                /* Alternatively, provide a basic OpenEXR image viewer */
                screen->openEXR(filename);
            } else {
                cerr << "Error: unknown file \"" << filename
                << "\", expected an extension of type .xml or .exr" << endl;
            }
        }

        nanogui::mainloop();
        delete screen;
        nanogui::shutdown();

    } catch (const std::exception &e) {
        cerr << "Fatal error: " << e.what() << endl;
        return -1;
    }
    return 0;
}
