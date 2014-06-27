#include <QtGui>

#include "ConsoleHighlighter.h"

ConsoleHighlighter::ConsoleHighlighter(QTextDocument *parent) :
    QSyntaxHighlighter(parent)
{
    mParent = parent;

    HighlightingRule rule;

    infoFormat.setForeground(QColor(0x2E, 0xCC, 0xFA));
    //keywordFormat.setFontWeight(QFont::setItalic(true));
    QStringList keywordPatterns;
    keywordPatterns << "INFO";
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp("\\b" + pattern + "\\b");
        rule.format = infoFormat;
        highlightingRules.append(rule);
    }
    
    keywordPatterns.clear();
    
    warningFormat.setForeground(QColor(230, 219, 116));
    //keywordFormat.setFontWeight(QFont::setItalic(true));
    keywordPatterns << "WARNING";
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp("\\b" + pattern + "\\b");
        rule.format = warningFormat;
        highlightingRules.append(rule);
    }
    
    keywordPatterns.clear();
    
    errorFormat.setForeground(Qt::red);
    //keywordFormat.setFontWeight(QFont::setItalic(true));
    keywordPatterns << "ERROR";
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp("\\b" + pattern + "\\b");
        rule.format = errorFormat;
        highlightingRules.append(rule);
    }
    
    keywordPatterns.clear();
    
    criticalFormat.setForeground(Qt::red);
    //keywordFormat.setFontWeight(QFont::setItalic(true));
    keywordPatterns << "CRITICAL";
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp("\\b" + pattern + "\\b");
        rule.format = criticalFormat;
        highlightingRules.append(rule);
    }
    
    keywordPatterns.clear();
}

void ConsoleHighlighter::highlightBlock(const QString &text) {
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    
    /*
    QString blk = currentBlock().text();
    if (blk.contains("function") && blk.contains("(")) {
        
    }
    */
    
    setCurrentBlockState(0);
}

void ConsoleHighlighter::highlightSelectedWord(const QString &text) {
    highlightBlock(text);
}

void ConsoleHighlighter::setSelectedWord(QString &word) {
    if (word.length() > 2) {
        //mSelectedWordPattern = QRegExp("\\b" + sanitize(word) + "\\b");
    } else {
        //mSelectedWordPattern = QRegExp();
    }
}

QString ConsoleHighlighter::sanitize(QString &word) {
    return word.replace("\\", "\\\\");
}

void ConsoleHighlighter::rehighlightLines(int line) {
    if (!mParent)
        return;
    
    QTextBlock block = mParent->findBlockByNumber(line);
    rehighlightBlock(block);
}

void ConsoleHighlighter::_rehighlightLines(QList<int> &/*lines*/) {
    
}

void ConsoleHighlighter::setPar(QTextDocument *d) {
    mParent = d;
}







