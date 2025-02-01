#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "bmp.h"
#include "../storage/storage.h"


void BMPFile::LoadBMPFile(const char* filename) {
    std::FILE* file_pointer = std::fopen(filename, "r+");
    if (!file_pointer) {
        std::cerr << "Error opening file!" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    std::fread(file_header, sizeof(BMPFileHeader), 1, file_pointer);
    std::fread(bitmap_header, sizeof(DIBHeader), 1, file_pointer);
    data = new uint8_t[bitmap_header->image_size];
    std::fseek(file_pointer, file_header->kFileOffset, SEEK_SET);
    fread(data, bitmap_header->image_size, 1, file_pointer);
    fclose(file_pointer);
}

void BMPFile::PrintFileHeader() {
    std::cout << "signature: " << file_header->signature[0] << file_header->signature[1] << std::endl;
    std::cout << "file_size: " <<  file_header->file_size << std::endl;
    std::cout << "padding 1: " <<  file_header->bf_reserved_1 << std::endl;
    std::cout << "padding 2: " <<  file_header->bf_reserved_2 << std::endl;
    std::cout << "kFileOffset: " << file_header->kFileOffset << std::endl;
}

void BMPFile::PrintBitmapHeader() {
    std::cout << "kDIBHeaderSize: " << bitmap_header->kDIBHeaderSize << std::endl;
    std::cout << "image_width: " << bitmap_header->image_width << std::endl;
    std::cout << "image_height: " << bitmap_header->image_height << std::endl;
    std::cout << "planes: " << bitmap_header->planes << std::endl;
    std::cout << "kBitsPerPixel: " << bitmap_header->kBitsPerPixel << std::endl;
    std::cout << "compression: " << bitmap_header->compression << std::endl;
    std::cout << "image_size: " << bitmap_header->image_size << std::endl;
    std::cout << "resolution_x: " << bitmap_header->resolution_x << std::endl;
    std::cout << "resolution_y: " << bitmap_header->resolution_y << std::endl;
    std::cout << "number_of_colors: " << bitmap_header->number_of_colors << std::endl;
    std::cout << "colors_important: " << bitmap_header->colors_important << std::endl;
}

void BMPFile::PrintPixels() {
    for(std::size_t i = 0; i < bitmap_header->image_size; ++i) {
        if (i % bitmap_header->image_width == 0) {
            std::cout << std::endl << i << ": ";
        }
        std::cout << (int)data[i] << " ";
    }
}

void BMPFile::FillColormap() {
    const RGBQUAD kWhiteColor = {255, 255, 255, 0};
    const RGBQUAD kGreenColor = {0, 255, 0, 0};
    const RGBQUAD kPurpleColor = {255, 0, 255, 0};
    const RGBQUAD kYellowColor = {0, 255, 255, 0};
    const RGBQUAD kBlackColor = {0, 0, 0, 0};

    for (std::size_t i = 0; i < 16; ++i) {
        switch (i) {
            case 0:
                colormap[i] = kWhiteColor;
                break;
            case 1:
                colormap[i] = kGreenColor;
                break;
            case 2:
                colormap[i] = kPurpleColor;
                break;
            case 3:
                colormap[i] = kYellowColor;
                break;
            default:
                colormap[i] = kBlackColor;
        }
    }
}

void BMPFile::SetWidth(int32_t value) {
    bitmap_header->image_width = value;
}

void BMPFile::SetHeight(int32_t value) {
    bitmap_header->image_height = value;
}

void BMPFile::SetPixel(int16_t x, int16_t y, uint64_t value, int32_t shift_x, int32_t shift_y) {
    if (value > 3) {
        value = 4;
    }
    x += shift_x;
    y += shift_y;
    uint64_t index = x + y * (bitmap_header->image_width);
    if (index % 2 == 0) {
        uint16_t pixel_pos = (index) / 2;
        data[pixel_pos] = (data[pixel_pos] & 0x0F) | (value << 4);
    } else {
        uint16_t pixel_pos = (index) / 2;
        data[pixel_pos] = (data[pixel_pos] & 0xF0) | (value);
    }
}

void BMPFile::SaveDIBitmap(const char* filename, HashTable& field) {
    std::FILE* file_pointer = std::fopen(filename, "wb");
    if (!file_pointer) {
        std::cerr << "Error opening file to write!" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    int32_t shift_x = 0;
    int32_t shift_y = 0;

    if (field.min_coordinate_x < 0) {
        shift_x = std::abs(field.min_coordinate_x);
    } else if (field.min_coordinate_x > 0) {
        shift_x = field.min_coordinate_x * -1;
    }
    if (field.min_coordinate_y < 0) {
        shift_y = std::abs(field.min_coordinate_y);
    } else if (field.min_coordinate_y > 0) {
        shift_y = field.min_coordinate_y * -1;
    }

    if (field.width % 8 != 0) {
        SetWidth((field.width / 8 + 1) * 8);
    } else {
        SetWidth(field.width);
    }
    if (field.heigh % 8 != 0) {
        SetHeight((field.heigh / 8 + 1) * 8);
    } else {
        SetHeight(field.heigh);
    }

    uint64_t data_size = bitmap_header->image_width * bitmap_header->image_height;
    file_header->signature[0] = 'B';
    file_header->signature[1] = 'M';
    file_header->file_size = file_header->kFileOffset;
    data = new uint8_t[data_size];

    for (std::size_t i = 0; i < data_size; ++i) {
        data[i] = (data[i] & 0x0f) | (0 << 4);
        data[i] = (data[i] & 0xf0) | 0;
    }

    for (std::size_t i = 0; i < field.GetHashTableSize(); ++i) {
        if (field.array[i].value_ != 0) {
            SetPixel(field.array[i].x_, field.array[i].y_, field.array[i].value_, shift_x, shift_y);
        }
        Node* iterator = &field.array[i];
        while (iterator->pointer_ != nullptr) {
            iterator = iterator->pointer_;
            SetPixel(iterator->x_, iterator->y_, iterator->value_, shift_x, shift_y);
        }
    }

    FillColormap();
    std::fwrite(file_header, sizeof(BMPFileHeader), 1, file_pointer);
    std::fwrite(bitmap_header, sizeof(DIBHeader), 1, file_pointer);
    std::fwrite(colormap, sizeof(RGBQUAD) * 16, 1, file_pointer);
    if (std::fwrite(data, 1, data_size, file_pointer) < data_size) {
        std::cerr << "Couldn't write bitmap!" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    fclose(file_pointer);
}

void SaveBMP(HashTable& field, uint64_t number_save, const char* output_directory) {
    field.GetMinAndMax();
    field.width = std::abs(field.max_coordinate_x - field.min_coordinate_x) + 1;
    field.heigh = std::abs(field.max_coordinate_y - field.min_coordinate_y) + 1;
    BMPFile bmp_image;
    char filename[1000] { };
    std::size_t i = 0;
    std::cout << output_directory << std::endl;
    while (output_directory[i] != '\0') {
        filename[i] = output_directory[i];
        ++i;
    }
    while (number_save != 0 && i < 990) {
        filename[i] = (number_save % 10) + '0';
        number_save /= 10;
        ++i;
    }
    filename[i] = '.';
    filename[i + 1] = 'b';
    filename[i + 2] = 'm';
    filename[i + 3] = 'p';
    bmp_image.SaveDIBitmap(filename, field);
    std::cout << "File " << filename << " was saved" << std::endl;
}
