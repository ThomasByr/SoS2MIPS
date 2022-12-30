%{

  #include "parser.h"
  #include "lib.h"
  #include <string.h>
  #include <stdio.h>


  int lineNumber = 1;

  int keywordLookup(char *);
  char *savedText;

%}

%option noinput
%option nounput
%option noyywrap

letter [a-zA-Z]
digit [0-9]
charl [^'"\n{digit}]

idl ({letter}|_)({letter}|{digit}|_)*
wordl ({letter}|{digit}|_)*

integerl {digit}+
floatl {integerl}"."{integerl}?|"."{integerl}

white [ \t]+

%x stringl
%x commentl

%%

{white}         /* skip whitespace, but we need to count line numbers */
\n              lineNumber++;

{idl}            {
    savedText = strdup(yytext);
    yylval = (YYSTYPE){ .string = savedText };
    return keywordLookup(yytext);
}

{integerl}     {
    savedText = strdup(yytext);
    yylval = (YYSTYPE){ .integer_t = strtoi(yytext) };
    return integer;
}


\"              BEGIN(stringl);
<stringl>\n      {
    savedText = strdup(yytext);
    alert("Unterminated string constant: %s", savedText);
    lineNumber++;
}
<stringl>\\\"    yymore();
<stringl>\"      {
    BEGIN(INITIAL);
    yytext[--yyleng] = '\0';
    savedText = strdup(yytext);
    printf("stringl: %s\n", savedText);
    return string;
}
<stringl>.       yymore();

\/\*            BEGIN(commentl);
<commentl>\*\/   BEGIN(INITIAL);
<commentl>.      /* skip comment text */
<commentl>\n     lineNumber++; /* eat up multiline comments */

\+               return '+';
-               return '-';
\*               return '*';
\/              return '/';
%                return '%';
!               return '!';
\<               return '<';
>                 return '>';
=                return '=';
;               return ';';
,                return ',';
\(                return '(';
\)                 return ')';
\[                return '[';
]                return ']';
\{              return '{';
\}               return '}';
\$                return '$';

<<EOF>>          return 0;

. {
  alert("Unexpected character: %s", yytext);
}

%%

int keywordLookup(char *s)
{
    struct keywordTokenPair {
       char *keyword;
       int token;
    };
    typedef struct keywordTokenPair KeywordTokenPair;

    const int KEYWORD_COUNT = 34;

    static KeywordTokenPair keywordTokenPairs[] =
        {
            {"declare", declare},
            {"if", IF},
            {"then", THEN},
            {"fi", FI},
            {"elif", ELIF},
            {"else", ELSE},
            {"for", FOR},
            {"do", DO},
            {"in", IN},
            {"done", DONE},
            {"while", WHILE},
            {"until", UNTIL},
            {"case", CASE},
            {"esac", ESAC},
            {"echo", EKKO},
            {"read", READ},
            {"return", RETURN},
            {"exit", EXIT},
            {"test", test},
            {"-o", or},
            {"-a", and},
            {"==", eq},
            {"-eq", eq},
            {"!=", neq},
            {"-ne", neq},
            {"-gt", gt},
            {"-ge", ge},
            {"-lt", lt},
            {"-le", le},
            {"-n", nnull},
            {"-z", null},
            {"expr", expr},
            {"local", local},
            {NULL , ID}
        };

    KeywordTokenPair *keywordTokenPair;

    keywordTokenPairs[KEYWORD_COUNT - 1].keyword = s;

    for (keywordTokenPair = keywordTokenPairs; strcmp(keywordTokenPair->keyword, s) != 0; keywordTokenPair++);

    return keywordTokenPair->token;
}