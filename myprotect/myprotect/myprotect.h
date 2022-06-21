#pragma once


void my_junk_code_begin1(void(*func1)() ,void(*func2)());
void my_junk_code_end1();


extern void (*junk_code_begin[101])();
/*
 *void my_junk_code_begin1(void(*func1)() ,void(*func2)())
 *
 */



extern void (*junk_code_end[101])();


void initialize();