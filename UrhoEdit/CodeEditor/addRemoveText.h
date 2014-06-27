#ifndef ADDREMOVETEXT_H
#define ADDREMOVETEXT_H

#include <QUndoCommand>

class AddRemoveText : public QUndoCommand {
public:
    AddRemoveText();
    
protected:
    void addText();
    void removeText();
};

class AddText : public AddRemoveText {
public:
    AddText() :
        AddRemoveText()
    {
        
    }
    
    void undo() {
        removeText();
    }
    
    void redo() {
        addText();
    }
};

class RemoveText : public AddRemoveText {
public:
    RemoveText() :
        AddRemoveText() 
    {
        
    }
    
    void undo() {
        addText();
    }
    
    void redo() {
        removeText();
    }
};



#endif // ADDREMOVETEXT_H
