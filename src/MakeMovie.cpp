// list all the files in the directory
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <parallel/algorithm>
#include <execution>

using namespace std;
namespace fs = std::filesystem;

void listFiles(const string &path, vector<string> &files) {
    for (const auto &entry : fs::directory_iterator(path)) {
        files.push_back(entry.path().string());
    }
}

// read a PPM file and return the content as a string
string readPPM(const string &filename) {
    ifstream file(filename);
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Upscale a PPM image
string upscalePPM(const string &content, int FinalScale=512) {
    stringstream ss(content);
    string Type, width, height, max, r, g, b;
    ss >> Type >> width >> height >> max;
    int w = stoi(width);
    int h = stoi(height);
    int m = stoi(max);

    int scale = FinalScale / w;

    stringstream out;
    out << Type << endl;
    out << w*scale << " " << h * scale << endl;
    out << max << endl;
    ostringstream line;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            ss >> r >> g >> b;
            for (int Size=0; Size<scale; Size++)
                line << r << " " << g << " " << b << " ";
        }
        for (int Size=0; Size<scale; Size++)
            out << line.str() << endl;
        line.str("");
    }
    return out.str();
}

// write a PPM file
void writePPM(const string &filename, const string &content) {
    ofstream file(filename);
    file << content;
}

int main() {
    string path = "PPM";
    vector<string> files;
    listFiles(path, files);
    cout << "Found " << files.size() << " files" << endl;
    __gnu_parallel::for_each(files.begin(), files.end(), [](auto&& file) {
        cout << "Upscale de : " << file << endl;
        string content = readPPM(file);
        writePPM(file, upscalePPM(content, 256));
    });
    
    // Run command in os
    int t = system("convert -scale 256 -delay 10 PPM/img*.ppm movie.gif");

    return 0;
}