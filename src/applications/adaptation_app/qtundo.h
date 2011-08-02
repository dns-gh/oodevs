/***************************************************************************
**
** Copyright (C) 2003-2004 Trolltech AS.  All rights reserved.
**
** Licensees holding valid Qt Enterprise Edition licenses may use this
** file in accordance with the Qt Solutions License Agreement provided
** with the Solution.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
** PURPOSE.
**
** Please email sales@trolltech.com for information
** about Qt Solutions License Agreements.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
*/
#ifndef QTUNDO_H
#define QTUNDO_H

#include <QtCore/qobject.h>
#include <QtCore/qmap.h>
#include <Qt3Support/q3ptrlist.h>
#include <Qt3Support/q3listbox.h>
#include <QtCore/qstringlist.h>
#include <QtGui/qaction.h>

class QWidget;
class QAction;
class QtUndoStack;

class QtCommand : public QObject
{
    Q_OBJECT

    friend class QtUndoStack;

    public:
    enum Type { Command, MacroBegin, MacroEnd };

    QtCommand(Type type, const QString &description = QString::null,
            bool canMerge = false);
    QtCommand(const QString &description = QString::null,
            bool canMerge = true);
    virtual ~QtCommand() {}

    virtual void redo() {};
    virtual void undo() {};

    QString description() const
        { return m_description; }
    void setDescription(const QString &s)
        { m_description = s; }
    bool canMerge() const
        { return m_can_merge; }
    void setCanMerge(bool b)
        { m_can_merge = b; }
    Type type() const
        { return m_type; }

    bool isMacroBegin() const
        { return m_type == MacroBegin; }
    bool isMacroEnd() const
        { return m_type == MacroEnd; }
    bool isCommand() const
        { return m_type == Command; }

    protected:
    virtual bool mergeMeWith(QtCommand *other);

    private:
    void shortenStack();

    bool m_can_merge;
    QString m_description;
    Type m_type;
};


class QtUndoStack : public QObject, private Q3PtrList<QtCommand>
{
    Q_OBJECT

    friend class QtUndoManager;

    public:
    QtUndoStack(QObject *parent = 0, const char* szName = 0);
    virtual ~QtUndoStack() {}
    void push(QtCommand *command);
    bool canUndo() const;
    bool canRedo() const;
    QString undoDescription() const;
    QString redoDescription() const;
    QStringList undoList() const;
    QStringList redoList() const;
    bool isClean() const;

    public slots:
    void undo();
    void redo();
    void clear();

    void setClean();

    signals:
        void cleanChanged(bool clean);
    void commandExecuted();

    private:
    typedef Q3PtrListIterator<QtCommand> CommandIter;

    void undoMacro();
    void redoMacro();
    CommandIter findMacroBegin(CommandIter it) const;
    CommandIter findMacroEnd(CommandIter it) const;

    // *m_current_iter == 0 means "one-before-first"
    CommandIter m_current_iter;
    uint m_num_commands;
    int m_macro_nest;

    bool m_have_clean_command;
    const QtCommand *m_clean_command;
};

class QtUndoManager : public QObject
{
    Q_OBJECT

    public:
    QAction *createUndoAction(QWidget *parent) const;
    QAction *createRedoAction(QWidget *parent) const;

    void associateView(QObject *obj, QtUndoStack *stack);
    void disassociateView(QObject *obj);

    bool canUndo() const;
    bool canRedo() const;
    QString undoDescription() const;
    QString redoDescription() const;
    void setUndoLimit(uint i);
    uint undoLimit() const;
    QStringList undoList() const;
    QStringList redoList() const;

    static QtUndoManager *manager();

    virtual bool eventFilter(QObject *obj, QEvent *e);

    public slots:
    void undo();
    void redo();

    void updateActions();

    signals:
    void changed();

    void undoDescriptionChanged(const QString &newDescription);
    void redoDescriptionChanged(const QString &newDescription);
    void canUndoChanged(bool enabled);
    void canRedoChanged(bool enabled);

    private slots:
    void stackDestroyed(QObject *stack);
    void viewDestroyed(QObject *view);

    private:
    typedef QMap<QObject*, QtUndoStack*> StackMap;

    QtUndoManager();
    virtual ~QtUndoManager() {}
    QtUndoStack *currentStack() const;

    StackMap m_stack_map;
    Q3ListBox *m_undo_list;
    mutable QtUndoStack *m_current_stack;

    static QtUndoManager *m_manager; // singleton
    static uint m_undo_limit;

    bool m_can_undo, m_can_redo;
    QString m_undo_description, m_redo_description;
};

class QtUndoListBox : public Q3ListBox
{
    Q_OBJECT

    public:
    QtUndoListBox(QWidget* pParent = 0, const char* szName = 0);
    virtual ~QtUndoListBox() {}

    public slots:
    void updateContents();

    private slots:
        void undoOrRedo();

    private:
    int m_undo_idx;
    bool m_dont_update;
};

class UndoRedoAction : public QAction
{
    Q_OBJECT

    public:
        UndoRedoAction(QWidget *parent) : QAction(parent) {}
        virtual ~UndoRedoAction() {}

    public slots:
        // It's a pity QAction::setText() is not a slot...
        void setTextSlot(const QString &text) { setText(text); }
};

#endif
