#include "typedefs.h"

const PBYTE widtbl_M[112] =
{
        5, 2, 3, 8, 7, 8, 8, 2, 
        6, 6, 7, 5, 2, 5, 4, 6, 
        7, 7, 7, 7, 7, 7, 7, 7, 
        7, 7, 2, 2, 5, 5, 5, 7, 
        8, 7, 7, 7, 7, 7, 7, 7, 
        6, 3, 7, 7, 6, 9, 7, 7, 
        7, 7, 7, 7, 7, 7, 7, 9, 
        7, 7, 7, 3, 6, 3, 16, 16, 
        3, 6, 6, 6, 6, 6, 5, 6, 
        6, 4, 4, 5, 4, 7, 6, 7, 
        6, 7, 5, 5, 4, 6, 7, 7, 
        5, 6, 6, 11, 16, 8, 2, 13, 
        15, 7, 16, 16, 11, 16, 3, 5, 
        13, 13, 13, 13, 13, 13, 13, 13
};

/* Column format, LSB top */

const PBYTE chr_M_20[10] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const PBYTE chr_M_21[4] = { 0x00, 0x00, 0xF8, 0x17 };
const PBYTE chr_M_22[6] = { 0x1C, 0x00, 0x00, 0x00, 0x1C, 0x00 };
const PBYTE chr_M_23[16] = { 0x40, 0x02, 0x40, 0x02, 0xF8, 0x1F, 0x40, 0x02, 0x40, 0x02, 
                            0xF8, 0x1F, 0x40, 0x02, 0x40, 0x02 };
const PBYTE chr_M_24[14] = { 0xC0, 0x08, 0x20, 0x11, 0x10, 0x11, 0xF8, 0x3F, 0x10, 0x11, 
                            0x10, 0x09, 0x20, 0x06 };
const PBYTE chr_M_25[16] = { 0x30, 0x18, 0x48, 0x04, 0x48, 0x02, 0x30, 0x01, 0x80, 0x0C, 
                            0x40, 0x12, 0x20, 0x12, 0x18, 0x0C };
const PBYTE chr_M_26[16] = { 0x60, 0x0C, 0x90, 0x12, 0x08, 0x11, 0x90, 0x12, 0x60, 0x0C, 
                            0x00, 0x0C, 0x00, 0x12, 0x00, 0x10 };
const PBYTE chr_M_27[4] = { 0x20, 0x00, 0x1C, 0x00 };
const PBYTE chr_M_28[12] = { 0xC0, 0x07, 0x30, 0x18, 0x08, 0x20, 0x04, 0x40, 0x02, 0x80, 
                            0x02, 0x80 };
const PBYTE chr_M_29[12] = { 0x02, 0x80, 0x02, 0x80, 0x04, 0x40, 0x08, 0x20, 0x30, 0x18, 
                            0xC0, 0x07 };
const PBYTE chr_M_2A[14] = { 0x20, 0x02, 0x40, 0x01, 0x80, 0x00, 0xF0, 0x07, 0x80, 0x00, 
                            0x40, 0x01, 0x20, 0x02 };
const PBYTE chr_M_2B[10] = { 0x00, 0x01, 0x00, 0x01, 0xC0, 0x07, 0x00, 0x01, 0x00, 0x01 };
const PBYTE chr_M_2C[4] = { 0x00, 0x58, 0x00, 0x38 };
const PBYTE chr_M_2D[10] = { 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01 };
const PBYTE chr_M_2E[8] = { 0x00, 0x18, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00 };
const PBYTE chr_M_2F[12] = { 0x00, 0x30, 0x00, 0x0C, 0x00, 0x03, 0xC0, 0x00, 0x30, 0x00, 
                            0x0C, 0x00 };
const PBYTE chr_M_30[14] = { 0xC0, 0x03, 0x30, 0x0C, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 
                            0x30, 0x0C, 0xC0, 0x03 };
const PBYTE chr_M_31[14] = { 0x00, 0x00, 0x20, 0x10, 0x10, 0x10, 0xF8, 0x1F, 0x00, 0x10, 
                            0x00, 0x10, 0x00, 0x00 };
const PBYTE chr_M_32[14] = { 0x20, 0x18, 0x10, 0x14, 0x08, 0x12, 0x08, 0x11, 0x08, 0x11, 
                            0x90, 0x10, 0x60, 0x10 };
const PBYTE chr_M_33[14] = { 0x10, 0x08, 0x08, 0x10, 0x88, 0x10, 0x88, 0x10, 0x88, 0x10, 
                            0x50, 0x09, 0x20, 0x06 };
const PBYTE chr_M_34[14] = { 0x00, 0x03, 0x80, 0x02, 0x40, 0x02, 0x20, 0x02, 0x10, 0x02, 
                            0xF8, 0x1F, 0x00, 0x02 };
const PBYTE chr_M_35[14] = { 0xF8, 0x08, 0x88, 0x10, 0x88, 0x10, 0x88, 0x10, 0x88, 0x10, 
                            0x08, 0x09, 0x00, 0x06 };
const PBYTE chr_M_36[14] = { 0xE0, 0x07, 0x10, 0x09, 0x88, 0x10, 0x88, 0x10, 0x88, 0x10, 
                            0x08, 0x09, 0x00, 0x06 };
const PBYTE chr_M_37[14] = { 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x1C, 0x08, 0x03, 
                            0xC8, 0x00, 0x38, 0x00 };
const PBYTE chr_M_38[14] = { 0x20, 0x06, 0x50, 0x09, 0x88, 0x10, 0x88, 0x10, 0x88, 0x10, 
                            0x50, 0x09, 0x20, 0x06 };
const PBYTE chr_M_39[14] = { 0x60, 0x00, 0x90, 0x10, 0x08, 0x11, 0x08, 0x11, 0x08, 0x11, 
                            0x90, 0x08, 0xE0, 0x07 };
const PBYTE chr_M_3A[4] = { 0xC0, 0x06, 0xC0, 0x06 };
const PBYTE chr_M_3B[4] = { 0xC0, 0x16, 0xC0, 0x0E };
const PBYTE chr_M_3C[10] = { 0x00, 0x01, 0x80, 0x02, 0x40, 0x04, 0x20, 0x08, 0x10, 0x10 };
const PBYTE chr_M_3D[10] = { 0x80, 0x02, 0x80, 0x02, 0x80, 0x02, 0x80, 0x02, 0x80, 0x02 };
const PBYTE chr_M_3E[10] = { 0x10, 0x10, 0x20, 0x08, 0x40, 0x04, 0x80, 0x02, 0x00, 0x01 };
const PBYTE chr_M_3F[14] = { 0x20, 0x00, 0x10, 0x00, 0x08, 0x00, 0x08, 0x16, 0x08, 0x01, 
                            0x90, 0x00, 0x60, 0x00 };
const PBYTE chr_M_40[16] = { 0xE0, 0x07, 0x10, 0x08, 0xC8, 0x13, 0x28, 0x14, 0x28, 0x14, 
                            0xC8, 0x17, 0x10, 0x14, 0xE0, 0x03 };
const PBYTE chr_M_41[14] = { 0x00, 0x1C, 0x80, 0x03, 0x60, 0x02, 0x18, 0x02, 0x60, 0x02, 
                            0x80, 0x03, 0x00, 0x1C };
const PBYTE chr_M_42[14] = { 0xF8, 0x1F, 0x88, 0x10, 0x88, 0x10, 0x88, 0x10, 0x88, 0x10, 
                            0x50, 0x09, 0x20, 0x06 };
const PBYTE chr_M_43[14] = { 0xE0, 0x07, 0x10, 0x08, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 
                            0x08, 0x10, 0x10, 0x08 };
const PBYTE chr_M_44[14] = { 0xF8, 0x1F, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 
                            0x10, 0x08, 0xE0, 0x07 };
const PBYTE chr_M_45[14] = { 0xF8, 0x1F, 0x88, 0x10, 0x88, 0x10, 0x88, 0x10, 0x88, 0x10, 
                            0x88, 0x10, 0x08, 0x10 };
const PBYTE chr_M_46[14] = { 0xF8, 0x1F, 0x88, 0x00, 0x88, 0x00, 0x88, 0x00, 0x88, 0x00, 
                            0x08, 0x00, 0x08, 0x00 };
const PBYTE chr_M_47[14] = { 0xE0, 0x07, 0x10, 0x08, 0x08, 0x10, 0x08, 0x11, 0x08, 0x11, 
                            0x08, 0x11, 0x10, 0x0E };
const PBYTE chr_M_48[12] = { 0xF8, 0x1F, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 
                            0xF8, 0x1F };
const PBYTE chr_M_49[6] = { 0x08, 0x10, 0xF8, 0x1F, 0x08, 0x10 };
const PBYTE chr_M_4A[14] = { 0x00, 0x04, 0x00, 0x08, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 
                            0x00, 0x08, 0xF8, 0x07 };
const PBYTE chr_M_4B[14] = { 0xF8, 0x1F, 0x80, 0x00, 0x40, 0x01, 0x20, 0x02, 0x10, 0x04, 
                            0x08, 0x08, 0x00, 0x10 };
const PBYTE chr_M_4C[12] = { 0xF8, 0x1F, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 
                            0x00, 0x10 };
const PBYTE chr_M_4D[18] = { 0xF8, 0x1F, 0x18, 0x00, 0x60, 0x00, 0x80, 0x01, 0x00, 0x06, 
                            0x80, 0x01, 0x60, 0x00, 0x18, 0x00, 0xF8, 0x1F };
const PBYTE chr_M_4E[14] = { 0xF8, 0x1F, 0x18, 0x00, 0x60, 0x00, 0x80, 0x01, 0x00, 0x06, 
                            0x00, 0x18, 0xF8, 0x1F };
const PBYTE chr_M_4F[14] = { 0xE0, 0x07, 0x10, 0x08, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 
                            0x10, 0x08, 0xE0, 0x07 };
const PBYTE chr_M_50[14] = { 0xF8, 0x1F, 0x08, 0x02, 0x08, 0x02, 0x08, 0x02, 0x08, 0x02, 
                            0x10, 0x01, 0xE0, 0x00 };
const PBYTE chr_M_51[14] = { 0xE0, 0x07, 0x10, 0x08, 0x08, 0x10, 0x08, 0x10, 0x08, 0x30, 
                            0x10, 0x48, 0xE0, 0x47 };
const PBYTE chr_M_52[14] = { 0xF8, 0x1F, 0x08, 0x01, 0x08, 0x01, 0x08, 0x03, 0x08, 0x05, 
                            0x90, 0x08, 0x60, 0x10 };
const PBYTE chr_M_53[14] = { 0x60, 0x04, 0x90, 0x08, 0x88, 0x10, 0x08, 0x11, 0x08, 0x11, 
                            0x10, 0x09, 0x20, 0x06 };
const PBYTE chr_M_54[14] = { 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0xF8, 0x1F, 0x08, 0x00, 
                            0x08, 0x00, 0x08, 0x00 };
const PBYTE chr_M_55[14] = { 0xF8, 0x07, 0x00, 0x08, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 
                            0x00, 0x08, 0xF8, 0x07 };
const PBYTE chr_M_56[14] = { 0x78, 0x00, 0x80, 0x01, 0x00, 0x06, 0x00, 0x18, 0x00, 0x06, 
                            0x80, 0x01, 0x78, 0x00 };
const PBYTE chr_M_57[18] = { 0xF8, 0x00, 0x00, 0x07, 0x00, 0x18, 0x00, 0x06, 0xC0, 0x01, 
                            0x00, 0x06, 0x00, 0x18, 0x00, 0x07, 0xF8, 0x00 };
const PBYTE chr_M_58[14] = { 0x18, 0x18, 0x20, 0x04, 0x40, 0x02, 0x80, 0x01, 0x40, 0x02, 
                            0x20, 0x04, 0x18, 0x18 };
const PBYTE chr_M_59[14] = { 0x38, 0x00, 0x40, 0x00, 0x80, 0x00, 0x00, 0x1F, 0x80, 0x00, 
                            0x40, 0x00, 0x38, 0x00 };
const PBYTE chr_M_5A[14] = { 0x08, 0x18, 0x08, 0x14, 0x08, 0x12, 0x88, 0x11, 0x48, 0x10, 
                            0x28, 0x10, 0x18, 0x10 };
const PBYTE chr_M_5B[6] = { 0xFC, 0x3F, 0x04, 0x20, 0x04, 0x20 };
const PBYTE chr_M_5C[12] = { 0x00, 0x30, 0x00, 0x0C, 0x00, 0x03, 0xC0, 0x00, 0x30, 0x00, 
                            0x0C, 0x00 };
const PBYTE chr_M_5D[6] = { 0x04, 0x20, 0x04, 0x20, 0xFC, 0x3F };
const PBYTE chr_M_5E[32] = { 0xF0, 0x3F, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 
                            0xF0, 0x3F, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 
                            0xF0, 0x3F, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 
                            0xF0, 0x3F };
const PBYTE chr_M_5F[32] = { 0xC0, 0x00, 0x30, 0x00, 0x08, 0x30, 0x10, 0x0C, 0x20, 0x03, 
                            0xC0, 0x00, 0x30, 0x00, 0x08, 0x30, 0x10, 0x0C, 0x20, 0x03, 
                            0xC0, 0x00, 0x30, 0x00, 0x08, 0x30, 0x10, 0x0C, 0x20, 0x03, 
                            0xC0, 0x00 };
const PBYTE chr_M_60[6] = { 0x00, 0x00, 0x38, 0x00, 0x40, 0x00 };
const PBYTE chr_M_61[12] = { 0x00, 0x0C, 0x40, 0x12, 0x40, 0x12, 0x40, 0x12, 0x80, 0x0C, 
                            0x00, 0x1F };
const PBYTE chr_M_62[12] = { 0xF0, 0x1F, 0x80, 0x08, 0x40, 0x10, 0x40, 0x10, 0x80, 0x08, 
                            0x00, 0x07 };
const PBYTE chr_M_63[12] = { 0x00, 0x07, 0x80, 0x08, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 
                            0x80, 0x08 };
const PBYTE chr_M_64[12] = { 0x00, 0x07, 0x80, 0x08, 0x40, 0x10, 0x40, 0x10, 0x80, 0x08, 
                            0xF0, 0x1F };
const PBYTE chr_M_65[12] = { 0x00, 0x07, 0x80, 0x0A, 0x40, 0x12, 0x40, 0x12, 0x40, 0x12, 
                            0x80, 0x09 };
const PBYTE chr_M_66[10] = { 0x00, 0x01, 0xF0, 0x1F, 0x08, 0x01, 0x08, 0x00, 0x10, 0x00 };
const PBYTE chr_M_67[12] = { 0x00, 0x07, 0x80, 0x88, 0x40, 0x90, 0x40, 0x90, 0x80, 0x48, 
                            0x80, 0x3F };
const PBYTE chr_M_68[12] = { 0xF8, 0x1F, 0x80, 0x00, 0x40, 0x00, 0x40, 0x00, 0x80, 0x00, 
                            0x00, 0x1F };
const PBYTE chr_M_69[8] = { 0x00, 0x00, 0xD0, 0x1F, 0x00, 0x00, 0x00, 0x00 };
const PBYTE chr_M_6A[8] = { 0x00, 0x40, 0x00, 0x80, 0x40, 0x80, 0xD0, 0x7F };
const PBYTE chr_M_6B[10] = { 0xF8, 0x1F, 0x00, 0x02, 0x00, 0x05, 0x80, 0x08, 0x40, 0x10 };
const PBYTE chr_M_6C[8] = { 0x08, 0x00, 0xF8, 0x1F, 0x00, 0x00, 0x00, 0x00 };
const PBYTE chr_M_6D[14] = { 0xC0, 0x1F, 0x80, 0x00, 0x40, 0x00, 0x80, 0x1F, 0x40, 0x00, 
                            0x40, 0x00, 0x80, 0x1F };
const PBYTE chr_M_6E[12] = { 0xC0, 0x1F, 0x80, 0x00, 0x40, 0x00, 0x40, 0x00, 0x80, 0x00, 
                            0x00, 0x1F };
const PBYTE chr_M_6F[14] = { 0x00, 0x07, 0x80, 0x08, 0x40, 0x10, 0x40, 0x10, 0x40, 0x10, 
                            0x80, 0x08, 0x00, 0x07 };
const PBYTE chr_M_70[12] = { 0xC0, 0xFF, 0x80, 0x08, 0x40, 0x10, 0x40, 0x10, 0x80, 0x08, 
                            0x00, 0x07 };
const PBYTE chr_M_71[14] = { 0x00, 0x07, 0x80, 0x08, 0x40, 0x10, 0x40, 0x10, 0x80, 0x08, 
                            0xC0, 0xFF, 0x00, 0x80 };
const PBYTE chr_M_72[10] = { 0xC0, 0x1F, 0x80, 0x00, 0x40, 0x00, 0x40, 0x00, 0x80, 0x00 };
const PBYTE chr_M_73[10] = { 0x80, 0x09, 0x40, 0x12, 0x40, 0x12, 0x40, 0x12, 0x80, 0x0C };
const PBYTE chr_M_74[8] = { 0x40, 0x00, 0xF0, 0x0F, 0x40, 0x10, 0x00, 0x10 };
const PBYTE chr_M_75[12] = { 0xC0, 0x07, 0x00, 0x08, 0x00, 0x10, 0x00, 0x10, 0x00, 0x08, 
                            0xC0, 0x1F };
const PBYTE chr_M_76[14] = { 0xC0, 0x03, 0x00, 0x04, 0x00, 0x08, 0x00, 0x10, 0x00, 0x08, 
                            0x00, 0x04, 0xC0, 0x03 };
const PBYTE chr_M_77[14] = { 0xC0, 0x0F, 0x00, 0x10, 0x00, 0x08, 0x00, 0x06, 0x00, 0x08, 
                            0x00, 0x10, 0xC0, 0x0F };
const PBYTE chr_M_78[10] = { 0xC0, 0x18, 0x00, 0x05, 0x00, 0x02, 0x00, 0x05, 0xC0, 0x18 };
const PBYTE chr_M_79[12] = { 0xC0, 0x07, 0x00, 0x88, 0x00, 0x90, 0x00, 0x90, 0x00, 0x48, 
                            0xC0, 0x3F };
const PBYTE chr_M_7A[12] = { 0x40, 0x18, 0x40, 0x14, 0x40, 0x12, 0x40, 0x12, 0x40, 0x11, 
                            0xC0, 0x10 };
const PBYTE chr_M_7B[22] = { 0xE0, 0x03, 0x10, 0x04, 0xC8, 0x09, 0x28, 0x0A, 0x28, 0x0A, 
                            0x28, 0x0A, 0x08, 0x08, 0x10, 0x04, 0xE0, 0x03, 0x00, 0x00, 
                            0x00, 0x00 };
const PBYTE chr_M_7C[32] = { 0xFE, 0x00, 0x12, 0x00, 0x32, 0x00, 0x52, 0x00, 0x8C, 0x00, 
                            0x00, 0x00, 0xE0, 0x0F, 0x20, 0x01, 0x20, 0x01, 0x20, 0x01, 
                            0xC0, 0x00, 0x00, 0x7C, 0x00, 0x12, 0x00, 0x11, 0x00, 0x12, 
                            0x00, 0x7C };
const PBYTE chr_M_7D[16] = { 0x00, 0x80, 0x00, 0x40, 0xC0, 0x3F, 0x00, 0x08, 0x00, 0x10, 
                            0x00, 0x10, 0x00, 0x08, 0xC0, 0x07 };
const PBYTE chr_M_7E[4] = { 0x00, 0x00, 0x00, 0x00 };
const PBYTE chr_M_7F[26] = { 0x00, 0x00, 0x00, 0x00, 0x80, 0x1F, 0x80, 0x00, 0x00, 0x1F, 
                            0x80, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x80, 0x1F, 0x80, 0x00, 
                            0x00, 0x1F, 0x80, 0x00, 0x00, 0x1F };
const PBYTE chr_M_80[30] = { 0x20, 0x18, 0x20, 0x14, 0x20, 0x12, 0x20, 0x11, 0xA0, 0x10, 
                            0x60, 0x10, 0x00, 0x00, 0x80, 0x18, 0x80, 0x14, 0x80, 0x12, 
                            0x80, 0x11, 0x00, 0x00, 0x00, 0x12, 0x00, 0x1A, 0x00, 0x16 };
const PBYTE chr_M_81[14] = { 0xFE, 0xFF, 0x02, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 
                            0x02, 0x80, 0xFE, 0xFF };
const PBYTE chr_M_82[32] = { 0xFF, 0xFF, 0x03, 0xC0, 0x05, 0xA0, 0x09, 0x90, 0x11, 0x88, 
                            0x21, 0x84, 0x41, 0x82, 0x81, 0x81, 0x81, 0x81, 0x41, 0x82, 
                            0x21, 0x84, 0x11, 0x88, 0x09, 0x90, 0x05, 0xA0, 0x03, 0xC0, 
                            0xFF, 0xFF };
const PBYTE chr_M_83[32] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x7F, 0x04, 0x40, 
                            0x04, 0x40, 0x04, 0x40, 0x04, 0x40, 0x04, 0x40, 0x04, 0x40, 
                            0x04, 0x40, 0x04, 0x40, 0x04, 0x40, 0x04, 0x40, 0x04, 0x40, 
                            0x04, 0x40 };
const PBYTE chr_M_84[22] = { 0x04, 0x40, 0x04, 0x40, 0x04, 0x40, 0x04, 0x40, 0x04, 0x41, 
                            0x84, 0x43, 0xC4, 0x47, 0xE4, 0x4F, 0xF4, 0x5F, 0x04, 0x40, 
                            0xFC, 0x7F };
const PBYTE chr_M_85[32] = { 0x04, 0x40, 0x04, 0x40, 0x04, 0x40, 0x04, 0x40, 0x04, 0x40, 
                            0x04, 0x40, 0x04, 0x40, 0x04, 0x40, 0x04, 0x40, 0x04, 0x40, 
                            0xFC, 0x7F, 0x00, 0x01, 0x80, 0x03, 0xC0, 0x07, 0xE0, 0x0F, 
                            0xF0, 0x1F };
const PBYTE chr_M_86[6] = { 0x40, 0x02, 0xE0, 0x02, 0x40, 0x02 };
const PBYTE chr_M_87[10] = { 0xC0, 0x03, 0x40, 0x00, 0x80, 0x03, 0x40, 0x00, 0x80, 0x03 };
const PBYTE chr_M_88[26] = { 0x00, 0x07, 0xC0, 0x18, 0x20, 0x20, 0x10, 0x40, 0x10, 0x40, 
                            0x08, 0x80, 0xF8, 0x83, 0x08, 0x80, 0x10, 0x40, 0x10, 0x40, 
                            0x20, 0x20, 0xC0, 0x18, 0x00, 0x07 };
const PBYTE chr_M_89[26] = { 0x00, 0x07, 0xC0, 0x18, 0x20, 0x20, 0x50, 0x40, 0x90, 0x40, 
                            0x08, 0x81, 0x08, 0x82, 0x08, 0x80, 0x10, 0x40, 0x10, 0x40, 
                            0x20, 0x20, 0xC0, 0x18, 0x00, 0x07 };
const PBYTE chr_M_8A[26] = { 0x00, 0x07, 0xC0, 0x1A, 0x20, 0x22, 0x10, 0x42, 0x10, 0x42, 
                            0x08, 0x82, 0x08, 0x82, 0x08, 0x80, 0x10, 0x40, 0x10, 0x40, 
                            0x20, 0x20, 0xC0, 0x18, 0x00, 0x07 };
const PBYTE chr_M_8B[26] = { 0x00, 0x07, 0xC0, 0x18, 0x20, 0x20, 0x10, 0x50, 0x10, 0x48, 
                            0x08, 0x84, 0x08, 0x82, 0x08, 0x80, 0x10, 0x40, 0x10, 0x40, 
                            0x20, 0x20, 0xC0, 0x18, 0x00, 0x07 };
const PBYTE chr_M_8C[26] = { 0x00, 0x07, 0xC0, 0x18, 0x20, 0x20, 0x10, 0x40, 0x10, 0x40, 
                            0x08, 0x80, 0x08, 0xFE, 0x08, 0x80, 0x10, 0x40, 0x10, 0x40, 
                            0x20, 0x20, 0xC0, 0x18, 0x00, 0x07 };
const PBYTE chr_M_8D[26] = { 0x00, 0x07, 0xC0, 0x18, 0x20, 0x20, 0x10, 0x40, 0x10, 0x40, 
                            0x08, 0x80, 0x08, 0x82, 0x08, 0x84, 0x10, 0x48, 0x10, 0x50, 
                            0x20, 0x20, 0xC0, 0x18, 0x00, 0x07 };
const PBYTE chr_M_8E[26] = { 0x00, 0x07, 0xC0, 0x18, 0x20, 0x20, 0x10, 0x40, 0x10, 0x40, 
                            0x08, 0x80, 0x08, 0x82, 0x08, 0x82, 0x10, 0x42, 0x10, 0x42, 
                            0x20, 0x22, 0xC0, 0x1A, 0x00, 0x07 };
const PBYTE chr_M_8F[26] = { 0x00, 0x07, 0xC0, 0x18, 0x20, 0x20, 0x10, 0x40, 0x10, 0x40, 
                            0x08, 0x80, 0x08, 0x82, 0x08, 0x81, 0x90, 0x40, 0x50, 0x40, 
                            0x20, 0x20, 0xC0, 0x18, 0x00, 0x07 };

const PBYTE * PROGMEM chrtbl_M[112] =
{
           chr_M_20, chr_M_21, chr_M_22, chr_M_23, chr_M_24, chr_M_25, chr_M_26, chr_M_27, 
           chr_M_28, chr_M_29, chr_M_2A, chr_M_2B, chr_M_2C, chr_M_2D, chr_M_2E, chr_M_2F, 
           chr_M_30, chr_M_31, chr_M_32, chr_M_33, chr_M_34, chr_M_35, chr_M_36, chr_M_37, 
           chr_M_38, chr_M_39, chr_M_3A, chr_M_3B, chr_M_3C, chr_M_3D, chr_M_3E, chr_M_3F, 
           chr_M_40, chr_M_41, chr_M_42, chr_M_43, chr_M_44, chr_M_45, chr_M_46, chr_M_47, 
           chr_M_48, chr_M_49, chr_M_4A, chr_M_4B, chr_M_4C, chr_M_4D, chr_M_4E, chr_M_4F, 
           chr_M_50, chr_M_51, chr_M_52, chr_M_53, chr_M_54, chr_M_55, chr_M_56, chr_M_57, 
           chr_M_58, chr_M_59, chr_M_5A, chr_M_5B, chr_M_5C, chr_M_5D, chr_M_5E, chr_M_5F, 
           chr_M_60, chr_M_61, chr_M_62, chr_M_63, chr_M_64, chr_M_65, chr_M_66, chr_M_67, 
           chr_M_68, chr_M_69, chr_M_6A, chr_M_6B, chr_M_6C, chr_M_6D, chr_M_6E, chr_M_6F, 
           chr_M_70, chr_M_71, chr_M_72, chr_M_73, chr_M_74, chr_M_75, chr_M_76, chr_M_77, 
           chr_M_78, chr_M_79, chr_M_7A, chr_M_7B, chr_M_7C, chr_M_7D, chr_M_7E, chr_M_7F, 
           chr_M_80, chr_M_81, chr_M_82, chr_M_83, chr_M_84, chr_M_85, chr_M_86, chr_M_87, 
           chr_M_88, chr_M_89, chr_M_8A, chr_M_8B, chr_M_8C, chr_M_8D, chr_M_8E, chr_M_8F
};