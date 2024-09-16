%{
    #define _CRT_SKIP_SECTION
    #include <string.h> // strdup, strchr
    #include <stdlib.h> // atoi and strtol
    #include <stdio.h>  // printf and fprintf
    #include "unistd.h"
    #include "parser.tab.hpp" 

    int lineNum = 1;
%}

%option noyywrap

%%

[ \t]                           { }

#.*                             { return COMMENT; }

[a-zA-Z_][a-zA-Z_0-9]*[ \t]*:   { yylval.strVal = strdup(yytext); 
                                  char *colon_pos = strchr(yylval.strVal, ':');
                                  if (colon_pos != NULL) {
                                    *colon_pos = '\0';
                                  }
                                  if (yylval.strVal == NULL) {
                                    fprintf(stderr, "Greška u alokaciji memorije\n");                                        
                                    exit(1);  
                                } return LABEL; }

"+"                             { return PLUS; }
"$"                             { return DOLLAR; }
"["                             { return LEFT_BRACKET; }
"]"                             { return RIGHT_BRACKET; }
","                             { return COMMA; }
"{"                             { return LEFT_BRACE; }
"}"                             { return RIGHT_BRACE; }

"\n"                            { lineNum++; return ENDL; }

"halt"                          { return INS_HALT; }
"int"                           { return INS_INT; }
"iret"                          { return INS_IRET; }
"call"                          { return INS_CALL; }
"ret"                           { return INS_RET; }
"jmp"                           { return INS_JMP; }
"beq"                           { return INS_BEQ; }
"bne"                           { return INS_BNE; }
"bgt"                           { return INS_BGT; }
"push"                          { return INS_PUSH; }
"pop"                           { return INS_POP; }
"xchg"                          { return INS_XCHG; }
"add"                           { return INS_ADD; }
"sub"                           { return INS_SUB; }
"mul"                           { return INS_MUL; }
"div"                           { return INS_DIV; }
"not"                           { return INS_NOT; }
"and"                           { return INS_AND; }
"or"                            { return INS_OR; }
"xor"                           { return INS_XOR; }
"shl"                           { return INS_SHL; }
"shr"                           { return INS_SHR; }
"ld"                            { return INS_LD; }
"st"                            { return INS_ST; }
"csrrd"                         { return INS_CSRRD; }
"csrwr"                         { return INS_CSRWR; }

".global"                       { return DIR_GLOBAL; }
".extern"                       { return DIR_EXTERN; }
".section"                      { return DIR_SECTION; }
".word"                         { return DIR_WORD; }
".skip"                         { return DIR_SKIP; }
".end"                          { return EOF; }

-?[1-9][0-9]*                   { yylval.number = strtoull(yytext, NULL, 10); return NUMBER; }
0[0-7]*                         { yylval.number = strtoull(yytext, NULL, 8); return NUMBER; }
0x[0-9a-fA-F]+                  { yylval.number = strtoull(yytext, NULL, 16); return NUMBER; }



%r([0-9]|1[0-5])                { yylval.number = atoi(yytext + 2); return GPR; }
%sp                             { yylval.number = 14; return GPR; }
%pc                             { yylval.number = 15; return GPR; }

%status                         { yylval.number = 0; return CSR; }
%handler                        { yylval.number = 1; return CSR; }
%cause                          { yylval.number = 2; return CSR; }

[a-zA-Z._][a-zA-Z._0-9]*        { yylval.strVal = strdup(yytext); 
                                  if (yylval.strVal == NULL) {
                                    fprintf(stderr, "Greška u alokaciji memorije\n");                                        
                                    exit(1);  
                                } return SYMBOL;}

                                
.                               {
                                    fprintf(stderr, "Nije prepoznat karakter '%s' na liniji %d\n", yytext, lineNum);
                                    exit(1);
                                }

%%
