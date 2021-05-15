#ifndef SYNTAXITEM_H
#define SYNTAXITEM_H

#include <string>

enum SyntaxItem{
    KEYWORD,
    NUMERIC,
    STRING,
    ID,
    COMMENT,
    PREPROCESSOR,
    ELSE
};

typedef struct ParseItem {
    SyntaxItem item;
    size_t itemLength;
} ParseItem;

#endif
