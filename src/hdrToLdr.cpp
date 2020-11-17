/*
    This file is part of Nori, a simple educational ray tracer

    Copyright (c) 2015 by Romain Prévost

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

#include <nori/bitmap.h>
#include <filesystem/path.h>

int main(int argc, char **argv) {
    using namespace nori;

    try {

        // if file is passed as argument, handle it
        if (argc == 2) {
            std::string filename = argv[1];
            filesystem::path path(filename);

            if (path.extension() == "exr") {
                Bitmap bitmap(filename);

                size_t lastdot = filename.find_last_of(".");
                std::string pngFilename = filename.substr(0, lastdot) + ".png";
                bitmap.saveToLDR(pngFilename);
            } else {
                cerr << "Error: unknown file \"" << filename
                << "\", expected an extension of type .exr" << endl;
            }
        }


    } catch (const std::exception &e) {
        cerr << "Fatal error: " << e.what() << endl;
        return -1;
    }
    return 0;
}
