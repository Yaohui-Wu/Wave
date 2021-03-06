// Usage (encryption): lang plaintext.file ciphertext.file password
// Usage (decryption): lang ciphertext.file plaintext.file password
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
// There are 8 sets of key tables, each with 256 values from 0 to 255.
    unsigned char aucKeysTable1[256] = {
        0xb3, 0x2E, 0x06, 0xa7, 0xdF, 0xb6, 0x34, 0xe0, 0x95, 0x16, 0xe5, 0x8D, 0x1A, 0x9E, 0x1E, 0xbB,
        0x70, 0x9B, 0x23, 0xfE, 0x4C, 0xcD, 0xdB, 0x24, 0x55, 0xaE, 0xc6, 0x04, 0xfA, 0xfC, 0x75, 0x7C,
        0x27, 0x85, 0x54, 0x6A, 0xeE, 0x71, 0x02, 0x69, 0xb8, 0x5C, 0xd5, 0x5B, 0x0B, 0xcA, 0x3B, 0x09,
        0x0A, 0x57, 0x0E, 0xa8, 0xeB, 0x14, 0x05, 0xe8, 0x12, 0x8C, 0xe7, 0x3A, 0x47, 0x07, 0x87, 0x49,
        0x92, 0x0C, 0x53, 0x80, 0x10, 0xf9, 0x2D, 0xcF, 0xd3, 0x45, 0x52, 0x51, 0x72, 0x62, 0x98, 0x82,
        0x7B, 0xaC, 0x26, 0xb2, 0x5F, 0xf6, 0xc2, 0xeC, 0x4D, 0x35, 0x13, 0xb1, 0xe4, 0x89, 0x11, 0x94,
        0x8B, 0xc5, 0x28, 0xa4, 0xd1, 0x01, 0x83, 0x60, 0x50, 0x20, 0x90, 0xa6, 0x84, 0xcB, 0x73, 0x64,
        0xf1, 0xc4, 0x91, 0x77, 0xb0, 0xbC, 0xa5, 0x88, 0xb7, 0xa3, 0x8E, 0x76, 0x3C, 0x99, 0x6B, 0xa2,
        0x37, 0xfB, 0x2B, 0xbA, 0xa0, 0x03, 0x29, 0xaF, 0xc7, 0xd2, 0xe1, 0xf4, 0x00, 0x32, 0xeF, 0x59,
        0x6F, 0x1D, 0xdD, 0xe6, 0x7A, 0x63, 0x6D, 0x17, 0x4A, 0x6E, 0xaA, 0xf8, 0xd6, 0x40, 0x30, 0x0F,
        0x81, 0x61, 0xeD, 0x48, 0xc8, 0x19, 0x58, 0xa9, 0x74, 0xd8, 0x3E, 0x39, 0xeA, 0x9A, 0x0D, 0x9C,
        0x9F, 0xc0, 0x2A, 0x21, 0xfF, 0x43, 0xfD, 0xc3, 0x36, 0xf5, 0xbD, 0x68, 0xb9, 0x41, 0x25, 0x44,
        0x2F, 0xd7, 0xc1, 0x5D, 0xdA, 0xcC, 0x4F, 0xa1, 0x4B, 0xd0, 0x9D, 0xb4, 0x38, 0x96, 0xe2, 0x8F,
        0xf7, 0xf3, 0xdC, 0x1B, 0xaB, 0x3F, 0x46, 0xe9, 0x7F, 0x31, 0xf2, 0x5A, 0x42, 0x1C, 0x93, 0x15,
        0x22, 0xdE, 0x97, 0x8A, 0x08, 0xaD, 0xe3, 0x3D, 0x18, 0xc9, 0xbE, 0xf0, 0x4E, 0xcE, 0x78, 0x7E,
        0x6C, 0x56, 0x79, 0x1F, 0x5E, 0x67, 0xd4, 0x65, 0xb5, 0x66, 0x7D, 0x2C, 0x86, 0xbF, 0x33, 0xd9},

                  aucKeysTable2[256] = {
        0x08, 0xb3, 0x5B, 0xe9, 0x72, 0x2C, 0x1E, 0xbE, 0x0D, 0x83, 0x4F, 0x21, 0xc7, 0x0C, 0x88, 0x8D,
        0x14, 0x65, 0x7C, 0x7A, 0x86, 0x3A, 0x99, 0xbD, 0xbB, 0xb6, 0x79, 0x28, 0x9C, 0x27, 0xe7, 0xa8,
        0x00, 0x50, 0x9E, 0x58, 0x18, 0xb4, 0xcA, 0x8A, 0x24, 0x3C, 0x8F, 0xe3, 0x66, 0x71, 0x25, 0x6D,
        0xeE, 0x73, 0xc2, 0x96, 0x8B, 0xc8, 0xeD, 0xaF, 0x39, 0xa0, 0x06, 0xaD, 0xaC, 0xd2, 0x37, 0xc1,
        0xfB, 0x4B, 0xf0, 0xaA, 0xdD, 0x47, 0x17, 0x05, 0x0F, 0x3F, 0x51, 0x2E, 0x7D, 0xaB, 0x2D, 0x54,
        0x10, 0xb7, 0x43, 0x64, 0xa2, 0x6A, 0x0B, 0x49, 0xd8, 0x9A, 0xd6, 0xf5, 0x55, 0x33, 0x6F, 0x1D,
        0x0E, 0xcB, 0x2B, 0x41, 0x70, 0xd1, 0x76, 0x59, 0x29, 0xeC, 0x77, 0xf4, 0xa1, 0xb2, 0x87, 0xf3,
        0xc5, 0x74, 0xe5, 0xe1, 0x80, 0xfA, 0x44, 0x36, 0x94, 0xf8, 0x93, 0x48, 0xeF, 0xfD, 0x1C, 0x32,
        0x53, 0xf2, 0x46, 0x13, 0x68, 0x97, 0x04, 0x92, 0x4D, 0xc6, 0xa4, 0x95, 0xd5, 0x56, 0xe6, 0x5E,
        0xd7, 0xe0, 0xfF, 0x84, 0x82, 0xeA, 0xf1, 0xe2, 0x0A, 0x3D, 0x11, 0xcD, 0x38, 0xd0, 0x5F, 0x20,
        0xdB, 0x98, 0x1A, 0xeB, 0x6B, 0x9B, 0x42, 0x5A, 0xcF, 0xc0, 0xc4, 0xbC, 0x02, 0x4C, 0x03, 0x26,
        0x52, 0x2A, 0xc3, 0xb9, 0xa7, 0xfE, 0x61, 0x12, 0x4E, 0x15, 0x91, 0x3E, 0xa5, 0x67, 0x34, 0xc9,
        0xcE, 0xf6, 0xbF, 0xe4, 0x1F, 0x60, 0x6C, 0xb1, 0x35, 0xa9, 0x45, 0xcC, 0x30, 0x40, 0xa3, 0xfC,
        0x9F, 0xd4, 0x16, 0xaE, 0xa6, 0xd3, 0x7E, 0x01, 0x63, 0xe8, 0x81, 0xdC, 0xdE, 0x75, 0x09, 0x9D,
        0x78, 0x57, 0x19, 0x07, 0x6E, 0x4A, 0x22, 0x23, 0x69, 0xb5, 0x7F, 0xd9, 0x62, 0x31, 0x2F, 0x5D,
        0xb0, 0x85, 0x89, 0x90, 0xf7, 0x7B, 0x8E, 0x1B, 0x3B, 0xbA, 0xdF, 0xdA, 0x8C, 0x5C, 0xb8, 0xf9},

                  aucKeysTable3[256] = {
        0xdB, 0x97, 0xd6, 0x12, 0x4A, 0x2A, 0xcC, 0x84, 0xa0, 0x89, 0x19, 0x44, 0x28, 0x62, 0x93, 0xc9,
        0xb7, 0xbF, 0xaC, 0x41, 0x73, 0x8F, 0x0A, 0x71, 0xd2, 0x9A, 0x56, 0x4E, 0xa3, 0x81, 0xf4, 0x40,
        0x5A, 0x79, 0xbA, 0x99, 0xaB, 0x53, 0x3B, 0x17, 0x09, 0x8C, 0xd3, 0x35, 0x9B, 0x8D, 0x92, 0x68,
        0xc5, 0x1E, 0x05, 0xfE, 0x0B, 0x37, 0x31, 0xb1, 0xd0, 0xb0, 0x21, 0xe7, 0x24, 0xfC, 0x95, 0xcD,
        0x5E, 0x5D, 0xb3, 0x33, 0x39, 0xb4, 0x48, 0x88, 0x26, 0x1A, 0x0F, 0x34, 0x63, 0xe2, 0xcE, 0x5C,
        0xaF, 0x3D, 0x25, 0xc2, 0x9E, 0x65, 0x6B, 0x06, 0x77, 0x11, 0x96, 0x69, 0xa7, 0xb6, 0x67, 0xb2,
        0x6E, 0x74, 0xfD, 0x7C, 0x29, 0xf9, 0x58, 0xd4, 0xbB, 0xdA, 0xc3, 0x98, 0x8E, 0x9F, 0x7E, 0x52,
        0xf5, 0x94, 0x60, 0xfA, 0xc0, 0x0C, 0x10, 0xa5, 0xd8, 0xaE, 0x9D, 0xa4, 0x49, 0x15, 0x13, 0x4F,
        0x4C, 0x2C, 0x61, 0x01, 0x0D, 0xaA, 0x6F, 0x5B, 0x50, 0x04, 0x59, 0x07, 0xd9, 0x66, 0x1B, 0xe3,
        0xcF, 0xb5, 0x1D, 0x2D, 0x7A, 0x32, 0xbD, 0xcA, 0x6A, 0x70, 0x22, 0x1F, 0xdD, 0xf7, 0xa1, 0x1C,
        0xcB, 0x2F, 0xe0, 0xc4, 0xfB, 0x5F, 0xe9, 0x3F, 0x2B, 0x57, 0x43, 0x87, 0x4D, 0xc7, 0x91, 0x30,
        0x47, 0x54, 0x8A, 0xa6, 0x3A, 0xf0, 0xeF, 0x14, 0xeC, 0x6D, 0xeB, 0x20, 0x90, 0x42, 0xf1, 0x45,
        0x9C, 0xeA, 0xc1, 0xd1, 0x3C, 0x08, 0xdF, 0x16, 0x82, 0x0E, 0xe8, 0x2E, 0xb8, 0x18, 0x7D, 0xb9,
        0x8B, 0xa2, 0x46, 0x76, 0xe6, 0xeD, 0xe4, 0x27, 0xf3, 0xf6, 0x6C, 0x00, 0x36, 0x78, 0x64, 0x85,
        0xdE, 0xf8, 0xe5, 0xa9, 0x80, 0xa8, 0x7F, 0x72, 0xd5, 0x86, 0x02, 0xe1, 0x3E, 0xbC, 0x75, 0x4B,
        0x38, 0x03, 0x23, 0x51, 0xc8, 0xd7, 0x7B, 0xeE, 0xf2, 0xdC, 0xaD, 0xfF, 0x55, 0xc6, 0xbE, 0x83},

                  aucKeysTable4[256] = {
        0xbA, 0x9A, 0xeC, 0x4C, 0x4E, 0xe6, 0x0C, 0xdA, 0x85, 0x93, 0x52, 0xb4, 0x0A, 0x04, 0xaF, 0xb0,
        0xc0, 0x35, 0x53, 0x6E, 0x07, 0x98, 0x90, 0x5F, 0xa0, 0xaC, 0x2B, 0x38, 0x51, 0x9F, 0x6A, 0x41,
        0xa7, 0x7F, 0x73, 0x31, 0x86, 0xb8, 0x42, 0x2C, 0xcC, 0xf3, 0xf5, 0xcD, 0xcE, 0x96, 0x65, 0x48,
        0x4F, 0x5B, 0xdD, 0x7A, 0xb1, 0x8A, 0x9C, 0xa3, 0xd4, 0x81, 0x9D, 0xe9, 0xdB, 0x6B, 0x3E, 0xeB,
        0x5D, 0xe8, 0xaD, 0x1B, 0x58, 0xb5, 0x4D, 0x3C, 0x30, 0x47, 0x1D, 0xcA, 0x06, 0x97, 0x24, 0xc6,
        0x94, 0x10, 0xd7, 0xe4, 0x34, 0x8B, 0x0E, 0x03, 0x57, 0x43, 0xc1, 0xaA, 0xa1, 0x67, 0x61, 0xfE,
        0xdF, 0x59, 0xfC, 0x1F, 0x18, 0x9E, 0xd0, 0xa9, 0x72, 0x33, 0xb6, 0xbC, 0x0D, 0xbE, 0x89, 0x87,
        0x1A, 0x44, 0x6C, 0x4A, 0x77, 0x60, 0x92, 0xd3, 0xfD, 0x19, 0x09, 0xd8, 0xdC, 0x79, 0xaB, 0x84,
        0xf4, 0x11, 0xaE, 0xb9, 0xdE, 0x27, 0x82, 0xd1, 0x1E, 0x7C, 0xe7, 0xd6, 0x78, 0xe3, 0xd2, 0xc5,
        0xc8, 0x2F, 0xc2, 0x95, 0x20, 0xb2, 0x15, 0x2D, 0xd5, 0x83, 0x25, 0xbD, 0x7D, 0x12, 0x13, 0xfB,
        0xfA, 0xa4, 0x55, 0x36, 0x3A, 0xc3, 0x70, 0x80, 0x6F, 0xd9, 0x40, 0xf7, 0x23, 0x4B, 0xeD, 0xc7,
        0xcB, 0x14, 0x05, 0xa5, 0xe2, 0x69, 0xe1, 0xf8, 0x6D, 0xb3, 0x99, 0xeF, 0x5E, 0x63, 0xf6, 0x2A,
        0xeE, 0x8D, 0xcF, 0x02, 0x71, 0xf0, 0x3D, 0x7E, 0x00, 0x91, 0x76, 0xf9, 0x1C, 0x45, 0x8F, 0x32,
        0x9B, 0xa8, 0x17, 0x50, 0x64, 0x5A, 0x46, 0x26, 0x88, 0xa6, 0xbB, 0x2E, 0xeA, 0xbF, 0x75, 0xa2,
        0xc4, 0xe5, 0x8E, 0x28, 0x21, 0x54, 0x7B, 0x37, 0xfF, 0xf2, 0xb7, 0x16, 0x39, 0x3B, 0x08, 0x66,
        0x56, 0x49, 0x8C, 0x0F, 0xe0, 0x5C, 0x62, 0xc9, 0x74, 0x22, 0xf1, 0x29, 0x3F, 0x0B, 0x01, 0x68},

                  aucKeysTable5[256] = {
        0xa3, 0x92, 0x27, 0x65, 0x4A, 0x5A, 0x93, 0xf7, 0x5F, 0x49, 0x35, 0x40, 0xa6, 0x50, 0x18, 0xc5,
        0xa1, 0x09, 0x36, 0x85, 0x30, 0xdF, 0x51, 0x6F, 0xe1, 0x57, 0x3D, 0xbD, 0xf2, 0xe7, 0x02, 0x4E,
        0x1D, 0xcA, 0xe3, 0x3F, 0xcB, 0x3C, 0xa9, 0x7C, 0xdA, 0x82, 0x83, 0x16, 0x2B, 0x32, 0x47, 0xd3,
        0x1A, 0x52, 0x55, 0x9E, 0xa5, 0x08, 0x7F, 0x6C, 0x8F, 0x98, 0x71, 0xc2, 0x8C, 0xe8, 0x15, 0xfD,
        0x4C, 0x8B, 0xb7, 0x03, 0xa2, 0x43, 0x9C, 0x69, 0x63, 0x59, 0x61, 0x89, 0x87, 0x3A, 0x8D, 0xaB,
        0xbB, 0x7B, 0xc3, 0x28, 0xc7, 0xe5, 0x96, 0xe0, 0xfF, 0x58, 0xeC, 0xcD, 0xa7, 0x19, 0x9F, 0xb5,
        0xd5, 0x6D, 0x06, 0x7E, 0x86, 0x99, 0xa8, 0xf5, 0xc6, 0x53, 0x62, 0x80, 0xdD, 0xaF, 0xe4, 0xbC,
        0x91, 0xd7, 0x9D, 0x2E, 0x29, 0x00, 0x0F, 0x34, 0xd8, 0x39, 0xfA, 0xbE, 0x23, 0x9B, 0x73, 0x97,
        0xc1, 0xd2, 0xaD, 0x12, 0x60, 0x38, 0xaC, 0xe2, 0xcC, 0xd4, 0x22, 0x14, 0xc9, 0x25, 0xfE, 0x3E,
        0x78, 0x13, 0xeB, 0xc4, 0x6B, 0x84, 0x1F, 0x70, 0xc8, 0x44, 0x45, 0x46, 0x6E, 0x1B, 0xc0, 0x88,
        0xe6, 0xcE, 0x2F, 0x5B, 0xb8, 0xd1, 0x17, 0x5C, 0x2A, 0x7A, 0x1C, 0x77, 0x2D, 0x10, 0xaA, 0x72,
        0x31, 0xdC, 0x64, 0xfB, 0x4D, 0x68, 0xd9, 0x42, 0xb4, 0x04, 0xb0, 0x4F, 0x66, 0xf8, 0x54, 0x6A,
        0x26, 0x2C, 0xeD, 0x8E, 0xeE, 0xd6, 0xa4, 0x76, 0xeA, 0xe9, 0x8A, 0x94, 0xb1, 0x95, 0xdB, 0xb2,
        0x0A, 0xcF, 0x7D, 0x0D, 0x90, 0xb3, 0xb6, 0x0C, 0x5E, 0xaE, 0x4B, 0x33, 0x56, 0xbF, 0x3B, 0x79,
        0xbA, 0xfC, 0x75, 0xeF, 0xf4, 0xf3, 0x21, 0x5D, 0x9A, 0x11, 0x01, 0xf9, 0xf0, 0xf1, 0xd0, 0x81,
        0x1E, 0xf6, 0x67, 0x48, 0xb9, 0x0E, 0xdE, 0x41, 0x37, 0x20, 0x24, 0x07, 0x0B, 0xa0, 0x05, 0x74},

                  aucKeysTable6[256] = {
        0x27, 0x74, 0x05, 0x2B, 0x70, 0x06, 0xf1, 0xeF, 0x6A, 0x02, 0x88, 0xdE, 0xe4, 0xaE, 0xb9, 0x86,
        0x15, 0x8E, 0x42, 0x9A, 0xa7, 0xd3, 0x08, 0x2D, 0xb3, 0x5D, 0x9C, 0xbB, 0x85, 0x3C, 0xeA, 0x2F,
        0x41, 0x4A, 0x84, 0x38, 0x6E, 0x8A, 0x81, 0x40, 0x5C, 0xb0, 0xa8, 0x58, 0x4F, 0x80, 0x17, 0x8D,
        0xcA, 0x0F, 0xfA, 0x3F, 0x44, 0x47, 0x39, 0xe3, 0x25, 0x5A, 0x61, 0xb6, 0xdC, 0xfC, 0xdA, 0x66,
        0x04, 0x3A, 0x37, 0x28, 0xc6, 0xbC, 0x36, 0x99, 0xcB, 0x97, 0xe0, 0x65, 0x68, 0x6D, 0x56, 0xa5,
        0x7D, 0x1F, 0x48, 0xe5, 0x5E, 0x01, 0x0D, 0xbD, 0xf4, 0x46, 0xbE, 0xf7, 0x52, 0x62, 0xbA, 0x8B,
        0x93, 0xd7, 0x35, 0x3D, 0xc0, 0xa0, 0xa9, 0x75, 0x07, 0x16, 0xc5, 0x95, 0xe7, 0xf6, 0x1B, 0x82,
        0x4C, 0xa3, 0xb1, 0x32, 0x72, 0xf9, 0x00, 0x77, 0x13, 0xc1, 0x79, 0x03, 0x8C, 0xb2, 0x94, 0xdB,
        0x91, 0xf8, 0xeB, 0x21, 0xbF, 0xb4, 0xcD, 0x89, 0x34, 0xdF, 0xaD, 0x10, 0x2A, 0x6C, 0xd4, 0xb8,
        0xe1, 0x4D, 0x0B, 0x26, 0x0C, 0x96, 0xc7, 0x12, 0x1D, 0x6F, 0xaF, 0x76, 0x1E, 0x87, 0x59, 0x0A,
        0xaC, 0x64, 0x45, 0x09, 0x60, 0x4B, 0xaB, 0xf5, 0x7C, 0x22, 0xa6, 0x71, 0x9B, 0x92, 0x57, 0x18,
        0x2E, 0x4E, 0x30, 0xc9, 0xd8, 0xc8, 0x2C, 0xa2, 0xe9, 0xcF, 0xc2, 0x3B, 0xe2, 0x33, 0x0E, 0xc4,
        0x19, 0xb7, 0x1C, 0xc3, 0x6B, 0xfD, 0x69, 0x9E, 0x54, 0x53, 0x73, 0x49, 0x9D, 0x7B, 0xeC, 0x31,
        0xd1, 0x3E, 0xfB, 0xcC, 0x7A, 0x55, 0x51, 0x9F, 0xf0, 0xd0, 0x67, 0xe6, 0xeD, 0xd6, 0x50, 0x14,
        0xdD, 0x7F, 0x23, 0xf2, 0xa4, 0xe8, 0x83, 0xeE, 0xb5, 0xd2, 0x8F, 0xd5, 0x11, 0xaA, 0xfE, 0x29,
        0xf3, 0x98, 0x63, 0xcE, 0x90, 0x7E, 0x5B, 0x78, 0x1A, 0xa1, 0x43, 0xd9, 0x24, 0xfF, 0x20, 0x5F},

                  aucKeysTable7[256] = {
        0x34, 0x3D, 0xcF, 0x6A, 0x38, 0x74, 0x0E, 0xc1, 0x94, 0x42, 0x48, 0xc9, 0x21, 0x0A, 0x3C, 0x12,
        0x36, 0xeD, 0x29, 0x06, 0x47, 0x30, 0x4C, 0x7D, 0x24, 0x6B, 0xaE, 0xbD, 0xd6, 0x49, 0x4E, 0x33,
        0xbB, 0x73, 0x65, 0xa8, 0x0C, 0x05, 0x91, 0x61, 0x5F, 0x1E, 0xe2, 0xfF, 0x2C, 0xeC, 0xb4, 0xb3,
        0xb6, 0x9A, 0x6E, 0x8B, 0x62, 0x57, 0x70, 0xe5, 0xfE, 0x1B, 0xc5, 0xb9, 0x8C, 0xbC, 0xdC, 0x39,
        0x25, 0x45, 0x40, 0x89, 0xa6, 0xdA, 0x5E, 0x02, 0x04, 0x00, 0xfB, 0x5A, 0xb7, 0x60, 0x5B, 0x80,
        0x8E, 0x90, 0x59, 0x75, 0xaA, 0xdB, 0xa2, 0x69, 0x6D, 0x93, 0x31, 0xd7, 0x3A, 0xd2, 0x41, 0x03,
        0xfD, 0x2D, 0xa0, 0xeF, 0x27, 0x10, 0x0F, 0x17, 0x1C, 0xc4, 0xf2, 0x26, 0xe1, 0x43, 0xaB, 0xa4,
        0xd9, 0xf4, 0x77, 0x6F, 0xfA, 0x5C, 0x35, 0x44, 0xaC, 0xc2, 0xd4, 0xeA, 0x67, 0xf3, 0xeE, 0x8F,
        0xd3, 0x1D, 0x4A, 0xcE, 0x46, 0xa7, 0x58, 0xcD, 0x2A, 0x81, 0x37, 0x3F, 0xd1, 0x78, 0x87, 0x85,
        0xb5, 0x76, 0x82, 0x66, 0xdF, 0x0B, 0xc7, 0xaF, 0xe4, 0xb1, 0xe8, 0x4B, 0xbE, 0x14, 0x55, 0x3B,
        0x4D, 0xe6, 0x7F, 0x15, 0xf6, 0x01, 0x88, 0x19, 0x4F, 0x99, 0x16, 0xf7, 0x3E, 0x07, 0x68, 0x79,
        0x63, 0x52, 0xc0, 0xc6, 0x2F, 0x09, 0x9D, 0xeB, 0xcC, 0x98, 0x83, 0x7B, 0x7C, 0x8D, 0xd8, 0x2E,
        0x9E, 0xbF, 0x7A, 0x95, 0x18, 0x28, 0x13, 0xdE, 0xa9, 0xd5, 0xfC, 0xf5, 0xb0, 0xcA, 0xbA, 0x22,
        0x92, 0xa5, 0xe7, 0xf1, 0xe9, 0xe3, 0x32, 0xcB, 0x23, 0x11, 0x50, 0x7E, 0x2B, 0xf0, 0xf9, 0x51,
        0x56, 0x71, 0xd0, 0xe0, 0x54, 0xb2, 0x72, 0x08, 0x1A, 0xaD, 0x9F, 0xc3, 0x20, 0x97, 0x96, 0x1F,
        0x0D, 0xdD, 0x84, 0x8A, 0xb8, 0xa1, 0xc8, 0x9C, 0xf8, 0x9B, 0x5D, 0x86, 0x64, 0x53, 0x6C, 0xa3},

                  aucKeysTable8[256] = {
        0x40, 0x2A, 0x83, 0x59, 0x12, 0xd7, 0x1C, 0x6F, 0x28, 0x72, 0x81, 0xdF, 0x36, 0x67, 0x52, 0x48,
        0x78, 0xf8, 0xb1, 0x9C, 0xbF, 0x20, 0x6A, 0x3E, 0xb0, 0x3B, 0x80, 0x7B, 0xfE, 0x68, 0x4D, 0x95,
        0xc6, 0x4B, 0xd3, 0xaA, 0x11, 0xa0, 0x85, 0xf7, 0x7F, 0x74, 0x08, 0xa7, 0x9D, 0xdE, 0x7D, 0xeD,
        0x43, 0x19, 0x8A, 0x30, 0x45, 0x1A, 0xbD, 0x3D, 0x14, 0x50, 0x91, 0x5B, 0xbB, 0x21, 0x5C, 0xb9,
        0x13, 0x99, 0xf6, 0x3F, 0xf4, 0xc7, 0x16, 0xf0, 0x4F, 0xeA, 0xa1, 0xf5, 0x35, 0x90, 0x66, 0xa8,
        0x51, 0x41, 0xe9, 0xe6, 0xc1, 0x49, 0x62, 0x6C, 0x05, 0x61, 0x86, 0xb3, 0x2D, 0xa2, 0x64, 0x89,
        0x0B, 0xa6, 0x98, 0x1E, 0x4E, 0x2B, 0xa5, 0x70, 0xb2, 0xdC, 0x4A, 0xbA, 0xcC, 0x1B, 0x5F, 0xf1,
        0x73, 0x8C, 0xb8, 0x0E, 0xd8, 0xe4, 0x84, 0x63, 0x6B, 0xdD, 0x06, 0x77, 0xfD, 0xeB, 0xfC, 0xcD,
        0x6E, 0x96, 0xbE, 0x76, 0x5D, 0x2F, 0x55, 0x02, 0xa3, 0x9E, 0x3A, 0x33, 0x8E, 0x8D, 0x2E, 0x75,
        0xd9, 0x65, 0x53, 0xc9, 0xf2, 0x34, 0x44, 0x37, 0x0A, 0xb6, 0x47, 0x92, 0x18, 0xc8, 0xeC, 0x7A,
        0x24, 0x25, 0xf9, 0x0D, 0x42, 0xd4, 0x00, 0xa9, 0xe1, 0x1D, 0xcE, 0x9B, 0x03, 0x79, 0xe5, 0x8B,
        0x9F, 0x46, 0x27, 0xaF, 0x82, 0xe8, 0x57, 0x0F, 0x1F, 0xcA, 0xc0, 0xb5, 0xb7, 0xaE, 0x23, 0x09,
        0x22, 0x07, 0x26, 0xe3, 0xfF, 0x5E, 0x31, 0xd2, 0xe2, 0xcB, 0x9A, 0x69, 0xdA, 0xeF, 0x93, 0x71,
        0xe0, 0x3C, 0x04, 0xc4, 0xc3, 0x29, 0x60, 0x97, 0xa4, 0x56, 0xaC, 0x4C, 0x15, 0x94, 0x87, 0x17,
        0x5A, 0xeE, 0xc2, 0x8F, 0xfA, 0x7E, 0x39, 0x54, 0x0C, 0xfB, 0x38, 0xd6, 0x7C, 0x01, 0x10, 0xf3,
        0xe7, 0xcF, 0xd1, 0xbC, 0xc5, 0xd5, 0x6D, 0x2C, 0xb4, 0xaB, 0xdB, 0xd0, 0x58, 0xaD, 0x32, 0x88};

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

// There are 8 sets of passwords, each with at least 8 ciphers and no more than 255 ciphers.
    unsigned char aucPassword[8][8];

// 95+1 printable characters pad
    unsigned char aucKeysTable[96];

// this character is arbitrary
    aucKeysTable[95] = 36;

// initialize random number seed
    srand(ulFileSize % RAND_MAX);

// the pad is initialized with a pseudo-random number algorithm
    for(unsigned long long i = 0; i < 95; ++i)
    {
LOOP:
       aucKeysTable[i] = 32 + rand() % 95;

       for(unsigned long long j = 0; j < i; ++j)
       {
            if(aucKeysTable[j] == aucKeysTable[i]) goto LOOP;
        }
    }

    for(unsigned long long i = 0; i < 8; ++i)
    {
// generate password
        for(unsigned long long j = 0; j < 8; ++j) aucPassword[i][j] = aucKeysTable[argv[3][j] % 96];
    }

// process plaintext / ciphertext data
    for(unsigned long i = 0; i < ulFileSize;)
    {
        i += EncryptOrDecrypt(aucPassword[0], aucKeysTable1, i);

        if(i >= ulFileSize) break;

        i += EncryptOrDecrypt(aucPassword[1], aucKeysTable2, i);

        if(i >= ulFileSize) break;

        i += EncryptOrDecrypt(aucPassword[2], aucKeysTable3, i);

        if(i >= ulFileSize) break;

        i += EncryptOrDecrypt(aucPassword[3], aucKeysTable4, i);

        if(i >= ulFileSize) break;

        i += EncryptOrDecrypt(aucPassword[4], aucKeysTable5, i);

        if(i >= ulFileSize) break;

        i += EncryptOrDecrypt(aucPassword[5], aucKeysTable6, i);

        if(i >= ulFileSize) break;

        i += EncryptOrDecrypt(aucPassword[6], aucKeysTable7, i);

        if(i >= ulFileSize) break;

        i += EncryptOrDecrypt(aucPassword[7], aucKeysTable8, i);
    }

// open ciphertext / plaintext file
    iPlaintextOrCiphertext = open(argv[2], O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

// write data to ciphertext / plaintext file
    write(iPlaintextOrCiphertext, pucPlaintextOrCiphertext, ulFileSize);

    close(iPlaintextOrCiphertext);

    free(pucPlaintextOrCiphertext);

    return 0;
}
