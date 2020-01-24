%option noyywrap

%{
/* Now in a section of C that will be embedded
   into the auto-generated code. Flex will not
   try to interpret code surrounded by %{ ... %} */

/* Bring in our declarations for token types and
   the yylval variable. */
#include "histogram.hpp"
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>     /* atof */

// This is to work around an irritating bug in Flex
// https://stackoverflow.com/questions/46213840/get-rid-of-warning-implicit-declaration-of-function-fileno-in-flex
extern "C" int fileno(FILE *stream);
  double createDecimal(std::string yytext);
/* End the embedded code section. */
%}


%%
      // still need to write another block for fractions. [-?[0-9]+\.[0-9]+]..        [-]?[0-9]+[,.]?[0-9]*([/][0-9]+[,.]?[0-9]*)*

-?[0-9]+\.?[0-9]* {
                      fprintf(stderr, "Decimal Number : %s\n", yytext); /* TODO: get value out of yytext and into yylval.number */
                      yylval.numberValue = atof(yytext);
                      return Number;
                    }

[-]?[0-9]+[/]-?[0-9]+ {
                                          fprintf(stderr, "Fractional Number : %s\n", yytext); /* TODO: get value out of yytext and into yylval.number */
                                          yylval.numberValue = createDecimal(yytext);
                                          return Number;
                                        }

[a-zA-Z]+ {
                   fprintf(stderr, "Non Bracketed Word : %s\n", yytext);
                   yylval.wordValue = new std::string();
                   *yylval.wordValue=yytext;
                   return Word;
                 }


\[[^\]\n]*\] { fprintf(stderr, "Bracketed Word : %s\n", yytext);

                   yytext++;
                   yytext[strlen(yytext)-1] = 0;
                   yylval.wordValue = new std::string();
                   *yylval.wordValue=yytext;
                   return Word;
                 }

[\n]              { fprintf(stderr, "Newline\n"); }

. {              }
%%

/* Error handler. This will get called if none of the rules match. */

  double createDecimal(std::string yytext)  //find strings before and after "/"
  {
    int IndexLocationOfDivision = yytext.find('/');
    std::string TempNumber1,TempNumber2;
    double ReturnValue;

    for (int i=0; i<IndexLocationOfDivision; i++)
    {
      TempNumber1 += yytext[i];
    }

    for (int i=IndexLocationOfDivision+1; i<yytext.size(); i++)
    {
      TempNumber2 += yytext[i];
    }

    double num1 = atof(TempNumber1.c_str());
    double num2 = atof(TempNumber2.c_str());

    ReturnValue = (num1/num2);
    return ReturnValue;
  }

  void yyerror (char const *s)
  {
    fprintf (stderr, "Flex Error: %s\n", s); /* s is the text that wasn't matched */
    exit(1);
  }
