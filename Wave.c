// Usage (encryption): wave plaintext.file ciphertext.file
// Usage (decryption): wave ciphertext.file plaintext.file
// Compiled on MacOS, Linux and *BSD in x86_64 platform.
// Talk is SO EASY, show you my GOD.
// Simple is beautiful.

#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

unsigned long ulFileSize = 0;

unsigned char *pucPlaintextOrCiphertext = NULL;

unsigned long EncryptOrDecrypt(unsigned char *pucPassword, unsigned char *pucKeysTable, unsigned long i)
{
    unsigned long ulKeyTemp, ulKeyIndex, k;

// The pseudo "JunTai" random number processing is performed on a key table.
    for(unsigned long j = 0; j < 32; ++j)
    {
        unsigned long *pulKeySwap1 = (unsigned long*)pucKeysTable, *pulKeySwap2 = (unsigned long*)pucKeysTable;

        ulKeyIndex = (unsigned char)pucPassword[j % 8] % 32;

        ulKeyTemp = pulKeySwap1[j];

        pulKeySwap1[j] = pulKeySwap2[ulKeyIndex];

        pulKeySwap2[ulKeyIndex] = ulKeyTemp;
    }

// use XOR to process the 256 bytes of plaintext / ciphertext data at a time
    for(k = 0; k < 256 && i + k < ulFileSize; ++k)
    {
        pucPlaintextOrCiphertext[i + k] ^= pucKeysTable[k];
    }

// use key table's value to change the password
    for(unsigned long l = 0; l < 8; ++l)
    {
        pucPassword[l] = pucKeysTable[pucPassword[l]];
    }

// returns the number of processed data
    return k;
}

// Passwords and key tables can be more than 8 sets, depending on the need, but to be paired one by one.
// The most important point is that you can multithreaded and encrypt or decrypt files or data in parallel.
int main(int argc, char *argv[])
{
// There are 8 sets of passwords, each with at least 8 ciphers and no more than 255 ciphers.
    unsigned char aucPassword1[8] = {'a', '1', 'B', '~', 'c', '2', 'D', '`'},
                  aucPassword2[8] = {'e', '3', 'F', '!', 'g', '4', 'H', '@'},
                  aucPassword3[8] = {'i', '5', 'J', '#', 'k', '6', 'L', '$'},
                  aucPassword4[8] = {'m', '7', 'N', '%', 'o', '8', 'P', '^'},
                  aucPassword5[8] = {'q', '9', 'R', '&', 's', '0', 'T', '*'},
                  aucPassword6[8] = {'u', 'A', 'V', '(', 'w', 'b', 'X', ')'},
                  aucPassword7[8] = {'y', 'C', 'Z', '-', 'E', 'd', 'f', '_'},
                  aucPassword8[8] = {'G', 'h', 'I', '=', 'j', 'K', 'l', '+'};

// There are 8 sets of key tables, each with 256 values from 0 to 255.
    unsigned char aucKeysTable1[256] = {
        0xa0, 0xd3, 0xc2, 0xc7, 0xe7, 0x53, 0x99, 0xfF, 0xe8, 0x71, 0xe3, 0xaE, 0xc5, 0x72, 0xeE, 0x70,
        0x86, 0x35, 0x0C, 0x27, 0xbE, 0x9E, 0x77, 0xc4, 0xeA, 0x1A, 0x74, 0x9F, 0xbC, 0xaB, 0x0A, 0x1B,
        0x1E, 0xe9, 0xdF, 0x7E, 0xdC, 0xf2, 0xaC, 0x4D, 0x7D, 0x2B, 0x02, 0xcE, 0x6A, 0x97, 0xb5, 0x9C,
        0x6C, 0xf1, 0xeF, 0x7F, 0x00, 0x95, 0x38, 0x20, 0x64, 0x52, 0xc6, 0x07, 0x9D, 0x5C, 0x45, 0x5E,
        0x05, 0xfD, 0xd1, 0x9B, 0xf7, 0xc0, 0x0D, 0x6D, 0xa7, 0xb6, 0xd0, 0x93, 0x7A, 0xb3, 0x6F, 0x22,
        0x88, 0xd9, 0x57, 0x50, 0x84, 0xa2, 0xa3, 0xcF, 0xaF, 0xcD, 0xaD, 0xb7, 0x37, 0x04, 0x11, 0x33,
        0x1C, 0x19, 0x8E, 0xe4, 0xe1, 0xc8, 0xf9, 0x47, 0x01, 0xd5, 0x4B, 0x7B, 0x26, 0x40, 0x2A, 0x56,
        0x79, 0x3B, 0x8B, 0x30, 0x43, 0x8C, 0xd6, 0x17, 0x55, 0x4C, 0xcA, 0x8F, 0xa5, 0x0B, 0x63, 0x1F,
        0xf4, 0xc3, 0x44, 0x5B, 0x62, 0x67, 0x21, 0x59, 0x6B, 0x68, 0xdD, 0xa8, 0xcB, 0x42, 0xa9, 0xa4,
        0x54, 0x1D, 0x12, 0x16, 0x66, 0x3A, 0x32, 0x75, 0x87, 0x69, 0x2C, 0x73, 0xf0, 0x06, 0xf6, 0x51,
        0x91, 0xb8, 0x49, 0xfB, 0x94, 0xbF, 0x0F, 0xe0, 0x2D, 0x76, 0x7C, 0x24, 0xb9, 0x61, 0x96, 0xe5,
        0x3F, 0xeB, 0x14, 0x23, 0x78, 0xb4, 0x09, 0x90, 0xd7, 0x65, 0xe6, 0xf5, 0x31, 0x28, 0x9A, 0x60,
        0xb1, 0x3C, 0x5F, 0xfA, 0xf3, 0x48, 0xc1, 0xeD, 0x80, 0x3E, 0x39, 0x0E, 0xfC, 0x85, 0x83, 0x2F,
        0xdA, 0x18, 0x25, 0x98, 0x2E, 0xb2, 0x34, 0xdE, 0xcC, 0x10, 0xa6, 0xf8, 0x92, 0x5D, 0xe2, 0x81,
        0xdB, 0xc9, 0x5A, 0xa1, 0x89, 0x36, 0xd8, 0x58, 0x82, 0x3D, 0xd4, 0x6E, 0xb0, 0xd2, 0x15, 0x8D,
        0xaA, 0xeC, 0x41, 0xfE, 0x8A, 0x46, 0xbB, 0x4F, 0xbD, 0x03, 0x08, 0x29, 0x4A, 0x13, 0x4E, 0xbA},

                  aucKeysTable2[256] = {
        0x6C, 0x2A, 0x6E, 0xa4, 0xcB, 0xeB, 0x68, 0xc4, 0x27, 0x34, 0xc3, 0xaE, 0xbD, 0x72, 0x9F, 0x78,
        0x03, 0xf8, 0x4F, 0x06, 0x3F, 0x6D, 0xbE, 0x9C, 0x77, 0xd4, 0xaF, 0xf1, 0xc0, 0x82, 0xaA, 0x89,
        0x58, 0x79, 0x07, 0xc7, 0x3B, 0x37, 0x44, 0x13, 0x81, 0x71, 0x9B, 0x23, 0xe1, 0xa5, 0x73, 0x15,
        0xbB, 0x2F, 0x3D, 0x25, 0xaB, 0x8C, 0x31, 0x96, 0x76, 0xcF, 0xb7, 0xdD, 0x02, 0x70, 0x1D, 0xfD,
        0x86, 0x55, 0xbC, 0x16, 0x35, 0x90, 0x66, 0x56, 0xbF, 0xc2, 0x19, 0xeC, 0x8D, 0x05, 0x80, 0x04,
        0x93, 0x67, 0x45, 0x09, 0x48, 0xe6, 0x47, 0xd6, 0x6A, 0x4C, 0xeA, 0xfC, 0xc8, 0x91, 0xa9, 0xd3,
        0xe0, 0x95, 0x08, 0x20, 0x87, 0xc6, 0xe5, 0x10, 0xd0, 0x6F, 0xd9, 0xcE, 0x9E, 0x32, 0x1C, 0x26,
        0x7A, 0x75, 0x99, 0x63, 0x42, 0x6B, 0xb6, 0xdC, 0x9D, 0x4A, 0x43, 0x0C, 0x38, 0xf6, 0x01, 0xa0,
        0xeF, 0x33, 0x4D, 0x54, 0x17, 0xdA, 0x92, 0xa2, 0x5D, 0x3E, 0x22, 0x8A, 0xaC, 0xfF, 0x7F, 0xb1,
        0x8B, 0x97, 0x98, 0xd2, 0xc9, 0xcA, 0xe9, 0xa6, 0x2E, 0x5E, 0x1B, 0xa1, 0x39, 0x7C, 0x3A, 0xb9,
        0xa7, 0x50, 0xe3, 0x62, 0x12, 0xb8, 0x11, 0x7D, 0x2D, 0xeD, 0xb5, 0xd8, 0xa3, 0xdE, 0x61, 0xeE,
        0xe8, 0x57, 0xd5, 0x4E, 0x41, 0x3C, 0xf3, 0xbA, 0x74, 0xc1, 0x7E, 0x1F, 0x2B, 0x0D, 0xf2, 0xcD,
        0x51, 0x28, 0x5A, 0x18, 0xf4, 0xb2, 0x21, 0xd1, 0x0E, 0xa8, 0x1A, 0x0A, 0xf7, 0xe4, 0x85, 0x7B,
        0x46, 0x40, 0xdB, 0x30, 0x64, 0x1E, 0x9A, 0xe7, 0xfA, 0x8F, 0xdF, 0x24, 0x14, 0xb4, 0x60, 0x00,
        0x5C, 0x88, 0xe2, 0xaD, 0xfB, 0x0B, 0x2C, 0xb0, 0xcC, 0x84, 0x53, 0xb3, 0x94, 0x59, 0xf5, 0x49,
        0x8E, 0x83, 0xf0, 0xc5, 0x52, 0xfE, 0x69, 0x36, 0x65, 0xd7, 0x4B, 0x0F, 0xf9, 0x5F, 0x29, 0x5B},

                  aucKeysTable3[256] = {
        0x84, 0xeD, 0x0A, 0xd5, 0x38, 0x7B, 0x6F, 0x88, 0x2E, 0x99, 0xc3, 0x36, 0x0E, 0x61, 0x56, 0x24,
        0x3D, 0x54, 0xd2, 0xcD, 0x5C, 0x18, 0x66, 0x26, 0x92, 0x43, 0x5A, 0xc0, 0x0F, 0x04, 0x78, 0xc6,
        0xbC, 0xa8, 0x9C, 0x91, 0x11, 0xa3, 0x77, 0x07, 0x20, 0xc5, 0x6A, 0xaC, 0x2C, 0x06, 0x31, 0x47,
        0xbA, 0xa2, 0x52, 0x70, 0x6E, 0xdA, 0x5B, 0x9B, 0xaF, 0x86, 0xf2, 0x79, 0xeB, 0x7F, 0x89, 0x50,
        0x13, 0x8D, 0x8C, 0xd3, 0x94, 0xb5, 0x17, 0x19, 0xaE, 0x0B, 0xcF, 0x27, 0x9E, 0x87, 0x35, 0x30,
        0x59, 0xd8, 0x9D, 0x3B, 0xc4, 0x2F, 0x14, 0x22, 0x0C, 0x8B, 0x7D, 0x5F, 0xb3, 0x72, 0xb4, 0xd6,
        0x12, 0xbD, 0x85, 0xb8, 0xbF, 0xf8, 0x4D, 0xe3, 0x00, 0x05, 0x8A, 0x41, 0xeF, 0xaA, 0xa0, 0x4F,
        0xd7, 0xe4, 0x40, 0x49, 0xa9, 0x42, 0xbE, 0x9F, 0xc7, 0xdC, 0x97, 0x32, 0x3C, 0x1E, 0x63, 0xfD,
        0x67, 0x02, 0x23, 0xdB, 0xd9, 0xb6, 0x74, 0xe1, 0x93, 0xb7, 0xe6, 0xdD, 0x1B, 0x8F, 0x1F, 0x58,
        0xd0, 0x44, 0xb1, 0xa4, 0x82, 0xc9, 0x96, 0x34, 0x76, 0x90, 0x4B, 0x4E, 0xc8, 0x25, 0x7A, 0xcE,
        0xcB, 0x1C, 0xe2, 0x81, 0xeC, 0x75, 0xf6, 0x3F, 0x4A, 0x3A, 0xc2, 0x60, 0xb2, 0x57, 0x6C, 0x64,
        0x7E, 0x68, 0x2D, 0x1D, 0xf0, 0x45, 0xe0, 0x10, 0xe5, 0xfA, 0xdE, 0x83, 0x51, 0xa1, 0x9A, 0x46,
        0xe7, 0x5E, 0xd1, 0x28, 0x6B, 0xa7, 0x0D, 0xc1, 0x95, 0x4C, 0x09, 0x73, 0x80, 0xfC, 0x37, 0x62,
        0xfE, 0x39, 0x16, 0x98, 0xb9, 0xbB, 0xd4, 0x2B, 0xf3, 0x69, 0xf1, 0x53, 0xa5, 0xeE, 0xcC, 0x7C,
        0xaB, 0xcA, 0x48, 0x65, 0x08, 0x6D, 0xaD, 0x01, 0xf7, 0x1A, 0x8E, 0xfB, 0xf9, 0x71, 0xe9, 0xf4,
        0x03, 0x33, 0xa6, 0x5D, 0x55, 0xdF, 0xe8, 0x29, 0xfF, 0x15, 0x3E, 0x2A, 0xf5, 0x21, 0xeA, 0xb0},

                  aucKeysTable4[256] = {
        0x3F, 0x58, 0x0C, 0x14, 0x3E, 0x19, 0x81, 0x35, 0x18, 0xdF, 0xe1, 0x25, 0x65, 0x0B, 0xf1, 0x6A,
        0x70, 0x26, 0x86, 0x1A, 0x41, 0xe4, 0x37, 0x28, 0xf6, 0x49, 0x6B, 0xd2, 0xbB, 0x69, 0x9B, 0x34,
        0xfA, 0x2D, 0x00, 0x40, 0x6E, 0xbD, 0xeE, 0x1F, 0xe8, 0xd4, 0xc1, 0x43, 0x59, 0x8F, 0xc8, 0xf0,
        0x80, 0x4B, 0x33, 0xaA, 0x6C, 0xd5, 0xa1, 0x54, 0xb6, 0x5B, 0x5C, 0x90, 0x9D, 0xcB, 0x11, 0x57,
        0xd9, 0x07, 0xd8, 0x5E, 0xaF, 0x77, 0x9C, 0x75, 0xfB, 0x71, 0x95, 0xc5, 0x3D, 0x55, 0x7F, 0x6F,
        0xdB, 0x8E, 0x10, 0xa8, 0x7D, 0x47, 0xc7, 0x27, 0xa2, 0x97, 0x67, 0xbC, 0xb1, 0x51, 0xc9, 0x66,
        0x7E, 0xfF, 0x29, 0xb3, 0x73, 0x8C, 0x4D, 0xd1, 0x79, 0xb5, 0x32, 0xd7, 0xeC, 0x21, 0x74, 0x1C,
        0x08, 0x4A, 0xfE, 0xaB, 0xf2, 0xd3, 0xb0, 0xc2, 0xb4, 0xdC, 0xaD, 0x8B, 0x16, 0xf9, 0x04, 0x8D,
        0x44, 0x53, 0x0A, 0x42, 0x02, 0x7C, 0x06, 0x62, 0xc3, 0x0E, 0xb2, 0xcD, 0xb7, 0xcE, 0x38, 0x36,
        0x3A, 0x03, 0x45, 0xe6, 0xbE, 0xa0, 0x4F, 0x13, 0x89, 0x46, 0xdE, 0x2F, 0xd0, 0xcF, 0x64, 0xeA,
        0x20, 0x88, 0x83, 0x05, 0xf3, 0xb9, 0xe9, 0x87, 0xaE, 0x4E, 0x3C, 0xf8, 0x09, 0xb8, 0xfD, 0x5A,
        0xbA, 0x84, 0x93, 0x30, 0xe3, 0xa9, 0xc6, 0xa4, 0x9A, 0x76, 0x91, 0xf5, 0x2B, 0x0F, 0x96, 0xa5,
        0x94, 0x48, 0xe2, 0x1B, 0x7A, 0x60, 0x1E, 0x52, 0x15, 0x9E, 0xe7, 0x6D, 0x7B, 0xfC, 0xeF, 0x72,
        0x9F, 0xeD, 0x78, 0x63, 0x98, 0x85, 0xdD, 0xbF, 0x56, 0x01, 0x50, 0x23, 0x5D, 0x31, 0x5F, 0xc0,
        0xa6, 0x61, 0xf7, 0xcA, 0xdA, 0x22, 0xa7, 0xaC, 0x2E, 0xd6, 0xa3, 0x12, 0x39, 0x82, 0xe5, 0xeB,
        0x24, 0x92, 0xf4, 0x68, 0x0D, 0x4C, 0x8A, 0x3B, 0x2C, 0x17, 0xe0, 0x1D, 0xc4, 0x99, 0xcC, 0x2A},

                  aucKeysTable5[256] = {
        0x4D, 0x50, 0x6E, 0xa7, 0x9C, 0xb6, 0x98, 0x34, 0xa4, 0x1B, 0x09, 0x43, 0x5B, 0x72, 0x1C, 0xdA,
        0x70, 0x33, 0xe6, 0xaD, 0x5A, 0xeA, 0x7E, 0xbA, 0x54, 0xf4, 0x99, 0xd2, 0x9D, 0x92, 0x2C, 0xc8,
        0x2A, 0x56, 0x30, 0xcC, 0xc2, 0xa3, 0x82, 0x67, 0x6F, 0xeB, 0x28, 0x0C, 0xdC, 0xb7, 0x71, 0x6B,
        0x86, 0x04, 0xaB, 0x7A, 0xaF, 0x31, 0x95, 0x0F, 0x69, 0x48, 0xc0, 0x79, 0xa6, 0x53, 0xaC, 0x81,
        0x9E, 0x02, 0xd1, 0xfE, 0x11, 0x60, 0x06, 0x7C, 0xa0, 0xcD, 0x29, 0xb2, 0xf5, 0x2B, 0x10, 0x66,
        0xbB, 0x4F, 0xf9, 0x2F, 0x32, 0x75, 0x88, 0xfF, 0x3C, 0xb1, 0x0D, 0x5F, 0x74, 0x85, 0xb3, 0x14,
        0x58, 0x4B, 0x51, 0xeC, 0x45, 0xe3, 0xa5, 0xa9, 0xeE, 0x20, 0xf0, 0xc1, 0x61, 0x1D, 0x90, 0x2D,
        0x3A, 0x18, 0xfB, 0x8A, 0xb4, 0xbC, 0x01, 0xd8, 0x37, 0xb0, 0x4C, 0x24, 0xd4, 0x73, 0x1A, 0x19,
        0x9F, 0xbE, 0xfC, 0x12, 0x59, 0x25, 0x3E, 0x23, 0x5E, 0x8F, 0xaE, 0x42, 0x1F, 0xfD, 0x08, 0xdD,
        0x94, 0x05, 0xf3, 0xe2, 0xa2, 0x6C, 0x44, 0x8D, 0x7B, 0xd0, 0xa8, 0xbF, 0x57, 0x3F, 0xe0, 0x1E,
        0x55, 0x03, 0x83, 0xcF, 0x36, 0x13, 0x0E, 0x3D, 0xeF, 0xd3, 0xdF, 0x8E, 0xb5, 0x52, 0xe7, 0x96,
        0xf7, 0x6A, 0xc4, 0xf1, 0x87, 0xd6, 0x68, 0xcE, 0x40, 0x07, 0xc7, 0xdB, 0x78, 0xa1, 0xf2, 0x7D,
        0xb8, 0x77, 0x21, 0x8B, 0x80, 0xcB, 0xc9, 0xe1, 0x97, 0xd7, 0xd5, 0x39, 0xdE, 0x6D, 0x89, 0x22,
        0x49, 0xf6, 0x84, 0x64, 0x26, 0x3B, 0x15, 0xe5, 0x0B, 0xfA, 0x8C, 0x0A, 0x63, 0x35, 0x00, 0x91,
        0xeD, 0x46, 0x9A, 0x5C, 0x47, 0x5D, 0x16, 0xb9, 0x27, 0xc5, 0x4E, 0xcA, 0xe9, 0xe8, 0x7F, 0x17,
        0x62, 0xc6, 0xd9, 0xaA, 0x41, 0xf8, 0x9B, 0x76, 0xc3, 0xbD, 0x93, 0x2E, 0x38, 0x65, 0x4A, 0xe4},

                  aucKeysTable6[256] = {
        0xe3, 0xa7, 0x68, 0x15, 0x20, 0x96, 0x61, 0xaD, 0x46, 0x09, 0x40, 0x51, 0xa5, 0xe1, 0x06, 0x33,
        0x2D, 0x05, 0x71, 0xd7, 0x45, 0x90, 0x19, 0x5F, 0x2F, 0x37, 0x6D, 0x57, 0x99, 0xbD, 0x6B, 0x39,
        0x38, 0xb6, 0x64, 0x31, 0x7B, 0x83, 0x89, 0x47, 0x49, 0xeF, 0x8A, 0xf0, 0xaB, 0xeE, 0xbC, 0x0C,
        0x7C, 0xdC, 0x48, 0xeD, 0xe4, 0x24, 0x82, 0xd0, 0x5B, 0x67, 0x8C, 0x80, 0x9F, 0xd8, 0x76, 0x91,
        0x77, 0x97, 0xfB, 0x02, 0x72, 0xbA, 0x00, 0x28, 0x85, 0x18, 0xc6, 0x94, 0x7A, 0x74, 0x3A, 0xd6,
        0x4C, 0x07, 0x4D, 0xa2, 0x0A, 0xa6, 0xc7, 0x13, 0xd4, 0x84, 0x0E, 0xcB, 0x1D, 0x6F, 0xb9, 0x62,
        0xa9, 0xaE, 0x4E, 0xc2, 0x4F, 0xd1, 0xbF, 0xfA, 0x60, 0x69, 0xe7, 0x63, 0xbE, 0x29, 0x6C, 0xc5,
        0x44, 0x95, 0x88, 0x8F, 0xc0, 0x4A, 0x9B, 0xcE, 0x9C, 0xb7, 0x1A, 0x6A, 0x36, 0x1F, 0x42, 0xf1,
        0x1C, 0xa3, 0xa1, 0x58, 0x54, 0x10, 0x56, 0xfF, 0x0B, 0x9D, 0xc1, 0x14, 0x93, 0x1E, 0x98, 0xd5,
        0x16, 0x8D, 0x78, 0x7D, 0xaF, 0x22, 0x41, 0xcD, 0xf2, 0xeB, 0x8E, 0x86, 0xf8, 0xdE, 0xf6, 0xc3,
        0xcF, 0xaA, 0xdF, 0x66, 0xd9, 0x70, 0xb1, 0x3C, 0x32, 0x55, 0x01, 0x79, 0x53, 0xdB, 0xe8, 0x1B,
        0x0D, 0x0F, 0x2C, 0x75, 0x73, 0x65, 0xb2, 0x2E, 0x87, 0x34, 0x50, 0xfC, 0xf5, 0xeC, 0x04, 0xd2,
        0x81, 0x7E, 0x27, 0x11, 0x5A, 0xdA, 0xe9, 0xeA, 0x3F, 0x4B, 0xb8, 0xa0, 0xbB, 0x23, 0xa8, 0xcC,
        0x12, 0x17, 0x9A, 0x6E, 0x52, 0xa4, 0xe6, 0x8B, 0xe5, 0xf7, 0xb3, 0x2B, 0x3B, 0x43, 0x03, 0x35,
        0xe2, 0xf9, 0xe0, 0x59, 0xd3, 0xaC, 0x25, 0xb0, 0xc8, 0x5D, 0x21, 0xc9, 0xb5, 0xf4, 0x7F, 0x26,
        0xdD, 0xfE, 0x3D, 0xcA, 0x3E, 0x92, 0xb4, 0xc4, 0xfD, 0xf3, 0x08, 0x5E, 0x30, 0x5C, 0x2A, 0x9E},

                  aucKeysTable7[256] = {
        0xb9, 0x9B, 0x10, 0x27, 0x26, 0xb6, 0x62, 0x29, 0x4B, 0x9C, 0x15, 0x0D, 0xc2, 0xaC, 0xeD, 0x7F,
        0xa2, 0x59, 0x02, 0x6D, 0xc7, 0x5F, 0xf5, 0x69, 0x7C, 0x6F, 0x24, 0x98, 0xfC, 0xb5, 0x61, 0x0F,
        0x09, 0x75, 0x3F, 0x41, 0xb3, 0x3E, 0x76, 0x92, 0x1C, 0xeC, 0xf8, 0x80, 0xaA, 0xbE, 0xa4, 0x5D,
        0x57, 0xa5, 0x1E, 0x79, 0x5E, 0x91, 0xe4, 0xe9, 0x54, 0x1D, 0xd8, 0x94, 0x97, 0x90, 0xb1, 0x9E,
        0x5C, 0xe3, 0x95, 0xeF, 0xc8, 0xfA, 0x17, 0x04, 0x4E, 0xbB, 0x11, 0xdB, 0xdC, 0x81, 0xfE, 0xe2,
        0xbF, 0x89, 0x0B, 0x77, 0x19, 0x38, 0x25, 0x32, 0x2B, 0x60, 0x48, 0xbC, 0x8A, 0xcE, 0x34, 0x1B,
        0x8B, 0x63, 0x8E, 0xf2, 0x4C, 0xcC, 0x9A, 0xb7, 0x8D, 0x21, 0xf4, 0x71, 0xa6, 0xc9, 0xdA, 0x68,
        0x0E, 0x87, 0xf6, 0xc3, 0x22, 0x07, 0xc1, 0x39, 0xa3, 0x82, 0xb8, 0x18, 0xfF, 0xd1, 0x03, 0x36,
        0x99, 0xcA, 0xc6, 0xd4, 0x05, 0x31, 0xcD, 0x84, 0x43, 0xfB, 0x2C, 0x23, 0x01, 0xd7, 0x12, 0xc5,
        0x96, 0x6C, 0x73, 0x9D, 0xd9, 0x65, 0x70, 0x37, 0xb2, 0x20, 0x85, 0x66, 0xa1, 0xd0, 0x88, 0xb0,
        0x3A, 0x7E, 0x72, 0xa0, 0xa7, 0x16, 0xf3, 0x51, 0xfD, 0xf0, 0x08, 0x2A, 0xe1, 0x47, 0x58, 0x64,
        0x35, 0xeA, 0xe5, 0x06, 0x6B, 0x67, 0xcF, 0xb4, 0x6E, 0x1A, 0xe8, 0x78, 0x45, 0x3B, 0x5A, 0xd2,
        0x49, 0xaD, 0x83, 0x6A, 0x13, 0x4D, 0x8F, 0x50, 0x86, 0x53, 0x2F, 0xeB, 0x42, 0xf1, 0x3D, 0x7B,
        0x46, 0xdE, 0xd3, 0x52, 0xf7, 0xeE, 0x74, 0x40, 0xa8, 0xd6, 0xaF, 0xbD, 0xdD, 0x8C, 0x44, 0xc4,
        0xe7, 0xaB, 0x2D, 0x28, 0x1F, 0x56, 0x14, 0x0A, 0xbA, 0x4F, 0x3C, 0xaE, 0x4A, 0xc0, 0x2E, 0x00,
        0x93, 0xa9, 0x33, 0xd5, 0x30, 0xdF, 0xcB, 0xf9, 0x5B, 0x7D, 0x9F, 0xe0, 0x7A, 0xe6, 0x0C, 0x55},

                  aucKeysTable8[256] = {
        0xc2, 0x4F, 0x27, 0x7A, 0x59, 0x62, 0x80, 0xa8, 0xfD, 0x03, 0x96, 0x8F, 0x88, 0x72, 0x20, 0xc9,
        0x63, 0xf2, 0xeC, 0x14, 0x4D, 0x67, 0xdE, 0x36, 0x51, 0x6B, 0x76, 0x1C, 0xf7, 0x2A, 0x9A, 0x00,
        0x8D, 0xb1, 0x6A, 0x7B, 0x1E, 0x17, 0x52, 0x8E, 0xe9, 0xcB, 0xdD, 0x97, 0x3D, 0xc5, 0x90, 0x43,
        0x12, 0xd8, 0xd6, 0xa5, 0xe7, 0xbB, 0x10, 0xf3, 0x54, 0xf8, 0xb9, 0x2C, 0xb0, 0x42, 0x3E, 0xc3,
        0x2E, 0x64, 0x6D, 0x57, 0x4A, 0x01, 0x5F, 0x71, 0x4B, 0x35, 0xb4, 0x4E, 0x77, 0xcE, 0x92, 0x95,
        0x87, 0x4C, 0x33, 0x7C, 0x1A, 0x5C, 0x9E, 0xb3, 0xa2, 0x6C, 0x89, 0xaB, 0xa4, 0xdB, 0x99, 0xb7,
        0xe8, 0xbC, 0xc6, 0xfC, 0xd4, 0x5D, 0x83, 0xdA, 0x13, 0x5A, 0x1B, 0x82, 0xd0, 0xdC, 0xfA, 0xf9,
        0xbD, 0x94, 0xaC, 0x8A, 0xf4, 0x49, 0x61, 0x26, 0x6F, 0xb5, 0x68, 0x9F, 0x04, 0x18, 0xd1, 0xcA,
        0xb2, 0x7D, 0x40, 0x38, 0x1D, 0x08, 0x50, 0xa7, 0x19, 0x9C, 0xaE, 0xc0, 0xeF, 0x06, 0x73, 0x1F,
        0xfE, 0x0B, 0x23, 0xa0, 0x47, 0x22, 0x0E, 0x15, 0x69, 0x53, 0x70, 0x78, 0x45, 0x5B, 0xeA, 0x85,
        0xf1, 0xa6, 0x3F, 0xd7, 0xd9, 0x60, 0x75, 0xaD, 0xdF, 0x58, 0xd2, 0x7E, 0x21, 0x07, 0xfB, 0xe2,
        0x46, 0xeD, 0x48, 0xcF, 0xc8, 0xc7, 0xeB, 0x44, 0x8B, 0x66, 0xe4, 0x86, 0xe6, 0xbA, 0x98, 0xbE,
        0x2B, 0x2D, 0x39, 0xf6, 0xaF, 0x2F, 0x30, 0x31, 0x02, 0x3C, 0x0F, 0x3A, 0x11, 0xf5, 0x65, 0xcC,
        0xcD, 0xc4, 0xf0, 0x7F, 0x81, 0xe3, 0xe1, 0x84, 0x93, 0x3B, 0x16, 0x29, 0x6E, 0xb8, 0xa3, 0xfF,
        0x25, 0xd3, 0x41, 0x0A, 0xeE, 0x91, 0xa1, 0x55, 0x28, 0xaA, 0x74, 0xd5, 0x5E, 0xc1, 0x09, 0x32,
        0x37, 0xb6, 0x05, 0xe5, 0x24, 0x9D, 0x9B, 0x56, 0xa9, 0x0C, 0x79, 0x8C, 0x0D, 0x34, 0xe0, 0xbF};

    struct stat statFileSize;

    stat(argv[1], &statFileSize);

// get plaintext / ciphertext file size
    ulFileSize = statFileSize.st_size;

// allocate data storage space
    pucPlaintextOrCiphertext = (unsigned char*)malloc(ulFileSize);

// open plaintext / ciphertext file
    int iPlaintextOrCiphertext = open(argv[1], O_RDONLY, S_IREAD | S_IWRITE);

// read data from plaintext / ciphertext file
    read(iPlaintextOrCiphertext, pucPlaintextOrCiphertext, ulFileSize);

    close(iPlaintextOrCiphertext);

// process plaintext / ciphertext data
    for(unsigned long i = 0; i < ulFileSize;)
    {
        i += EncryptOrDecrypt(aucPassword1, aucKeysTable1, i);

        if(i >= ulFileSize) break;

        i += EncryptOrDecrypt(aucPassword2, aucKeysTable2, i);

        if(i >= ulFileSize) break;

        i += EncryptOrDecrypt(aucPassword3, aucKeysTable3, i);

        if(i >= ulFileSize) break;

        i += EncryptOrDecrypt(aucPassword4, aucKeysTable4, i);

        if(i >= ulFileSize) break;

        i += EncryptOrDecrypt(aucPassword5, aucKeysTable5, i);

        if(i >= ulFileSize) break;

        i += EncryptOrDecrypt(aucPassword6, aucKeysTable6, i);

        if(i >= ulFileSize) break;

        i += EncryptOrDecrypt(aucPassword7, aucKeysTable7, i);

        if(i >= ulFileSize) break;

        i += EncryptOrDecrypt(aucPassword8, aucKeysTable8, i);
    }

// open ciphertext / plaintext file
    iPlaintextOrCiphertext = open(argv[2], O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

// write data to ciphertext / plaintext file
    write(iPlaintextOrCiphertext, pucPlaintextOrCiphertext, ulFileSize);

    close(iPlaintextOrCiphertext);

    free(pucPlaintextOrCiphertext);

    return 0;
}
