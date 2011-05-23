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
#pragma warning( push, 0 )

#include "qtundo.h"

#include "moc_qtundo.cpp"

#include <qapplication.h>
#include <qiconset.h>
#include <qdragobject.h>
#include <qpixmap.h>
#include <qwidget.h>




/*!
    \class QtCommand

    \brief The QtCommand class is the base class of all commands stored on a QtUndoStack.

    For an overview of the Qt Undo/Redo framework, see \l overview.html.

    A QtCommand represents a single editing action which an
    application can make, for example, inserting or deleting a block
    of text in a text editor. QtCommand knows how to apply a change to
    an object and how to undo the change.

    A change is applied by calling redo(). A change is undone by
    calling undo(). The default implementations of these virtual
    functions in QtCommand do nothing, they must be overriden in a
    derived class.

    \code
    CmdChangeImage::CmdChangeImage(Face *face, const QString &image)
    {
    m_face = face;
    m_old_image = face->image();
    m_new_image = image;
    setCanMerge(false);
    setDescription(tr("change %1 to %2").arg(m_old_image).arg(m_new_image));
    }

    void CmdChangeImage::redo()
    {
    m_face->setImage(m_new_image);
    }

    void CmdChangeImage::undo()
    {
    m_face->setImage(m_old_image);
    }
    \endcode

    A QtCommand object has a type(), which is used to differentiate
    between ordinary commands and macro delimiters. An ordinary
    command is of type() \c Command. A macro delimiter's type() is
    either \c MacroBegin or \c MacroEnd. Sequences of commands between
    macro delimiters are undone and redone in one go, rather than one
    at a time.

    Each QtCommand has a merge flag, returned by canMerge().
    QtUndoStack::push() will attempt to merge a new command with the
    current command in the stack only if they are both instances of
    the same class (ascertained using QObject::className()) \e and if
    the new command's merge flag is true.

    A QtCommand object has a description(), which describes its effect
    on the edited object. This description is used to assign
    meaningful information to the widgets which trigger undo or
    redo in an application, such as the QAction objects returned by
    QtUndoManager::createUndoAction() and QtUndoManager::createRedoAction().

    \warning All classes derived from QtCommand must contain the
    Q_OBJECT macro in their declaration.

    \sa QtUndoStack QtUndoManager
*/

/*!
    \enum QtCommand::Type

    This enumerated type is used by QtCommand to differentiate between
    ordinary commands and macro delimiters.

    \value Command The command is a normal command which applies a
    change to an object.
    \value MacroBegin The command marks the start of a macro sequence.
    \value MacroEnd The command marks the end of a macro sequence.
*/

/*!
    Constructs a QtCommand object with \a description and the merge
    flag set to \a canMerge. The command type is set to
    \c Command. This is the preferred constructor for commands
    which are not macro delimiters.

    \sa description() canMerge()
*/

QtCommand::QtCommand(const QString &description, bool canMerge)
{
    setDescription(description);
    setCanMerge(canMerge);
    m_type = Command;
}

/*!
    Constructs a QtCommand object of \a type, with \a description and
    the merge flag set to \a canMerge. This is the preferred
    constructor for macro delimiters.

    \sa QtCommand::Type description() canMerge()
*/

QtCommand::QtCommand(Type type, const QString &description, bool canMerge)
{
    setDescription(description);
    setCanMerge(canMerge);
    m_type = type;
}

/*!
    \fn QtCommand::Type QtCommand::type() const

    Returns the type of this command.
*/
/*!
    \fn bool QtCommand::isMacroBegin() const

    Returns true if the command is a macro start delimiter; otherwise
    returns false.

    \sa QtCommand::Type type()
*/
/*!
    \fn bool QtCommand::isMacroEnd() const

    Returns true if the command is a macro end delimiter; otherwise
    returns false.

    \sa QtCommand::Type type()

*/
/*!
    \fn bool QtCommand::isCommand() const

    Returns true if the command is an ordinary editing command;
    otherwise returns false.

    \sa QtCommand::Type type()
*/
/*!
    \fn QString QtCommand::description() const

    Returns a string which describes the effect of this command on
    the edited object.

    Typical examples include "typing", "delete block", "change font", etc.

    This description is used to assign meaningful information to the widgets
    which trigger undo or redo in an application, such as the QAction
    objects returned by QtUndoManager::createUndoAction() and
    QtUndoManager::createRedoAction().


    \sa setDescription() QtUndoStack::undoDescription() QtUndoStack::redoDescription() QtUndoManager::undoDescription() QtUndoManager::redoDescription()
*/

/*!
    \fn void QtCommand::setDescription(const QString &s)

    Sets the description of this command to \a s.

    \sa description()
*/

/*!
    \fn bool QtCommand::canMerge() const

    Returns the command's merge flag. QtUndoStack::push() will attempt
    to merge a new command with the command on top of the stack only
    if they are both instances of the same class (ascertained using
    QObject::className()) \e and if the  new command's merge flag is
    true.

    \sa setCanMerge() mergeMeWith()
*/

/*!
    \fn void QtCommand::setCanMerge(bool b)

    Sets the merge flag for this command to \a b.

    \sa canMerge() mergeMeWith()
*/

/*!
    \fn bool QtCommand::mergeMeWith(QtCommand *other)

    Attempts to merge \a other into this command. Returns true if it
    succeeds; otherwise returns false. If this function returns false,
    QtUndoStack::push() will push \a other on top of the stack. The
    default implementation does nothing and returns false. This function
    must be reimplemented in each derived command class which sets its
    merge flag to true.

    \code
    CmdChangeColor::CmdChangeColor(Face *face, const QString &color)
    {
    ...
    setCanMerge(true);
    ...
    }

    bool CmdChangeColor::mergeMeWith(QtCommand *c)
    {
    CmdChangeColor *other = (CmdChangeColor*) c;

    if (m_face != other->m_face)
        return false;

    m_new_color = other->m_new_color;
    setDescription("change " + m_old_color + " to " + m_new_color);
    setDescription(tr("change %1 to %2").arg(m_old_color).arg(m_new_color));
    return true;
    }
    \endcode

    \sa canMerge() setCanMerge()
*/

bool QtCommand::mergeMeWith(QtCommand *)
{
    return false;
}

/*!
    \fn void QtCommand::redo()

    This virtual function must be reimplemented by subclasses to apply
    changes.
*/

/*!
    \fn void QtCommand::undo()

    This virtual function must be reimplemented by subclasses to undo
    the changes applied by redo(). Calling redo() and then undo()
    should leave the edited object unmodified.
*/

/*!
    \class QtUndoStack

    \brief The QtUndoStack class is a stack of QtCommand objects.

    For an overview of the Qt Undo/Redo framework, see \l overview.html.

    New commands are added with push(). When a command is pushed on to
    the stack, QtUndoStack takes ownership of the command and applies
    it by calling QtCommand::redo(). Undo and redo are invoked with
    undo() and redo(). These functions may be called directly, or
    through the QtUndoManager::undoAction() and
    QtUndoManager::redoAction() QAction objects. In the latter case,
    QtUndoManager will choose a stack based on which object has the
    focus, and call the relevant undo() or redo() function.

    \code
    FaceEdit::FaceEdit(QWidget *parent, const char *name)
    : QWidget(parent, name, Qt::WDestructiveClose)
    {
    ...
    m_undo_stack = new QtUndoStack(this);
    ...
    }

    void FaceEdit::setImage(const QString &image_name)
    {
    Face *face = focusedFace();
    ...
    m_undo_stack->push(new CmdChangeImage(face, image_name));
    ...
    }
    \endcode

    QtUndoStack supports command compression. This is useful when
    several commands can be compressed into a single command, which
    can be undone and redone in one go. An example of this is a text
    editor. When the user types in a character, a new action is
    created, which inserts that character into the document at the
    cursor position. However, it is more convenient for the user to be
    able to undo or redo typing in whole words, sentences and
    paragraphs. Command compression allows these single-character
    commands to be merged into a single command which inserts or
    deletes chunks of text. See push() for more information on
    command compression.

    QtUndoStack supports command macros. A command macro is a sequence
    of commands which are undone and redone in one go. The sequence
    starts with a QtCommand object whose \link QtCommand::type()
    type()\endlink is \link QtCommand::MacroBegin MacroBegin\endlink.
    The \link QtCommand::description() description()\endlink of this
    command is taken to describe the effect of the entire macro. The
    sequence ends with a QtCommand object whose \link
    QtCommand::type() type()\endlink is
    \link QtCommand::MacroEnd MacroEnd\endlink. See undo() and redo()
    for more information on command macros.

    \code
    void FaceEdit::clearFaces()
    {
    ...
    m_undo_stack->push(new QtCommand(QtCommand::MacroBegin, "Clear faces"));
    for (; *child_it != 0; ++child_it) {
        Face *face = (Face*) *child_it;
        m_undo_stack->push(new CmdChangeImage(face, "none"));
        m_undo_stack->push(new CmdChangeColor(face, "green"));
    }
    m_undo_stack->push(new QtCommand(QtCommand::MacroEnd));
    ...
    }
    \endcode

    A certain state of the edited object may be marked as "clean", using
    setClean(). This function is usually called whenever the edited object
    is saved.  QtUndoStack emits the cleanChanged() signal whenever
    the edited object enters or leaves the clean state.

    \sa QtCommand QtUndoManager
*/

/*!
    Constructs a QtUndoStack object. \a parent and \a name are passed
    to the QObject constructor. Additionally, \a parent is registered
    as the target for the newly created stack. When \a parent has the
    focus the QtUndoManager will use this undo stack for its undo()
    and redo() actions.
*/

QtUndoStack::QtUndoStack(QObject *parent, const char *name)
    : QObject(parent, name), m_current_iter(*this)
{
    setAutoDelete(true);
    QtUndoManager *manager = QtUndoManager::manager();
    connect(this, SIGNAL(destroyed(QObject*)),
        manager, SLOT(stackDestroyed(QObject*)));
    manager->associateView(parent, this);

    m_macro_nest = 0;
    m_num_commands = 0;

    m_clean_command = 0; // the initial empty stack is clean
    m_have_clean_command = true;
}

/*!
    Returns true if the edited object is in a clean state; otherwise returns false.
    The edited object is in a clean state if setClean() was previously called and
    the state of the edited object at the time of the call was the same as it is now.

    More precisely, the edited object is in a clean state if the current pointer of
    the QtUndoStack points to the same command as it did at the time of the last
    call to setClean().

    \sa setClean() cleanChanged()
*/
bool QtUndoStack::isClean() const
{
    return m_have_clean_command
            && m_clean_command == m_current_iter.current();
}

/*!
    \fn void QtUndoStack::cleanChanged(bool clean)

    This signal is emitted whenever the edited object enters or leaves the
    clean state. If \a clean is true, the edited object is currently clean;
    otherwise it is currently not clean.

    \sa isClean() setClean()
*/

/*!
    Marks state of the edited object as clean. This function is usually called
    whenever the edited object is saved. The cleanChanged() signal is emited
    whenever the edited object enters or leaves the clean state.

    \sa isClean() cleanChanged()
*/

void QtUndoStack::setClean()
{
    bool old_clean = isClean();

    m_have_clean_command = true;
    m_clean_command = m_current_iter.current();

    if (old_clean != isClean())
        emit cleanChanged(isClean());
}

QtUndoStack::CommandIter QtUndoStack::findMacroBegin(CommandIter it) const
{
    int nest = 1;

    QtCommand *command = *it;
    Q_ASSERT(command != 0 && command->isMacroEnd());
    do {
    --it;

    command = *it;
    Q_ASSERT(command != 0);

    if (command->isMacroBegin())
        --nest;
    else if (command->isMacroEnd())
        ++nest;
    Q_ASSERT(nest >= 0);
    } while (nest > 0 || !command->isMacroBegin());

    return it;
}

QtUndoStack::CommandIter QtUndoStack::findMacroEnd(CommandIter it) const
{
    int nest = 1;

    QtCommand *command = *it;
    Q_ASSERT(command != 0 && command->isMacroBegin());
    do {
    ++it;

    command = *it;
    Q_ASSERT(command != 0);

    if (command->isMacroEnd())
        --nest;
    else if (command->isMacroBegin())
        ++nest;
    Q_ASSERT(nest >= 0);
    } while (nest > 0 || !command->isMacroEnd());

    return it;
}

/*!
    Pushes \a command onto this stack or merges it with the current
    command. If the current command is not topmost on the stack, all
    commands above it are deleted. Calls the \a command's
    QtCommand::redo() function to apply it.

    This function will attempt to merge a new command with the command
    on top of the stack only if they are both instances of the same
    class (ascertained using QObject::className()) \e and if the new
    command's merge flag is true.

    If there is no current command on this stack, or \a command and
    the current command are of different types, or \a command's merge
    flag is false, or QtCommand::mergeMeWith() returns false, push()
    will push \a command onto the stack.

    \sa undo() redo() clear()
*/


void QtUndoStack::push(QtCommand *command)
{
    bool old_clean = isClean();

    command->redo();

    // If the current command on the stack is not last, we delete all
    // commands that follow it before adding the new command.
    while (!m_current_iter.atLast()) {
    if (m_have_clean_command
            && m_current_iter.current() == m_clean_command) {
        m_have_clean_command = false;
        m_clean_command = 0;
    }
    removeLast();
    }

    switch (command->type()) {

    case QtCommand::Command: {
        // Either merge the new command with the current command, or append it to the
        // stack.
        QtCommand *current = m_current_iter.current();
        if (command->canMerge()
            && current != 0
            && current->metaObject() == command->metaObject()
            && current->mergeMeWith(command))
        delete command;
        else
        append(command);

        if (m_macro_nest == 0) {
        ++m_num_commands;
            emit commandExecuted();
        }

        break;
    }

    case QtCommand::MacroBegin:
        append(command);
        ++m_macro_nest;
        break;

    case QtCommand::MacroEnd:
        if (m_macro_nest == 0) {
        qWarning("QtUndoStack::push(): MacroEnd without MacroBegin");
                break;
        }

        append(command);
        --m_macro_nest;

        // Set the description to the corresponding MacroBegin's description
        CommandIter it(*this);
        it.toLast();
        it = findMacroBegin(it); // I've just pushed the MacroEnd
        Q_ASSERT(*it != 0);
        command->setDescription((*it)->description());

        if (m_macro_nest == 0) {
        ++m_num_commands;
        emit commandExecuted();
        }

        break;
    }

    m_current_iter.toLast();
    QtUndoManager::manager()->updateActions();
    if (old_clean != isClean())
        emit cleanChanged(isClean());
}

/*!
    \fn bool QtUndoStack::canUndo() const

    Returns true if a command is available for undo; otherwise returns
    false. Undo is not possible if the stack is empty or if the bottom
    command on the stack has already been undone.

    \sa undo() canRedo()
*/

bool QtUndoStack::canUndo() const
{
    if (isEmpty())
    return false;

    if (m_macro_nest > 0)
    return false;

    if (m_current_iter.current() == 0)
    return false;

    return true;
}

/*!
    \fn bool QtUndoStack::canRedo() const

    Returns true if a command is available for redo; otherwise returns
    false. Redo is not possible if the stack is empty or if the top
    command on the stack has already been redone.

    \sa redo() canUndo()
*/

bool QtUndoStack::canRedo() const
{
    if (isEmpty())
    return false;

    if (m_macro_nest > 0)
    return false;

    // We know the stack is not empty
    if (m_current_iter.current() == 0)
    return true;

    if (m_current_iter.atLast())
    return false;

    return true;
}


void QtUndoStack::undoMacro()
{
    Q_ASSERT(m_macro_nest == 0);
    Q_ASSERT(m_current_iter.current() != 0);
    Q_ASSERT(m_current_iter.current()->type() == QtCommand::MacroEnd);

    int nest = 1;

    bool command_executed = false;

    QtCommand *command = 0;
    do {
    --m_current_iter;

    command = m_current_iter.current();
    Q_ASSERT(command != 0);

    if (command->isMacroBegin())
        --nest;
    else if (command->isMacroEnd())
        ++nest;
    else
        command_executed = true;
    Q_ASSERT(nest >= 0);

    command->undo();
    } while (nest > 0 || !command->isMacroBegin());

    if (command_executed)
        emit commandExecuted();
}

void QtUndoStack::redoMacro()
{
    Q_ASSERT(m_macro_nest == 0);
    Q_ASSERT(m_current_iter.current() != 0);
    Q_ASSERT(m_current_iter.current()->type() == QtCommand::MacroBegin);

    int nest = 1;

    bool command_executed = false;

    QtCommand *command = 0;
    do {
    ++m_current_iter;

    command = m_current_iter.current();
    Q_ASSERT(command != 0);

    if (command->isMacroBegin())
        ++nest;
    else if (command->isMacroEnd())
        --nest;
    else
        command_executed = true;
    Q_ASSERT(nest >= 0);

    command->redo();
    } while (!command->isMacroEnd() || nest > 0);

    if (command_executed)
        emit commandExecuted();
}

/*!
    If the current command's \link QtCommand::type() type()\endlink is
    \c Command, calls the current command's \link QtCommand::undo()
    undo()\endlink function and moves the current pointer one command
    down the stack.

    If the current command's \link QtCommand::type() type()\endlink is
    \c MacroEnd, traverses the stack downwards calling each command's
    \link QtCommand::undo() undo()\endlink, until a command of type \c
    MacroBegin is found. The current pointer is then set to one
    command below the macro begin marker.

    \sa push() redo() canUndo()
*/

void QtUndoStack::undo()
{
    bool old_clean = isClean();

    if (!canUndo()) {
    qWarning("QtUndoStack::undo(): can't undo");
    return;
    }

    QtCommand *command = m_current_iter.current();
    Q_ASSERT(!command->isMacroBegin());

    if (command->isCommand()) {
    command->undo();
    emit commandExecuted();
    } else
    undoMacro();

    --m_current_iter;

    QtUndoManager::manager()->updateActions();
    if (old_clean != isClean())
        emit cleanChanged(isClean());
}


/*!
    If the current command's \link QtCommand::type() type()\endlink is
    \c Command, moves the current pointer one command up in the stack
    and calls the new current command's \link QtCommand::redo()
    redo()\endlink.

    If the current command's \link QtCommand::type() type()\endlink is
    \c MacroBegin, traverses the stack upwards calling each command's
    \link QtCommand::redo() redo()\endlink, until a command of type
    \c MacroEnd is found. The current pointer is left pointing
    to this command.

    \sa push() undo() canRedo()
*/

void QtUndoStack::redo()
{
    bool old_clean = isClean();

    if (!canRedo()) {
    qWarning("QtUndoStack::redo(): can't redo");
    return;
    }

    if (m_current_iter.current() == 0)
    m_current_iter.toFirst();
    else
    ++m_current_iter;

    QtCommand *command = m_current_iter.current();
    Q_ASSERT(!command->isMacroEnd());

    if (command->isCommand()) {
    command->redo();
        emit commandExecuted();
    }
    else
    redoMacro();

    QtUndoManager::manager()->updateActions();
    if (old_clean != isClean())
        emit cleanChanged(isClean());
}

/*!
    Returns \link QtCommand::description() description()\endlink for
    the current command on the stack, or a null string if there is no
    current command.

    \sa QtUndoManager::undoDescription() QtCommand::description() redoDescription()
*/

QString QtUndoStack::undoDescription() const
{
    if (canUndo())
    return m_current_iter.current()->description();
    else
    return QString::null;
}

/*!
    Returns \link QtCommand::description() description()\endlink for
    the command preceding the current command on the stack, or a null
    string if the current command is at the top.

    \sa QtUndoManager::redoDescription() QtCommand::description() undoDescription()
*/

QString QtUndoStack::redoDescription() const
{
    if (canRedo()) {
    CommandIter it = m_current_iter;
    if (it.current() == 0)
        it.toFirst();
    else
        ++it;
    return it.current()->description();
    }
    else
    return QString::null;
}

/*!
    Clears all the commands on this undo stack.

    \sa push()
*/

void QtUndoStack::clear()
{
    bool old_clean = isClean();

    QPtrList<QtCommand>::clear();
    m_macro_nest = 0;
    m_num_commands = 0;
    m_have_clean_command = true;
    m_clean_command = 0;

    if (old_clean != isClean())
        emit cleanChanged(isClean());
}

/*!
    Returns a list of descriptions of all the commands up to the
    current command in this stack.

    \sa redoList()
*/

QStringList QtUndoStack::undoList() const
{
    QStringList result;

    if (m_macro_nest > 0)
    return result;

    if (m_current_iter.current() == 0)
    return result;

    CommandIter it(*this);
    for (; *it != 0; ++it) {
    QtCommand *command = *it;

    result.append(command->description());

    Q_ASSERT(!command->isMacroEnd());
    if (command->isMacroBegin())
        it = findMacroEnd(it);

    if (it == m_current_iter)
        break;
    }

    return result;
}

/*!
    Returns a list of descriptions of all the commands preceding the
    current command in this stack.

    \sa undoList()
*/

QStringList QtUndoStack::redoList() const
{
    QStringList result;

    if (m_macro_nest > 0)
    return result;

    CommandIter it = m_current_iter;
    if (it.current() == 0)
    it.toFirst();
    else
    ++it;

    for (; it.current() != 0; ++it) {
    QtCommand *command = it.current();

    result.append(command->description());

    Q_ASSERT(!command->isMacroEnd());
    if (command->isMacroBegin())
        it = findMacroEnd(it);
    }

    return result;
}

/*!
    \fn void QtUndoStack::commandExecuted()

    This signal is emitted whenever a QtCommand on the stack is undone
    or redone. When macro commands are undone or redone, this signal is
    emitted only once, even though the macro may contain more than one
    command.
*/


/*!
    \class QtUndoManager

    \brief The QtUndoManager class manages command stacks in an
    undo/redo framework based on the Command design pattern.

    For an overview of the Qt Undo/Redo framework, see \l overview.html.

    QtUndoManager keeps a list of QtUndoStack objects. Each is a list
    of QtCommand objects and a pointer to the last executed command
    (the undo stack's \e current command). Undo is invoked by calling
    the current command's QtCommand::undo() function and making the
    previous command in the stack the current command. Redo is invoked
    by making the next command in the stack the current command and
    calling its QtCommand::redo() function.

    An application has one global QtUndoManager, accessed through the
    static function QtUndoManager::manager() (which creates the
    QtUndoManager when it is called for the first time).

    Undo and redo are requested through the undo() and redo() slots.
    QtUndoManager also provides the functions createUndoAction()
    and createRedoAction() for creating QAction objects that trigger
    undo and redo. These QActions have the additional benefit of keeping
    their text properties in sync with undoDescription() and redoDescription(),
    as well as disabling themselves whenever no commands are available
    for undo or redo.

    \code
    MainWindow::MainWindow(QWidget *parent, const char *name)
    : QMainWindow(parent, name)
    {
    ...
    QtUndoManager *manager = QtUndoManager::manager();
    QAction *undo_action = manager->createUndoAction(this);
    QAction *redo_action = manager->createRedoAction(this);
    undo_action->setAccel(QKeySequence("Ctrl+Z"));
    redo_action->setAccel(QKeySequence("Shift+Ctrl+Z"));

    QToolBar *toolbar = new QToolBar(this);
    undo_action->addTo(toolbar);
    redo_action->addTo(toolbar);

    QPopupMenu *editmenu = new QPopupMenu(this);
    undo_action->addTo(editmenu);
    redo_action->addTo(editmenu);
    ...
    }
    \endcode

    A single application can have multiple undo stacks, typically one
    for each editor window in an MDI application. Each stack is
    associated with a widget, called the stack's \e target. undo() and
    redo() requests are directed to a stack whenever its target, or a
    child of its target, has the keyboard focus. A target is associated
    with a stack in QtUndoStack's constructor. Additional targets may be
    associated with a stack using associateView(). This is useful when
    two or more editor windows edit the same underlying object. An SDI
    aplication typically has a single stack associated with the
    application's main window.

    Whenever the widget with the keyboard focus has no targets in its
    parent chain, the QAction objects created using createUndoAction() and
    createRedoAction() are disabled.

    \img qtundo-menu.png
    <p>
    \img qtundo-toolbar.png

    \sa QtCommand QtUndoStack
*/

static bool loadPixmapFromMimeSource(const QString &abs_name, QPixmap &pm)
{
    const QMimeSource *ms = QMimeSourceFactory::defaultFactory()->data(abs_name);
    if (ms)
    return QImageDrag::decode(ms, pm);
    else
    return false;
}

/*! \internal */

QtUndoManager::QtUndoManager()
{
    m_current_stack = 0;

    updateActions();

    qApp->installEventFilter(this);
}

/*!
    Creates a QAction object connected to the QtUndoManager's undo()
    slot. \a parent becomes the owner and parent of the QAction. This is
    significant, since any accelerators that are assigned to the
    QAction will only work within \a parent.

    The returned QAction will keep its text property in sync with
    undoDescription() and disable itself whenever no commands are available
    for undo.

    If the application's default QMimeSourceFactory contains a pixmap
    called "undo", this pixmap is assigned to the QAction.

    \sa undo() undoDescription() createRedoAction()
*/

QAction *QtUndoManager::createUndoAction(QWidget *parent) const
{
    UndoRedoAction *undo_action = new UndoRedoAction(parent);
    connect(undo_action, SIGNAL(activated()), this, SLOT(undo()));
    connect(this, SIGNAL(undoDescriptionChanged(const QString&)),
                undo_action, SLOT(setTextSlot(const QString&)));
    connect(this, SIGNAL(canUndoChanged(bool)),
                undo_action, SLOT(setEnabled(bool)));

    undo_action->setEnabled(m_can_undo);
    undo_action->setText(m_undo_description);

    QPixmap pm;
    if (loadPixmapFromMimeSource("undo", pm))
    undo_action->setIconSet(pm);

    return undo_action;
}

/*!
    Creates a QAction object connected to the QtUndoManager's redo()
    slot. \a parent becomes the owner and parent of the QAction. This is
    significant, since any accelerators that are assigned to the
    QAction will only work within \a parent.

    The returned QAction will keep its text property in sync with
    redoDescription() and disable itself whenever no commands are available
    for redo.

    If the application's default QMimeSourceFactory contains a pixmap
    called "redo", this pixmap is assigned to the QAction.

    \sa redo() redoDescription() createUndoAction()
*/

QAction *QtUndoManager::createRedoAction(QWidget *parent) const
{
    UndoRedoAction *redo_action = new UndoRedoAction(parent);
    connect(redo_action, SIGNAL(activated()), this, SLOT(redo()));
    connect(this, SIGNAL(redoDescriptionChanged(const QString&)),
                redo_action, SLOT(setTextSlot(const QString&)));
    connect(this, SIGNAL(canRedoChanged(bool)),
                redo_action, SLOT(setEnabled(bool)));

    redo_action->setEnabled(m_can_redo);
    redo_action->setText(m_redo_description);

    QPixmap pm;
    if (loadPixmapFromMimeSource("redo", pm))
    redo_action->setIconSet(pm);

    return redo_action;
}

/*! \internal */

void QtUndoManager::updateActions()
{
    bool changed = false;

    QtUndoStack *stack = currentStack();

    bool undo_enabled = stack != 0 && stack->canUndo();
    QString undo_description = "Undo";
    if (undo_enabled)
        undo_description += " " + stack->undoDescription();

    if (undo_enabled != m_can_undo) {
    changed = true;
        m_can_undo = undo_enabled;
        emit canUndoChanged(undo_enabled);
    }
    if (undo_description != m_undo_description) {
    changed = true;
        m_undo_description = undo_description;
    emit undoDescriptionChanged(undo_description);
    }

    bool redo_enabled = stack != 0 && stack->canRedo();
    QString redo_description = "Redo";
    if (redo_enabled)
        redo_description += " " + stack->redoDescription();

    if (redo_enabled != m_can_redo) {
    changed = true;
        m_can_redo = redo_enabled;
        emit canRedoChanged(redo_enabled);
    }
    if (redo_description != m_redo_description) {
    changed = true;
        m_redo_description = redo_description;
    emit redoDescriptionChanged(redo_description);
    }

    if (changed)
        emit QtUndoManager::changed();
}

/*!
    Returns true if a command is available for redo; otherwise returns
    false. Redo is not possible if: the widget with the keyboard focus
    has no targets in its parent chain, or a target is found but the
    associated stack is empty, or if the last command on the stack
    has already been undone.

    The QAction returned by createUndoAction() disables itself
    whenever canUndo() is false.

    \sa undo() canRedo()
*/

bool QtUndoManager::canUndo() const
{
    return m_can_undo;
}

/*!
    Returns true if a command is available for undo; otherwise returns
    false. Undo is not possible if: the widget with the keyboard focus
    has no targets in its parent chain, or a target is found but the
    associated stack is empty, or if the first command on the stack
    has already been redone.

    A QAction returned by createRedoAction() disables itself
    whenever canRedo() is false.

    \sa redo() canUndo()
*/

bool QtUndoManager::canRedo() const
{
    return m_can_redo;
}

/*! \internal */

void QtUndoManager::stackDestroyed(QObject *stack)
{
    if (m_current_stack == stack)
    m_current_stack = 0;

    // remove all views associated with that stack from the map
    StackMap::iterator it = m_stack_map.begin();
    while (it != m_stack_map.end()) {
    if (*it == stack) {
        disconnect(it.key(), 0, this, 0);
        StackMap::iterator tmp = it; // iterator invalidation
        ++tmp;
        m_stack_map.remove(it);
        it = tmp;
    }
    else ++it;
    }

    updateActions();
}

/*! \internal */

void QtUndoManager::viewDestroyed(QObject *view)
{
    StackMap::iterator it = m_stack_map.find(view);
    if (it == m_stack_map.end()) {
    qWarning("QtUndoManager::viewDestroyed(): no such view");
    return;
    }

    if (*it == m_current_stack)
    m_current_stack = 0;

    m_stack_map.erase(it);

    updateActions();
}


/*!
    Directs an undo request to the appropriate QtUndoStack. The stack
    is chosen by finding the widget with the keyboard focus and
    searching its parent chain for a target. If a target is found,
    QtUndoStack::undo() is called on the associated stack. If no such
    target is found, this function does nothing.

    \sa redo() canUndo()
*/

void QtUndoManager::undo()
{
    QtUndoStack *stack = currentStack();

    if (stack == 0 || !stack->canUndo()) {
    qWarning("QtUndoManager::undo(): can't undo");
    return;
    }

    stack->undo();

    updateActions();
}

/*!
    Directs the redo request to the appropriate QtUndoStack. The stack
    is chosen by finding the widget with the keyboard focus and
    searching its parent chain for a target. If a target is found,
    QtUndoStack::redo() is called on the associated stack. If no such
    target is found, this function does nothing.

    \sa undo() canRedo()
*/

void QtUndoManager::redo()
{
    QtUndoStack *stack = currentStack();

    if (stack == 0 || !stack->canRedo()) {
    qWarning("QtUndoManager::redo(): can't redo");
    return;
    }

    stack->redo();

    updateActions();
}

QtUndoManager *QtUndoManager::m_manager = 0;
uint QtUndoManager::m_undo_limit = 0;

/*!
    Returns the application-global instance of QtUndoManager, creating it if
    it does not yet exist.
*/

QtUndoManager *QtUndoManager::manager()
{
    if (m_manager == 0)
    m_manager = new QtUndoManager;

    return m_manager;
}

/*!
    Disassociates \a obj from any stack.

    \sa associateView()
*/

void QtUndoManager::disassociateView(QObject *obj)
{
    if (obj == 0) {
    qWarning("QtUndoManager::disassociateView(): canot disassociate null object");
    return;
    }

    StackMap::iterator it = m_stack_map.find(obj);
    if (it == m_stack_map.end()) {
    qWarning("QtUndoManager::disassociateView(): object has no associated stack");
    return;
    }

    disconnect(obj, 0, this, 0);
    m_stack_map.erase(it);
}

/*!
    Associates \a obj with \a stack, making \a obj the \a stack's
    target. undo() and redo() requests will be directed to \a stack,
    whenever \a obj or one of its children has the keyboard focus.

    \sa disassociateView()
*/

void QtUndoManager::associateView(QObject *obj, QtUndoStack *stack)
{
    if (obj == 0) {
    qWarning("QtUndoManager::associateView(): cannot associate a null object");
    return;
    }

    if (stack == 0) {
    qWarning("QtUndoManager::associateView(): cannot associate a null stack");
    return;
    }

    if (m_stack_map.contains(obj)) {
    qWarning("QtUndoManager::associateView(): view already associated with a stack");
    return;
    }

    m_stack_map[obj] = stack;
    connect(obj, SIGNAL(destroyed(QObject*)), this,
        SLOT(viewDestroyed(QObject*)));

    updateActions();
}

/*!
    Returns the maximum size that any undo stack can grow to. A size
    of 0 means that the stacks can grow indefinitely.

    \sa setUndoLimit()
*/

uint QtUndoManager::undoLimit() const
{
    return m_undo_limit;
}

/*!
    Sets the maximum size that any stack can grow to, to \a i. A
    size of 0 means that the stacks can grow indefinitely.

    \sa undoLimit()
*/


void QtUndoManager::setUndoLimit(uint i)
{
    m_undo_limit = i;
}

/*! \internal */

bool QtUndoManager::eventFilter(QObject*, QEvent *e)
{
    if (e->type() == QEvent::FocusIn || e->type() == QEvent::FocusOut)
    updateActions();

    return false;
}

/*! \internal */

QtUndoStack *QtUndoManager::currentStack() const
{
    QWidget *w = qApp->focusWidget();
    while (w != 0) {
    StackMap::const_iterator it = m_stack_map.find(w);
    if (it != m_stack_map.end()) {
        m_current_stack = (QtUndoStack*) *it;
        break;
    }
    w = w->parentWidget();
    }

    return m_current_stack;
}

/*!
    \fn void QtUndoManager::changed()
    \internal
*/

/*!
    Returns the current undo description.

    The undo description is a string that describes what effects calling
    QtUndoManager::undo() will have on the edited object.

    It contains the text returned by QtCommand::description() for the current
    command on the QtUndoStack associated with the target widget that contains
    the keyboard focus.

    The QAction returned by createUndoAction() keeps its text property
    in sync with the undo description. This function is useful if you want to
    trigger undo with a custom widget, rather than this QAction.

    \sa undoDescriptionChanged() createUndoAction() QtCommand::description() QtUndoStack::undoDescription()
*/

QString QtUndoManager::undoDescription() const
{
    return m_undo_description;
}

/*!
    Returns the current redo description.

    The redo description is a string that describes what effects calling
    QtUndoManager::redo() will have on the edited object.

    It contains the text returned by QtCommand::description() for the
    command preceding the current command on the QtUndoStack associated
    with the target widget that contains the keyboard focus.

    The QAction returned by createRedoAction() keeps its text property
    in sync with the redo description. This function is useful if you want to
    trigger redo with a custom widget, rather than this QAction.

    \sa redoDescriptionChanged() createRedoAction() QtCommand::description() QtUndoStack::redoDescription()
*/

QString QtUndoManager::redoDescription() const
{
    return m_redo_description;
}

/*!
    \fn void QtUndoManager::redoDescriptionChanged(const QString &newDescription)

    This signal is emitted whenever the redo description changes.
    \a newDescription is the new redo description. It is useful when
    you want to trigger redo using a custom widget, rather than
    using the QAction returned by createRedoAction().

    \sa redoDescription() canRedoChanged() undoDescriptionChanged()
*/

/*!
    \fn void QtUndoManager::undoDescriptionChanged(const QString &newDescription)

    This signal is emitted whenever the undo description changes.
    \a newDescription is the new undo description. It is useful when
    you want to trigger undo using a custom widget, rather than
    using the QAction returned by createUndoAction().

    \sa undoDescription() canUndoChanged() redoDescriptionChanged()
*/

/*!
    \fn void QtUndoManager::canUndoChanged(bool enabled)

    This signal is emitted whenever the state reported by canUndo()
    changes. \a enabled is the new state.

    This function is useful if you want to trigger undo with a custom widget,
    rather than the QAction returned by createUndoAction().

    \sa canUndo() undoDescriptionChanged() canRedoChanged()
*/

/*!
    \fn void QtUndoManager::canRedoChanged(bool enabled)

    This signal is emitted whenever the state reported by canRedo()
    changes. \a enabled is the new state.

    This function is useful if you want to trigger redo with a custom widget,
    rather than the QAction returned by createRedoAction().

    \sa canRedo() redoDescriptionChanged() canUndoChanged()
*/

/*!
    Returns a list of descriptions of all commands up to the the
    current command in the stack associated with the currently focused
    target. If no target has focus, returns an empty list.

    \sa redoList()
*/

QStringList QtUndoManager::undoList() const
{
    QtUndoStack *stack = currentStack();
    if (stack == 0)
    return QStringList();

    return stack->undoList();
}

/*!
    Returns a list of descriptions of all commands preceding the
    current command in the stack associated with the currently focused
    target. If no target has focus, returns an empty list.

    \sa undoList()
*/

QStringList QtUndoManager::redoList() const
{
    QtUndoStack *stack = currentStack();
    if (stack == 0)
    return QStringList();

    return stack->redoList();
}

/*!
    \class QtUndoListBox

    \brief The QtUndoListBox class is a QListBox which displays the
    commands on the QtUndoStack associated with the focused target.

    QtUndoListBox keeps track of changes in the stack and focus in the
    application, and updates itself accordingly. Selecting a command
    causes undo or redo until the selected command is current. Hence
    the history of changes can be undone or redone by traversing the
    list.

    \img qtundo-list.png
*/

static const QString g_empty_text = "<empty>";

/*!
    Constructs a QtUndoListBox. \a parent and \a name are passed to
    the QListBox constructor.
*/

QtUndoListBox::QtUndoListBox(QWidget *parent, const char *name)
    : QListBox(parent, name)
{
    connect(QtUndoManager::manager(), SIGNAL(changed()),
        this, SLOT(updateContents()));
    connect(this, SIGNAL(highlighted(int)), this, SLOT(undoOrRedo()));

    m_undo_idx = -1;
    m_dont_update = false;

    updateContents();
}

/*! \internal */

void QtUndoListBox::updateContents()
{
    if (m_dont_update)
        return;

    blockSignals(true);

    QtUndoManager *manager = QtUndoManager::manager();

    QStringList undo_list;
    undo_list.append(g_empty_text);
    undo_list += manager->undoList();
    m_undo_idx = undo_list.count() - 1;
    undo_list += manager->redoList();

    QStringList current_list;
    for (uint i = 0; i < count(); ++i)
        current_list.append(text(i));

    if (current_list != undo_list) {
        clear();
    insertStringList(undo_list);
    }

    setCurrentItem(m_undo_idx);

    blockSignals(false);
}

/*! \internal */

void QtUndoListBox::undoOrRedo()
{
    int  idx = currentItem();
    if (idx == -1)
        return;

    if (idx == m_undo_idx)
        return;

    QtUndoManager *manager = QtUndoManager::manager();

    m_dont_update = true;
    if (idx < m_undo_idx) {
        for (int i = idx; i < m_undo_idx; ++i) {
        Q_ASSERT(manager->canUndo());
        manager->undo();
    }
    } else {
        for (int i = m_undo_idx; i < idx; ++i) {
        Q_ASSERT(manager->canRedo());
        manager->redo();
    }
    }
    m_dont_update = false;

    updateContents();
}



