// MIT License
// Copyright (c) 2022 Advanced Micro Devices, Inc. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// SOFTWARE.
//////////////////////////////////////////////////////////////////////////////////
// Module Name: lab2 yuv filter
// Purpose:
//    This is a C++ version of the yuv filter example.
// Reference:
//////////////////////////////////////////////////////////////////////////////////
#include "yuv_filter.h"
#include <stdlib.h>

// ─────────────────────────────────────────────────────────────
// Top-level function
// ─────────────────────────────────────────────────────────────
void yuv_filter(
    image_t *in,
    image_t *out,
    yuv_scale_t Y_scale,
    yuv_scale_t U_scale,
    yuv_scale_t V_scale)
{
    static image_t yuv_img;
    static image_t scale_img;

    image_t *yuv = &yuv_img;
    image_t *scale = &scale_img;

    rgb2yuv(in, yuv);
    yuv_scale(yuv, scale, Y_scale, U_scale, V_scale);
    yuv2rgb(scale, out);
}

// ─────────────────────────────────────────────────────────────
// Convert RGB to YUV
// ─────────────────────────────────────────────────────────────
void rgb2yuv(image_t *in, image_t *out)
{
    image_dim_t x, y;
    image_dim_t width  = in->width;
    image_dim_t height = in->height;

    out->width  = width;
    out->height = height;

    const rgb2yuv_coef_t Wrgb[3][3] = {
        { 66, 129,  25},
        {-38, -74, 112},
        {122, -94, -18},
    };

RGB2YUV_LOOP_X:
    for (x = 0; x < width; x++) {
RGB2YUV_LOOP_Y:
        for (y = 0; y < height; y++) {
            image_pix_t R = in->channels.ch1[x][y];
            image_pix_t G = in->channels.ch2[x][y];
            image_pix_t B = in->channels.ch3[x][y];

            image_pix_t Y = ((Wrgb[0][0]*R + Wrgb[0][1]*G + Wrgb[0][2]*B + 128) >> 8) + 16;
            image_pix_t U = ((Wrgb[1][0]*R + Wrgb[1][1]*G + Wrgb[1][2]*B + 128) >> 8) + 128;
            image_pix_t V = ((Wrgb[2][0]*R + Wrgb[2][1]*G + Wrgb[2][2]*B + 128) >> 8) + 128;

            out->channels.ch1[x][y] = Y;
            out->channels.ch2[x][y] = U;
            out->channels.ch3[x][y] = V;
        }
    }
}

// ─────────────────────────────────────────────────────────────
// Convert YUV back to RGB
// ─────────────────────────────────────────────────────────────
void yuv2rgb(image_t *in, image_t *out)
{
    image_dim_t x, y;
    image_dim_t width  = in->width;
    image_dim_t height = in->height;

    out->width  = width;
    out->height = height;

    const yuv2rgb_coef_t Wyuv[3][3] = {
        {298,    0,  409},
        {298, -100, -208},
        {298,  516,    0},
    };

YUV2RGB_LOOP_X:
    for (x = 0; x < width; x++) {
YUV2RGB_LOOP_Y:
        for (y = 0; y < height; y++) {
            image_pix_t Y = in->channels.ch1[x][y];
            image_pix_t U = in->channels.ch2[x][y];
            image_pix_t V = in->channels.ch3[x][y];

            yuv_intrnl_t C = Y - 16;
            yuv_intrnl_t D = U - 128;
            yuv_intrnl_t E = V - 128;

            image_pix_t R = CLIP((Wyuv[0][0]*C + Wyuv[0][2]*E + 128) >> 8);
            image_pix_t G = CLIP((Wyuv[1][0]*C + Wyuv[1][1]*D + Wyuv[1][2]*E + 128) >> 8);
            image_pix_t B = CLIP((Wyuv[2][0]*C + Wyuv[2][1]*D + 128) >> 8);

            out->channels.ch1[x][y] = R;
            out->channels.ch2[x][y] = G;
            out->channels.ch3[x][y] = B;
        }
    }
}

// ─────────────────────────────────────────────────────────────
// Apply scaling to Y/U/V
// ─────────────────────────────────────────────────────────────
void yuv_scale(
    image_t *in,
    image_t *out,
    yuv_scale_t Y_scale,
    yuv_scale_t U_scale,
    yuv_scale_t V_scale
)
{
    image_dim_t x, y;
    image_dim_t width  = in->width;
    image_dim_t height = in->height;

    out->width  = width;
    out->height = height;

YUV_SCALE_LOOP_X:
    for (x = 0; x < width; x++) {
YUV_SCALE_LOOP_Y:
        for (y = 0; y < height; y++) {
            yuv_intrnl_t Yn = (in->channels.ch1[x][y] * Y_scale) >> 7;
            yuv_intrnl_t Un = (in->channels.ch2[x][y] * U_scale) >> 7;
            yuv_intrnl_t Vn = (in->channels.ch3[x][y] * V_scale) >> 7;

            out->channels.ch1[x][y] = Yn;
            out->channels.ch2[x][y] = Un;
            out->channels.ch3[x][y] = Vn;
        }
    }
}
