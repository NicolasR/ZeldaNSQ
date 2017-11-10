#include "WFile.h"

WFile::WFile(string filename) : buffer(0), size(0) {
    name = filename;
    ifstream file;
    file.open (name.c_str(), ifstream::in | ifstream::binary);
    if (file.is_open()) {
        file.seekg (0, ios::end);
        size = file.tellg();
        file.seekg (0, ios::beg);
        buffer = new char [size];
        file.read (buffer, size);
        file.close();
    }
}

WFile::~WFile() {
    delete buffer;
}

char* WFile::getBuffer() {
    return buffer;
}

long WFile::getSize() {
    return size;
}

