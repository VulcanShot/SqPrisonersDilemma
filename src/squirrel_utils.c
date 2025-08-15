#include <squirrel.h>

/// @brief Gets a string value from a table with stack index -3.
/// @param v The virtual machine
/// @param key The key of the table slot
/// @param value The integer value of the table slot
SQRESULT sq_push_integer_slot(HSQUIRRELVM v, const char *key, int value)
{
    sq_pushstring(v, key, -1);
    sq_pushinteger(v, value);
    return sq_newslot(v, -3, SQFalse);
}

void sq_push_integer_array(HSQUIRRELVM v, int arr[], int n)
{
    sq_newarray(v, n);
    for (int i = 0; i < n; i++)
    {
        sq_pushinteger(v, arr[i]);
        sq_arrayappend(v, -2);
    }
}

/// @brief Gets a string value from a table with stack index -2.
/// @param v The virtual machine
/// @param key The key of the table slot
/// @param value A pointer to the string where the value will be written
SQRESULT sq_get_string_slot(HSQUIRRELVM v, const char *key, const SQChar **value)
{
    sq_pushstring(v, key, -1);
    sq_get(v, -2);
    if ( SQ_FAILED(sq_getstring(v, -1, value)) )
    {
        return SQ_ERROR;
    }
    sq_pop(v, 1);
    return SQ_OK;
}