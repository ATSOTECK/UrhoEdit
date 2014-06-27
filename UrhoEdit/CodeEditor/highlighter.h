#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>

#include <QHash>
#include <QTextCharFormat>

#include "codeEditor.h"

class Highlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0);

    void highlightSelectedWord(const QString &text);
    
    void setSelectedWord(QString &word);
    void rehighlightLines(int line);
    
    void setPar(QTextDocument *d);

protected:
    void highlightBlock(const QString &text);

private:
    void _rehighlightLines(QList<int> &lines);
    QString sanitize(QString &word);
    
    struct HighlightingRule {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat singleQuotationFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat argFormat;
    QTextCharFormat varFormat;
    QTextCharFormat objFormat;
    QTextCharFormat selectedWordFormat;
    QTextCharFormat numberFormat;
    QTextCharFormat escapeFormat;
    QTextCharFormat doubleBracketFormat;
    
    QRegExp doubleBracketStart;
    QRegExp doubleBracketEnd;

    QTextDocument *mParent;
    QString mSelectedWord;
    QRegExp mSelectedWordPattern;
};

#endif // HIGHLIGHTER_H
