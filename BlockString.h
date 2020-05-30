#define SYMBOLS_BLOCK_CAPACITY 26
#define DEV
#ifdef DEV
    #define UNIMPLEMENTED() assert(0)
    #include "termcolor.hpp"
#endif
class BlockString
{
#ifdef DEV
    public:
#endif
    struct Block
    {
        u8 start = 0;
        u8 end = 0;
        char symbols[SYMBOLS_BLOCK_CAPACITY];
        Block* next_block = nullptr;

        u32 Length();
        u32 Is_Full();
        // returns number of symbols that could be added to block from right
        u32 Can_Add_To_End();
    };

    Block* initial_block = nullptr;
    Block* final_block = nullptr;
    u32 length = 0;

    public:
    BlockString();
    BlockString(BlockString& str);
    BlockString(char* str);
    //TODO: maybe this is the time to use some tool for detecting mem leak?
    ~BlockString();
    u32 Length();
    friend ostream& operator<< (ostream& os, BlockString& str);
    //NOTE: i think this should be returned by ref so we could write smt
    //like string[i] = 'd';
    char& operator[] (u32 index);
    BlockString& operator= (BlockString& str);
    BlockString operator+ (char* right);
    BlockString operator+ (BlockString& right);
    // friend cuz otherwise *this parameter added
    friend BlockString operator+ (char* left, BlockString& right);
    friend BlockString Copy(u32 from, u32 len, BlockString& str);
    // return -1, if no substr found
    s32 SubStr(BlockString& substr);

    // NOTE
    static u32 StrLength(char* str);

#ifdef DEV
    void Debug_Print();
#endif

};
