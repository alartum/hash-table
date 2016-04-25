#ifndef HASH_FUNCTIONS_H_INCLUDED
#define HASH_FUNCTIONS_H_INCLUDED

#include <string.h>
size_t hash_xor (const char source_word[])
{
    size_t hash_value = (size_t)source_word[0];

    for (int i = 1; source_word[i] != '\0'; i++)
    {
        *(char*)(&hash_value) ^= source_word[i];
        hash_value = (hash_value >> 1) | (hash_value << (8*sizeof(size_t) - 1));
    }
    //printf ("XOR [%s]=%lu\n", word_copy, hash_value);
    return hash_value;
}

// Optimised xor hash version
// source_word is stored in rdi
// mov [rbp]-0x28, rdi
__attribute__ ((noinline)) size_t hash_xor_opt (const char source_word[])
{
    size_t hash_value;

    /* May be __volatile__ if we want to provide registers ourselves*/
    __asm__  (
             ".intel_syntax noprefix;"
             "  xor %0, %0;" /* result will be stored here */
             "  xor rbx, rbx;"
             "  mov bl, [%1];" /* stores char to bl */

             "cmp bl, 0x0;" /* let's check if we are at the end */
             "je  .end;" /* exit, if so */
             "jmp .middle;" /* jump into cycle else */
             ".cycle:"
             "  xor %0, rbx;" /* no comment */
             "  ror %0, 1;" /* rotate rbx 1 byte (char size) right */
             ".middle:"
             "  inc %1;" /* move forward */
             "  mov bl, [%1];" /* store next char */
             "  cmp bl, 0x0;"  /* checking the end */
             "jne .cycle;"  /* cycling */
             ".end:" /* the compiler will pop rbp for us and then return */
                     /* the result is in rax already, don't need to check*/
             ".att_syntax prefix;"
             : "=r" (hash_value)
             : "r"  (source_word)
             : "rbx" // We'll spoil rbx
             );

    return hash_value;
}

size_t hash_const (/*const char word[]*/)
{
    //printf ("CONST=0\n");
    return 0;
}

size_t hash_char (const char word[])
{
    //printf("CHAR=%d\n", word[0]);
    return (size_t)word[0];
}

size_t hash_length (const char word[])
{
    //printf("LENGTH=%lu\n", strlen(word));
    return (size_t)strlen (word);
}

size_t hash_sum (const char word[])
{
    size_t hash_value = 0;
    for (int i =0; word[i] != '\0'; i++)
        hash_value += word[i];
    //printf ("SUM=%lu\n", hash_value);
    return hash_value;
}
#endif // HASH_FUNCTIONS_H_INCLUDED
