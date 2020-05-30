#pragma warning( push, 0 )
#include <stdint.h>
#include <assert.h>
#include <iostream>
#pragma warning( pop )

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

using namespace std;
#include "BlockString.cpp"

char str[] = {'H', 'e', '\0'};
char *p = "Hello world! Hello Hello Hello Hello Hello Hell";
char *hello = "Hello";
char *world = " World";
char *a = "Dsad";

void Test_Assignment()
{
    BlockString d(p);
    d = d;
    d.initial_block->symbols[0] ='s';
}

void Test_Output()
{
    BlockString s;
    s = s + p;
    cout << s << endl;
    BlockString d(s);
    cout << d << endl;
    d = d + a;
    cout << d << endl;
    d = d + a;
    cout << d << endl;
    d = d + a;
    cout << d << endl;
    BlockString test(a);
    cout << test << endl;
    d.Debug_Print();
    cout << endl;
    d.initial_block->symbols[0] ='s';
    d.initial_block->start = 1;
    d.Debug_Print();
    cout << endl;
    cout << d << endl;
    d.initial_block->end = 9;
    d.Debug_Print();
    cout << endl;
    cout << d << endl;
}

void Test_Bracket_Operator()
{
    BlockString d(p);
    cout << d << endl;
    cout << d[0] << endl;

    d[46] = 'N';
    try
    {
        d[47] = 'N';
    }
    catch(const char *e)
    {
        cout << e << endl;
    }

    //TODO: now it stops outputting string cuz of this zero character
    /*
    d[2] = 0;
    cout << d[2] << endl;
    cout << d << endl;
    d.Debug_Print();
    */
    BlockString empty1;
    try
    {
        empty1[0] = '1';
    }
    catch(const char *e)
    {
        cout << e << endl;
    }
}

void Test_Length()
{
    BlockString d(p);
    assert(d.Length() == 47);
    BlockString empty1;
    assert(empty1.Length() == 0);
    char emp_str[] = "\0";
    BlockString empty2(emp_str);
    assert(empty2.Length() == 0);
    d = d + emp_str;
    assert(d.Length() == 47);
}

void Test_Adding_Char_Str()
{
    cout << "TEST_ADDING_CHAR_STR" << endl;
    BlockString d(hello);
    d = world + d;
    assert(d.Length() == 11);
    d.Debug_Print();
    cout << endl << d << endl;
    cout << endl;
    d = world + d;
    d = world + d;
    d = world + d;
    d.Debug_Print();
    cout << endl << d << endl;
}

void Test_Adding_Str_Str()
{
    cout << "TEST_ADDING_STR_STR" << endl;
    BlockString d(hello);
    BlockString s(world);
    BlockString n(world);
    d = n + s;
    d.Debug_Print();
    cout << endl << d << endl;
    cout << endl;
    d = s + d;
    d = d + d;
    d = s + d;
    d.Debug_Print();
    cout << endl << d << endl;
}

void Test_Adding_Str_Char()
{
    cout << "TEST_ADDING_STR_CHAR" << endl;
    BlockString d(hello);
    d = d + world;
    assert(d.Length() == 11);
    d.Debug_Print();
    cout << endl << d << endl;
    cout << endl;
}

void Test_Adding()
{
    Test_Adding_Char_Str();
    Test_Adding_Str_Str();
    Test_Adding_Str_Char();
}

void Test_Copy()
{
    BlockString d(hello);
    BlockString copy;
    copy = Copy(1, 3, d);
    copy.Debug_Print();
    cout << endl << copy << endl;
    assert(copy.Length() == 3);
}

int main()
{
    cout << endl << "TEST_OUTPUT" << endl;
    Test_Output();
    cout << endl << "TEST_BRACKET_OPERATOR" << endl;
    Test_Bracket_Operator();
    cout << endl << "TEST_LENGTH" << endl;
    Test_Length();
    cout << endl << "TEST_ASSIGNMENT" << endl;
    Test_Assignment();
    cout << endl << "TEST_ADDING" << endl;
    Test_Adding();
    cout << endl << "TEST_COPY" << endl;
    Test_Copy();
    return 0;
}
