#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#pragma pack(push, 1)
typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BITMAPINFOHEADER;

typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} PIXEL;
#pragma pack(pop)

int main(int argc, char *argv[]) {
    if(argc != 3) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        exit(1);
    }
    
    char *inputFileName = argv[1];
    char *outputFileName = argv[2];
    
    FILE *file = fopen(inputFileName, "rb");
    if (file == NULL) {
        perror("Unable to open the input file");
        exit(1);
    }
    
    BITMAPFILEHEADER bf;
    BITMAPINFOHEADER bi;
    
    fread(&bf, sizeof(bf), 1, file);
    fread(&bi, sizeof(bi), 1, file);
    
    if (bf.bfType != 0x4D42 || bi.biBitCount != 24) {
        fprintf(stderr, "Unsupported file format\n");
        fclose(file);
        exit(1);
    }
    
    PIXEL **image = (PIXEL **)malloc(bi.biHeight * sizeof(PIXEL *));
    for (int i = 0; i < bi.biHeight; i++) {
        image[i] = (PIXEL *)malloc(bi.biWidth * sizeof(PIXEL));
    }
    
    fseek(file, bf.bfOffBits, SEEK_SET);
    
    for (int i = 0; i < bi.biHeight; i++) {
        fread(image[i], sizeof(PIXEL), bi.biWidth, file);
        fseek(file, bi.biWidth % 4, SEEK_CUR); // Handle padding
    }
    
    fclose(file);
    
    PIXEL **edge_image = (PIXEL **)malloc(bi.biHeight * sizeof(PIXEL *));
    for (int i = 0; i < bi.biHeight; i++) {
        edge_image[i] = (PIXEL *)calloc(bi.biWidth, sizeof(PIXEL));
    }
    
    for (int i = 1; i < bi.biHeight - 1; i++) {
        for (int j = 1; j < bi.biWidth - 1; j++) {
            int gx_r = image[i-1][j+1].red + 2*image[i][j+1].red + image[i+1][j+1].red
                     - image[i-1][j-1].red - 2*image[i][j-1].red - image[i+1][j-1].red;
            int gy_r = image[i-1][j-1].red + 2*image[i-1][j].red + image[i-1][j+1].red
                     - image[i+1][j-1].red - 2*image[i+1][j].red - image[i+1][j+1].red;
            
            int gradient_r = abs(gx_r) + abs(gy_r);
            
            int gx_g = image[i-1][j+1].green + 2*image[i][j+1].green + image[i+1][j+1].green
                     - image[i-1][j-1].green - 2*image[i][j-1].green - image[i+1][j-1].green;
            int gy_g = image[i-1][j-1].green + 2*image[i-1][j].green + image[i-1][j+1].green
                     - image[i+1][j-1].green - 2*image[i+1][j].green - image[i+1][j+1].green;
            
            int gradient_g = abs(gx_g) + abs(gy_g);
            
            int gx_b = image[i-1][j+1].blue + 2*image[i][j+1].blue + image[i+1][j+1].blue
                     - image[i-1][j-1].blue - 2*image[i][j-1].blue - image[i+1][j-1].blue;
            int gy_b = image[i-1][j-1].blue + 2*image[i-1][j].blue + image[i-1][j+1].blue
                     - image[i+1][j-1].blue - 2*image[i+1][j].blue - image[i+1][j+1].blue;
            
            int gradient_b = abs(gx_b) + abs(gy_b);
            
            if (gradient_r > 255) gradient_r = 255;
            if (gradient_g > 255) gradient_g = 255;
            if (gradient_b > 255) gradient_b = 255;
            
            edge_image[i][j].red = gradient_r;
            edge_image[i][j].green = gradient_g;
            edge_image[i][j].blue = gradient_b;
        }
    }
    
    file = fopen(outputFileName, "wb");
    if (file == NULL) {
        perror("Unable to open the output file");
        exit(1);
    }
    
    fwrite(&bf, sizeof(bf), 1, file);
    fwrite(&bi, sizeof(bi), 1, file);
    fseek(file, bf.bfOffBits, SEEK_SET);
    
    for (int i = 0; i < bi.biHeight; i++) {
        fwrite(edge_image[i], sizeof(PIXEL), bi.biWidth, file);
        // Correctly add padding bytes here, if needed
    }
    
    fclose(file);
    
    // Free the allocated memory
    for (int i = 0; i < bi.biHeight; i++) {
        free(image[i]);
        free(edge_image[i]);
    }
    free(image);
    free(edge_image);
    
    return 0;
}

