%{
    #include <string.h>: // strdup, strchr
    #include <stdlib.h>: // atoi and strtol
    #include <stdio.h>  // printf and fprintf

    lineNum = 1
%}

%option noyywrap
%option nounput

%%

[ \t]                           { };

#.*                             { return COMMENT; }

[a-zA-Z_][a-zA-Z_0-9]*[ \t]*:   { yylval.strval = strdup(yytext); 
                                  if (yylval.strval == NULL) {
                                    fprintf(stderr, "Greška u alokaciji memorije\n");                                        
                                    exit(1);  
                                } return LABEL; }

"+"                             { return PLUS; }
"$"                             { return DOLLAR; }
"["                             { return LEFT_BRACKET; }
"]"                             { return RIGHT_BRACKET; }
","                             { return COMMA; }

"\n"                            { lineNum++; return ENDL; }
.                               {
                                    fprintf(stderr, "Nije prepoznat karakter '%s' na liniji %d\n", yytext, lineNum);
                                    exit(1);
                                }

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
".end"                          { return DIR_END; }

-?[0-9]+                        { yylval.numval = atoi(yytext); return NUMBER; }
0[0-7]+                         { yylval.numval = strtol(yytext, NULL, 8); return NUMBER; }
0x[0-9a-fA-F]+                  { yylval.numval = strtol(yytext, NULL, 16); return NUMBER; }



%r[0-9]+                        { yylval.numval = atoi(yytext + 2); return GPR; }
sp                              { yylval.numval = 14; return GPR; }
pc                              { yylval.numval = 15; return GPR; }

%status                         { yylval.numval = 0; return CSR; }
%handler                        { yylval.numval = 1; return CSR; }
%cause                          { yylval.numval = 2; return CSR; }

[a-zA-Z._][a-zA-Z._0-9]*        { yylval.strval = strdup(yytext); 
                                  if (yylval.strval == NULL) {
                                    fprintf(stderr, "Greška u alokaciji memorije\n");                                        
                                    exit(1);  
                                } return SYMBOL;}

%%


