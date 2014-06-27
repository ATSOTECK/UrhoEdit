#ifndef CONSOLE_HIGHLIGHTER_H
#define CONSOLE_HIGHLIGHTER_H

#include <QSyntaxHighlighter>

#include <QHash>
#include <QTextCharFormat>

class ConsoleHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    ConsoleHighlighter(QTextDocument *parent = 0);

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

    QTextCharFormat infoFormat;
    QTextCharFormat warningFormat;
    QTextCharFormat errorFormat;
    QTextCharFormat criticalFormat;

    QTextDocument *mParent;
};

#endif // CONSOLE_HIGHLIGHTER_H
