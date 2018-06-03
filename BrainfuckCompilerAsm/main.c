#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void*
readfile_ex(
    void *dst,
    size_t dstsize,
    const char *fname,
    size_t offset,
    size_t *psize,
    int binary
    );

void*
readfile(
    const char *fname,
    size_t *psize
    );

int BFCompile(char *Script, size_t ScriptSize, char *Code, size_t *CodeSize);

int main(int argc, char *argv[])
{
  char *Script;
  char *Code;
  size_t ScriptSize;
  size_t CodeSize = 0;
  char *OutputName = "out.s";
  FILE *OutputFile;

  if (argc < 2)
  {
     puts("usage: bfc <ScriptName> [OutputName]");
     return 1;
  }

  if (argc == 3)
  {
     OutputName = argv[2];
  }

  Script = readfile_ex(NULL, 0, argv[1], 0, &ScriptSize, 0);

  if (!Script)
  {
      puts("error: could not read script file");
      return 2;
  }

  Code = malloc(ScriptSize * 20);

  if (BFCompile(Script, ScriptSize, Code, &CodeSize) != 0)
  {
      puts("error: could not compile file");
      return 3;
  }

  OutputFile = fopen(OutputName, "w");

  if (!OutputFile)
  {
      puts("error: could not create target file");
      return 3;
  }

  fwrite(Code, 1, CodeSize, OutputFile);

  fclose(OutputFile);
  free(Code);

  return 0;
}

void WriteBuffer(const char *Buffer, size_t BufferSize, char *Out, size_t *OutSize)
{
    memcpy(Out + *OutSize, Buffer, BufferSize);
    *OutSize += BufferSize;
}


int BFCompile(char *Script, size_t ScriptSize, char *Code, size_t *CodeSize)
{
    int i = 0;


    const char *code_prologue = "\t.intel_syntax noprefix\n\t.text\n\t.globl _main\n_main:\n\tpush ebp\n\tmov ebp, esp\n\tand esp, -16\n\tsub esp, 16\n";
    const char *code_move_r = "\tinc eax\n";
    const char *code_move_l = "\tdec eax\n";
    const char *code_inc = "\tinc [eax]\n";
    const char *code_dec = "\tdec [eax]\n";
    const char *code_putchar = "\t;putchar\n";
    const char *code_getchar = "\t;getchar\n";
    const char *code_epilogue = "\tmov eax, 0\n\tleave\n\tret\n.lcomm tablica,30000,32\n";

    #define WriteCode(a) WriteBuffer(a, strlen(a), Code, CodeSize);


    WriteCode(code_prologue);


    for( ; i < ScriptSize; i++)
    {
        switch(Script[i])
        {
            case '<':
                WriteCode(code_move_l);
                break;
            case '>':
                WriteCode(code_move_r);
                break;
            case '-':
                WriteCode(code_dec);
                break;
            case '+':
                WriteCode(code_inc);
                break;
            case '.':
                WriteCode(code_putchar);
                break;
            case ',':
                WriteCode(code_getchar);
        }
    }

    WriteCode(code_epilogue);

    #undef WriteCode

    return 0;
}

void* readfile_ex(
    void *dst,
    size_t dstsize,
    const char *fname,
    size_t offset,
    size_t *psize,
    int binary
)
{
  FILE *f;
  long fsize;
  void *ptr = dst;

  if(binary)
    f = fopen(fname, "rb");
  else
    f = fopen(fname, "r");

  if(f == NULL)
  {
    printf("error: could not open file \"%s\"\n", fname);
    return NULL;
  }

  fseek(f, 0, SEEK_END);
  fsize = ftell(f) - offset;
  fseek(f, offset, SEEK_SET);

  if(!ptr)
  {
    ptr = malloc(fsize);
    if(ptr == NULL)
    {
      puts("error: not enough memory");
      return NULL;
    }
    dstsize = fsize;
  }

  fread(ptr, 1, dstsize, f);

  fclose(f);

  if(psize)
    *psize = (size_t)fsize;

  return ptr;
}

void*
readfile(
    const char *fname,
    size_t *psize
    )
{
  return readfile_ex(
      NULL, 0, fname, 0, psize, 1
      );
}
