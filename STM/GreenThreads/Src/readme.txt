This folder contains internal implementation of mechainsm.
File 'test.cpp' provides example how to add solution to your 'C' code.
You should create separate *.cpp file. 
Inside this file define all global symbols - thread functions,
context structures. 
Your solution should be wrapped in 'C' function inside
de-manglers :
#ifdef __cplusplus
extern "C" {
#endif
    ... your function to export here ...
#ifdef __cplusplus
}
#endif
To reveal your function to main.c, declare in inside 'greenThreadsMain.h'.
User must at least check configuration in file 'greenThreadsConfig.h'.