#include <stdarg.h>
#include <stdio.h>

#include <squirrel.h>
#include <sqstdio.h>
#include <sqstdaux.h>

#ifdef _MSC_VER
#pragma comment (lib ,"squirrel.lib")
#pragma comment (lib ,"sqstdlib.lib")
#endif

#ifdef SQUNICODE

#define scvprintf vfwprintf
#else

#define scvprintf vfprintf
#endif

#define VM_SIZE 1024

void printfunc(HSQUIRRELVM v,const SQChar *s,...)
{
    va_list vl;
    va_start(vl, s);
    scvprintf(stdout, s, vl);
    va_end(vl);
}

void errorfunc(HSQUIRRELVM v,const SQChar *s,...)
{
    va_list vl;
    va_start(vl, s);
    scvprintf(stderr, s, vl);
    va_end(vl);
}

void ask_move(HSQUIRRELVM v, int n, float f, const SQChar *s)
{
    SQInteger top = sq_gettop(v); //saves the stack size before the call
    sq_pushroottable(v); //pushes the global table
    sq_pushstring(v,_SC("make_move"),-1);
    if(SQ_SUCCEEDED(sq_get(v,-2))) { //gets the field 'foo' from the global table
        sq_pushroottable(v); //push the 'this' (in this case is the global table)
        sq_pushinteger(v,n);
        sq_pushfloat(v,f);
        sq_pushstring(v,s,-1);
        sq_call(v,4,SQFalse,SQTrue); //calls the function
    }
    sq_settop(v,top); //restores the original stack size
}

HSQUIRRELVM initialize()
{
    HSQUIRRELVM v;
    v = sq_open(VM_SIZE);

    sq_pushroottable(v);
    sqstd_register_mathlib(v);
    sqstd_register_stringlib(v);
    sq_pop(v, 1);

    sqstd_seterrorhandlers(v);
    sq_setprintfunc(v, printfunc,errorfunc);

    sq_pushroottable(v); //push the root table(were the globals of the script will be stored)
    SQRESULT result = sqstd_dofile(v, _SC("test.nut"), SQFalse, SQTrue);
    if (SQ_SUCCEEDED(result))
    {
        ask_move(v,1,2.5,_SC("teststring"));
    }

    sq_pop(v, 1);
    sq_close(v);

    return 0;
}

void exit(HSQUIRRELVM v)
{
    sq_close(v);
}