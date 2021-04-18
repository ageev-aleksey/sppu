#include "camera/JpgBuffConverter.h"
#include <stdlib.h>
#include <stdio.h>
#include <jpeglib.h>
#include <setjmp.h>

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


class JpgBuffConverterImpl {
public:
    JpgBuffConverterImpl() {
        cinfo.err = jpeg_std_error(&jerr.pub);
        jerr.pub.error_exit = my_error_exit;
        /* Establish the setjmp return context for my_error_exit to use. */
//        if (setjmp(jerr.setjmp_buffer)) {
//            /* If we get here, the JPEG code has signaled an error.
//             * We need to clean up the JPEG object, close the input file, and return.
//             */
//            jpeg_destroy_decompress(&cinfo);
//            return;
//        }
        /* Now we can initialize the JPEG decompression object. */
        jpeg_create_decompress(&cinfo);
    }

    ~JpgBuffConverterImpl() {
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



JpgBuffConverter::JpgBuffConverter()
    : m_pimpl(new JpgBuffConverterImpl)
{
    // empty
}

cv::Mat JpgBuffConverter::operator()(const std::vector<unsigned char> &jpg_img) {
    return m_pimpl->operator()(jpg_img);
}
