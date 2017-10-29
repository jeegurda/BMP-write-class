#include "BMP.h"

bool BMP::saveBMP(string fileName, string *err) {
    FILE *filePtr;

    filePtr = fopen(fileName.c_str(),"wb");

    if (filePtr == NULL) {
        *err =  "Could not write bitmap image file.\n";
        return false;
    }

    fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
    fwrite(&infoHeader,sizeof(BITMAPINFOHEADER), 1, filePtr);

    fseek(filePtr,fileHeader.bfOffBits,SEEK_SET);

    fwrite(imgArray,  (infoHeader.biWidth * 3)  * infoHeader.biHeight,1, filePtr);

    fclose(filePtr);

    return true;
}

unsigned char * BMP::getSubImage(int x, int y, int w, int h) {
    int width = origWidth * 3;

    int p = 0;
    int xx = x;
    int yy = origHeight - (h + y);

    unsigned char * tmpArray = new unsigned char [(((w+padding)*h)*3)];

    for (int m = 0; m < h; m++) {
        for (int n = 0; n < w; n++) {
            int b = (width*(yy-1)+xx*3-2)-1;
            int g = (width*(yy-1)+xx*3-1)-1;
            int r = (width*(yy-1)+xx*3)-1;

            tmpArray[p] = (char)imgArray[b];
            tmpArray[p+1] = (char)imgArray[g];
            tmpArray[p+2] = (char)imgArray[r];

            p+=3;
            xx++;
        }
        if (xx >= w) {
          xx = x;
        }
        yy++;
    }

    imgArray = tmpArray;
    calcHeaders(w, h, 24);

    return tmpArray;
}

void BMP::calcHeaders(int width, int height, int bpp) {
    int headerSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    int padding = (4 - (width * 3) % 4) %4;
    int padWidth = (width * 3) + padding;

    fileHeader.bfOffBits  = headerSize;
    fileHeader.bfReserved1 = 0;
    fileHeader.bfReserved2 = 0;
    fileHeader.bfType =  0x4D42;
    fileHeader.bfSize = ((width * height) * 3 + headerSize) ;

    infoHeader.biSize = sizeof(BITMAPINFOHEADER);
    infoHeader.biWidth = width;
    infoHeader.biHeight = height;
    infoHeader.biPlanes = 1;
    infoHeader.biBitCount = bpp;
    infoHeader.biCompression = 0;
    infoHeader.biSizeImage = ((width * 3) + padding) * height;
    infoHeader.biXPelsPerMeter = 0;
    infoHeader.biYPelsPerMeter = 0;
    infoHeader.biClrUsed = 0;
    infoHeader.biClrImportant = 0;

    origWidth = width;
    origHeight = height;
}
