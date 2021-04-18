#include <stdlib.h>
#include <stdio.h>
#include <jpeglib.h>
#include <setjmp.h>

#include <iostream>
#include <fstream>
#include <vector>

#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>

#define uchar unsigned char
char filename1[] = "./red_point_on_wall.jpg";
char filename2[] = "./red.jpg";
char filename3[] = "./image.jpg";


struct my_error_mgr {
    struct jpeg_error_mgr pub;    /* "public" fields */

    jmp_buf setjmp_buffer;        /* for return to caller */
};

void my_error_exit(j_common_ptr cinfo)
{
    /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
    my_error_mgr* myerr = (my_error_mgr*)cinfo->err;

    /* Always display the message. */
    /* We could postpone this until after returning, if we chose. */
    (*cinfo->err->output_message) (cinfo);

    /* Return control to the setjmp point */
    longjmp(myerr->setjmp_buffer, 1);
}


class JpgBuffConverter {
public:
    JpgBuffConverter() {
        cinfo.err = jpeg_std_error(&jerr.pub);
        jerr.pub.error_exit = my_error_exit;
        /* Establish the setjmp return context for my_error_exit to use. */
        if (setjmp(jerr.setjmp_buffer)) {
            /* If we get here, the JPEG code has signaled an error.
             * We need to clean up the JPEG object, close the input file, and return.
             */
            jpeg_destroy_decompress(&cinfo);
            return;
        }
        /* Now we can initialize the JPEG decompression object. */
        jpeg_create_decompress(&cinfo);
    }

    ~JpgBuffConverter() {
        jpeg_destroy_decompress(&cinfo);
    }

    cv::Mat operator()(const std::vector<uchar> &jpg_img) {
        jpeg_mem_src(&cinfo, jpg_img.data(), jpg_img.size());
        jpeg_read_header(&cinfo, TRUE);
        cinfo.out_color_space = JCS_EXT_BGR;
        jpeg_start_decompress(&cinfo);

        /*    switch (jpeg_read_header(&cinfo, TRUE)) {
        case JPEG_SUSPENDED:
        case JPEG_HEADER_TABLES_ONLY:
            return 1;
        case JPEG_HEADER_OK:
            break;
    }*/

        auto retImg = cv::Mat(
                cv::Size(cinfo.output_width, cinfo.output_height),
                CV_8UC3);

        while (cinfo.output_scanline < cinfo.output_height) {
            JSAMPLE* row = retImg.ptr(cinfo.output_scanline);
            jpeg_read_scanlines(&cinfo, &row, 1);
        }

        jpeg_finish_decompress(&cinfo);
        return retImg;
    }
private:
    struct jpeg_decompress_struct cinfo;
    struct my_error_mgr jerr;

};

cv::Mat jpgBuff2Mat(const std::vector<uchar> &jpg_img) {

}

std::vector<uchar> readBinFile(const std::string &filename) {
    std::vector<uchar> jpg_img;
    int jpeg_size = 0;
    std::ifstream file(filename, std::ios::binary);

    char byte;
    file.read(&byte, 1);
    while(!file.eof()) {
        jpg_img.push_back(byte);
        file.read(&byte, 1);
    }
    file.close();
    return jpg_img;
}

int main() {

    std::vector<std::vector<uchar>> slides;
    slides.push_back(readBinFile(filename1));
    slides.push_back(readBinFile(filename2));
    slides.push_back(readBinFile(filename3));


    JpgBuffConverter load;
    cv::Mat glob_img;

    size_t cycle = 0;
    while (cv::waitKey(500) != 27) {
        size_t index = cycle % slides.size();
        glob_img = load(slides[index]);
        cv::imshow("jpeg", glob_img);
        cycle++;
    }

    return 0;
}