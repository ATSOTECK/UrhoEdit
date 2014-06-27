#include "bookMark.h"

BookMark::BookMark(QTextBlock block):
    mBlock(block)
{

}

void BookMark::setTextBlock(QTextBlock block) {
    mBlock = block;
}
