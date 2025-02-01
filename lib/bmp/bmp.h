#pragma once

#include "../storage/storage.h"
#include <cstdint>

void SaveBMP(HashTable& field, uint64_t number_save, const char* output_directory);

class BMPFile {
private:
#pragma pack(push)
#pragma pack(1)
    struct BMPFileHeader {
        uint8_t signature[2];
        uint32_t file_size;
        uint16_t bf_reserved_1 = 0;
        uint16_t bf_reserved_2 = 0;
        const uint32_t kFileOffset = 118;
    };

    struct DIBHeader {
        const uint32_t kDIBHeaderSize = 40;
        int32_t image_width = 0;
        int32_t image_height = 0;
        uint16_t planes = 1;
        const uint16_t kBitsPerPixel = 4;
        uint32_t compression = 0;
        uint32_t image_size = 0;
        uint32_t resolution_x = 0;
        uint32_t resolution_y = 0;
        uint32_t number_of_colors = 0;
        uint32_t colors_important = 0;
    };

    struct RGBQUAD {
        uint8_t rgb_blue;
        uint8_t rgb_green;
        uint8_t rgb_red;
        uint8_t rgb_reserved = 0;
    };
#pragma pack(pop)
    BMPFileHeader* file_header = nullptr;
    DIBHeader* bitmap_header = nullptr;
    RGBQUAD* colormap = nullptr;
    uint8_t* data = nullptr;

public:
    BMPFile() {
        file_header = new BMPFileHeader;
        bitmap_header = new DIBHeader;
        colormap = new RGBQUAD[16];
    }
    ~BMPFile() {
        delete file_header;
        delete bitmap_header;
        delete[] colormap;
        if (data != nullptr) {
            delete[] data;
        }
    }
    void LoadBMPFile(const char* filename);
    void PrintFileHeader();
    void PrintBitmapHeader(); 
    void PrintPixels();
    void FillColormap();
    void GetWidthAndHeight();
    void SaveDIBitmap(const char* filename, HashTable& field);
    void SetWidth(int32_t value);
    void SetHeight(int32_t value);
    void SetPixel(int16_t x, int16_t y, uint64_t value, int32_t shift_x, int32_t shift_y);
    void SetImageSize();
};