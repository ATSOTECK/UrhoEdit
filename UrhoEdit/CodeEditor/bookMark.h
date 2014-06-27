#ifndef BOOKMARK_H
#define BOOKMARK_H

#include <QTextBlock>

class BookMark {
public:
    BookMark(QTextBlock block);

    void setTextBlock(QTextBlock block);

    QTextBlock getTextBlock() const {
        return mBlock;
    }

private:
    QTextBlock mBlock;
};

#endif // BOOKMARK_H
