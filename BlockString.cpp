#include "BlockString.h"

/*
=====================================================
        helper functions
=====================================================
*/

// size in bytes
void Copy_Memory(char* dest, char* source, u32 size)
{
    for(u32 i = 0; i < size; i++)
    {
        dest[i] = source[i];
    }
}

/*
=====================================================
        dev functions
=====================================================
*/
#ifdef DEV
//TODO: it stops outputting string when it met 0 symbol, which is tecnically
//correct, but not what we want, same in operator<<
void BlockString::Debug_Print()
{
    char output_string[1024];
    u32 symbols_to_copy;
    Block* current_block = initial_block;
    while(current_block != nullptr)
    {
        cout << termcolor::yellow;
        cout << "|" << (u32)current_block->start << "|" << (u32)current_block->end << "|";

        cout << termcolor::red;
        symbols_to_copy = current_block->start;
        Copy_Memory(output_string, current_block->symbols, symbols_to_copy);
        output_string[symbols_to_copy] = 0;
        cout << output_string;

        cout << termcolor::green;
        symbols_to_copy = current_block->end - current_block->start;
        Copy_Memory(output_string, current_block->symbols + current_block->start, symbols_to_copy);
        output_string[symbols_to_copy] = 0;
        cout << output_string;

        cout << termcolor::red;
        symbols_to_copy = SYMBOLS_BLOCK_CAPACITY - current_block->end;
        Copy_Memory(output_string, current_block->symbols + current_block->end, symbols_to_copy);
        output_string[symbols_to_copy] = 0;
        cout << output_string;

        cout << termcolor::reset;
        cout << "-->";
        current_block = current_block->next_block;
    }
    cout << "X";
}
#endif


/*
=====================================================
        functions for char*
=====================================================
*/

// OPTIMIZE
u32 BlockString::StrLength(char* str)
{
    u32 result = 0;
    char* p = str;
    while(*p)
    {
        p++;
        result++;
    }
    return result;
}

/*
=====================================================
        functions for Block type
=====================================================
*/
u32 BlockString::Block::Length()
{
    return end - start;
}

u32 BlockString::Block::Is_Full()
{
    return Length() == SYMBOLS_BLOCK_CAPACITY;
}

u32 BlockString::Block::Can_Add_To_End()
{
    return SYMBOLS_BLOCK_CAPACITY - end;
}
/*
=====================================================
        functions for BlockString type
=====================================================
*/
BlockString::BlockString()
{
}

BlockString::BlockString(BlockString& str)
{
#ifdef DEV
    assert(initial_block == nullptr);
    assert(final_block == nullptr);
#endif
    // Dunno, maybe I don't need this
    Block** p_current_block = &initial_block;
    Block* cursor = str.initial_block;
    while(cursor != nullptr)
    {
        (*p_current_block) = new Block;
        (*p_current_block)->start = cursor->start;
        (*p_current_block)->end = cursor->end;
        Copy_Memory((*p_current_block)->symbols, cursor->symbols, SYMBOLS_BLOCK_CAPACITY);
        final_block = *p_current_block;
        p_current_block = &((*p_current_block)->next_block);
        cursor = cursor->next_block;
    }
}

BlockString::BlockString(char* str)
{
    *this = *this + str;
}

BlockString::~BlockString()
{
    Block* tmp;
    while(initial_block != nullptr)
    {
        tmp = initial_block;
        initial_block = initial_block->next_block;
        delete tmp;
    }
}

// TODO: now I just iterate over all blocks, later I will remember
// length in member field
u32 BlockString::Length()
{
    u32 result = 0;
    Block* current_block = initial_block;
    while(current_block != nullptr)
    {
        result += current_block->Length();
        current_block = current_block->next_block;
    }
    return result;
    // return length;
}

//TODO: it stops outputting string when it met 0 symbol, which is tecnically
//correct, but not we want, same in DEBUG_PRINT
ostream& operator<< (ostream& os, BlockString& str)
{
    //TODO: think about outputting
    char buffer[1024];
    u32 index = 0;
    BlockString::Block* current_block = str.initial_block;
    while(current_block != nullptr)
    {
        u32 block_length = current_block->Length();
        Copy_Memory(buffer + index, current_block->symbols + current_block->start, block_length);
        index += block_length;
        current_block = current_block->next_block;
    }
    buffer[index] = 0;
    os << buffer;
    return os;
}

char& BlockString::operator[] (u32 index)
{
    Block* current_block = initial_block;
    u32 symbols_passed = 0;
    while(current_block != nullptr)
    {
        u32 block_length = current_block->Length();
        // this means our index in current_block
        if(block_length > index)
        {
            char& result = (current_block->symbols + current_block->start)[index];
            return result;
        }
        else
        {
            index -= block_length;
            current_block = current_block->next_block;
        }
    }
    throw "Index out of bounce";
}

BlockString& BlockString::operator= (BlockString& str)
{
    // TODO: think more carefully about this equality check
    // what if we have smt like str1 = char_string + str1? If we have no
    // empty elements in initial_block from left, then our initial block will change,
    // but what if we had space and we add symbols in that block? then our pointer is the same,
    // but strings are different
    // Added: Don't know, string might change in unpredictable way before assigning (added
    // symobol th end/start, change substr)

    /*
    if(this->initial_block == str.initial_block)
    {
        return *this;
    }
    */

    //TODO: test if this idea works (creating local object by ref and assign it to our
    // object, dont forget to change pointer in local object!)
    BlockString local(str); //copy constructor

    //Free memory from initial string
    Block* tmp;
    while(initial_block != nullptr)
    {
        tmp = initial_block;
        initial_block = initial_block->next_block;
        delete tmp;
    }

    initial_block = local.initial_block;
    final_block = local.final_block;

    local.initial_block = local.final_block = nullptr;
    // Now destructor for local object invokes, but it does nothing

    return *this;
}

BlockString BlockString::operator+ (char* right)
{
    u32 right_str_len = BlockString::StrLength(right);
    if(right_str_len == 0)
    {
        return *this;
    }

    BlockString local(*this);
    if(local.initial_block == nullptr && local.final_block == nullptr)
    {
        u32 number_of_full_blocks = (right_str_len / SYMBOLS_BLOCK_CAPACITY);
        u32 number_of_last_symbol = right_str_len - number_of_full_blocks*SYMBOLS_BLOCK_CAPACITY;

        // local.initial_block = new char[local.block_capacity + 2 + ]
        local.initial_block = new Block;
        Block* current_block = local.initial_block;
        u32 i;
        for(i = 0; i < number_of_full_blocks; i++)
        {
#ifdef DEV
            assert(current_block->start == 0);
            assert(current_block->end == 0);
#endif
            current_block->end = SYMBOLS_BLOCK_CAPACITY;
            Copy_Memory(current_block->symbols, right + i*SYMBOLS_BLOCK_CAPACITY, SYMBOLS_BLOCK_CAPACITY);
            current_block->next_block = new Block;
            current_block = current_block->next_block;
        }
        for(u32 index = 0; index < number_of_last_symbol; index++)
        {
            current_block->symbols[index] = right[i*SYMBOLS_BLOCK_CAPACITY + index];
            (current_block->end)++;
        }
        local.final_block = current_block;
        return local;
    }
    else
    {
        u32 can_add = local.final_block->Can_Add_To_End();
        if(can_add >= right_str_len)
        {
            // Do not need to allocate new blocks
            Copy_Memory(local.final_block->symbols + local.final_block->end, right, right_str_len);
            local.final_block->end += right_str_len;
            // TODO: Length increment?
            return local;
        }
        else
        {
            // Need allocate new blocks, but first fill up last block
            Copy_Memory(local.final_block->symbols + local.final_block->end, right, can_add);
            local.final_block->end += can_add;
            right += can_add;
            right_str_len -= can_add;
            //TODO: same as adding to empty string
            u32 number_of_full_blocks = (right_str_len / SYMBOLS_BLOCK_CAPACITY);
            u32 number_of_last_symbol = right_str_len - number_of_full_blocks*SYMBOLS_BLOCK_CAPACITY;

            local.final_block->next_block = new Block;
            Block* current_block = local.final_block->next_block;
            u32 i;
            for(i = 0; i < number_of_full_blocks; i++)
            {
#ifdef DEV
                assert(current_block->start == 0);
#endif
                current_block->end = SYMBOLS_BLOCK_CAPACITY;
                Copy_Memory(current_block->symbols, right + i*SYMBOLS_BLOCK_CAPACITY, SYMBOLS_BLOCK_CAPACITY);
                current_block->next_block = new Block;
                current_block = current_block->next_block;
            }
            for(u32 index = 0; index < number_of_last_symbol; index++)
            {
                current_block->symbols[index] = right[i*SYMBOLS_BLOCK_CAPACITY + index];
                (current_block->end)++;
            }
            local.final_block = current_block;
            return local;
        }
    }
}

BlockString BlockString::operator+ (BlockString& right)
{
    u32 right_str_len = right.Length();
    if(right_str_len == 0)
    {
        return *this;
    }
    BlockString local(*this);

    //OPTIMIZE: definitely not the best solution, later I should change this
    //overhead: assignment and copying
    char buffer[1024];
    u32 index;
    for(index = 0; index < right_str_len; index++)
    {
        buffer[index] = right[index];
    }
    buffer[index] = 0;
    local = local + buffer;

    return local;
}

BlockString operator+ (char* left, BlockString& right)
{
    u32 right_str_len = right.Length();
    //OPTIMIZE: and again, not the best solution
    BlockString local(left);
    if(right_str_len == 0)
    {
        return local;
    }
    local = local + right;
    return local;
}

BlockString Copy(u32 from, u32 len, BlockString& str)
{
    //OPTIMIZE: yet again, not the best solution
    BlockString local;
    char buffer[1024];
    u32 index;
    u32 str_len = str.Length();
    if(from + len <= str_len)
    {
        for(index = 0; index < len; index++)
        {
            buffer[index] = str[from + index];
        }
        buffer[index] = 0;
        local = local + buffer;
        return local;
    }
    else
    {
        // TODO: throw an error?
        throw "Index out fo bounce (Copy)";
    }
}

s32 BlockString::SubStr(BlockString& substr)
{
    UNIMPLEMENTED();
    return -1;
}
