/****************************************************************************
** QtCommand meta object code from reading C++ file 'qtundo.h'
**
** Created: jeu. 21. juil. 11:33:20 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "qtundo.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *QtCommand::className() const
{
    return "QtCommand";
}

QMetaObject *QtCommand::metaObj = 0;
static QMetaObjectCleanUp cleanUp_QtCommand( "QtCommand", &QtCommand::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString QtCommand::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "QtCommand", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString QtCommand::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "QtCommand", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* QtCommand::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"QtCommand", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_QtCommand.setMetaObject( metaObj );
    return metaObj;
}

void* QtCommand::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "QtCommand" ) )
	return this;
    return QObject::qt_cast( clname );
}

bool QtCommand::qt_invoke( int _id, QUObject* _o )
{
    return QObject::qt_invoke(_id,_o);
}

bool QtCommand::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool QtCommand::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool QtCommand::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *QtUndoStack::className() const
{
    return "QtUndoStack";
}

QMetaObject *QtUndoStack::metaObj = 0;
static QMetaObjectCleanUp cleanUp_QtUndoStack( "QtUndoStack", &QtUndoStack::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString QtUndoStack::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "QtUndoStack", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString QtUndoStack::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "QtUndoStack", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* QtUndoStack::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"undo", 0, 0 };
    static const QUMethod slot_1 = {"redo", 0, 0 };
    static const QUMethod slot_2 = {"clear", 0, 0 };
    static const QUMethod slot_3 = {"setClean", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "undo()", &slot_0, QMetaData::Public },
	{ "redo()", &slot_1, QMetaData::Public },
	{ "clear()", &slot_2, QMetaData::Public },
	{ "setClean()", &slot_3, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ "clean", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"cleanChanged", 1, param_signal_0 };
    static const QUMethod signal_1 = {"commandExecuted", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "cleanChanged(bool)", &signal_0, QMetaData::Public },
	{ "commandExecuted()", &signal_1, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"QtUndoStack", parentObject,
	slot_tbl, 4,
	signal_tbl, 2,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_QtUndoStack.setMetaObject( metaObj );
    return metaObj;
}

void* QtUndoStack::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "QtUndoStack" ) )
	return this;
    if ( !qstrcmp( clname, "QPtrList<QtCommand>" ) )
	return (QPtrList<QtCommand>*)this;
    return QObject::qt_cast( clname );
}

// SIGNAL cleanChanged
void QtUndoStack::cleanChanged( bool t0 )
{
    activate_signal_bool( staticMetaObject()->signalOffset() + 0, t0 );
}

// SIGNAL commandExecuted
void QtUndoStack::commandExecuted()
{
    activate_signal( staticMetaObject()->signalOffset() + 1 );
}

bool QtUndoStack::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: undo(); break;
    case 1: redo(); break;
    case 2: clear(); break;
    case 3: setClean(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool QtUndoStack::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: cleanChanged((bool)static_QUType_bool.get(_o+1)); break;
    case 1: commandExecuted(); break;
    default:
	return QObject::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool QtUndoStack::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool QtUndoStack::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *QtUndoManager::className() const
{
    return "QtUndoManager";
}

QMetaObject *QtUndoManager::metaObj = 0;
static QMetaObjectCleanUp cleanUp_QtUndoManager( "QtUndoManager", &QtUndoManager::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString QtUndoManager::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "QtUndoManager", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString QtUndoManager::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "QtUndoManager", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* QtUndoManager::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"undo", 0, 0 };
    static const QUMethod slot_1 = {"redo", 0, 0 };
    static const QUMethod slot_2 = {"updateActions", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ "stack", &static_QUType_ptr, "QObject", QUParameter::In }
    };
    static const QUMethod slot_3 = {"stackDestroyed", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "view", &static_QUType_ptr, "QObject", QUParameter::In }
    };
    static const QUMethod slot_4 = {"viewDestroyed", 1, param_slot_4 };
    static const QMetaData slot_tbl[] = {
	{ "undo()", &slot_0, QMetaData::Public },
	{ "redo()", &slot_1, QMetaData::Public },
	{ "updateActions()", &slot_2, QMetaData::Public },
	{ "stackDestroyed(QObject*)", &slot_3, QMetaData::Private },
	{ "viewDestroyed(QObject*)", &slot_4, QMetaData::Private }
    };
    static const QUMethod signal_0 = {"changed", 0, 0 };
    static const QUParameter param_signal_1[] = {
	{ "newDescription", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_1 = {"undoDescriptionChanged", 1, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ "newDescription", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_2 = {"redoDescriptionChanged", 1, param_signal_2 };
    static const QUParameter param_signal_3[] = {
	{ "enabled", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod signal_3 = {"canUndoChanged", 1, param_signal_3 };
    static const QUParameter param_signal_4[] = {
	{ "enabled", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod signal_4 = {"canRedoChanged", 1, param_signal_4 };
    static const QMetaData signal_tbl[] = {
	{ "changed()", &signal_0, QMetaData::Public },
	{ "undoDescriptionChanged(const QString&)", &signal_1, QMetaData::Public },
	{ "redoDescriptionChanged(const QString&)", &signal_2, QMetaData::Public },
	{ "canUndoChanged(bool)", &signal_3, QMetaData::Public },
	{ "canRedoChanged(bool)", &signal_4, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"QtUndoManager", parentObject,
	slot_tbl, 5,
	signal_tbl, 5,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_QtUndoManager.setMetaObject( metaObj );
    return metaObj;
}

void* QtUndoManager::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "QtUndoManager" ) )
	return this;
    return QObject::qt_cast( clname );
}

// SIGNAL changed
void QtUndoManager::changed()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

// SIGNAL undoDescriptionChanged
void QtUndoManager::undoDescriptionChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 1, t0 );
}

// SIGNAL redoDescriptionChanged
void QtUndoManager::redoDescriptionChanged( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 2, t0 );
}

// SIGNAL canUndoChanged
void QtUndoManager::canUndoChanged( bool t0 )
{
    activate_signal_bool( staticMetaObject()->signalOffset() + 3, t0 );
}

// SIGNAL canRedoChanged
void QtUndoManager::canRedoChanged( bool t0 )
{
    activate_signal_bool( staticMetaObject()->signalOffset() + 4, t0 );
}

bool QtUndoManager::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: undo(); break;
    case 1: redo(); break;
    case 2: updateActions(); break;
    case 3: stackDestroyed((QObject*)static_QUType_ptr.get(_o+1)); break;
    case 4: viewDestroyed((QObject*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool QtUndoManager::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: changed(); break;
    case 1: undoDescriptionChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 2: redoDescriptionChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 3: canUndoChanged((bool)static_QUType_bool.get(_o+1)); break;
    case 4: canRedoChanged((bool)static_QUType_bool.get(_o+1)); break;
    default:
	return QObject::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool QtUndoManager::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool QtUndoManager::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *QtUndoListBox::className() const
{
    return "QtUndoListBox";
}

QMetaObject *QtUndoListBox::metaObj = 0;
static QMetaObjectCleanUp cleanUp_QtUndoListBox( "QtUndoListBox", &QtUndoListBox::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString QtUndoListBox::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "QtUndoListBox", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString QtUndoListBox::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "QtUndoListBox", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* QtUndoListBox::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QListBox::staticMetaObject();
    static const QUMethod slot_0 = {"updateContents", 0, 0 };
    static const QUMethod slot_1 = {"undoOrRedo", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "updateContents()", &slot_0, QMetaData::Public },
	{ "undoOrRedo()", &slot_1, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"QtUndoListBox", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_QtUndoListBox.setMetaObject( metaObj );
    return metaObj;
}

void* QtUndoListBox::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "QtUndoListBox" ) )
	return this;
    return QListBox::qt_cast( clname );
}

bool QtUndoListBox::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: updateContents(); break;
    case 1: undoOrRedo(); break;
    default:
	return QListBox::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool QtUndoListBox::qt_emit( int _id, QUObject* _o )
{
    return QListBox::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool QtUndoListBox::qt_property( int id, int f, QVariant* v)
{
    return QListBox::qt_property( id, f, v);
}

bool QtUndoListBox::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *UndoRedoAction::className() const
{
    return "UndoRedoAction";
}

QMetaObject *UndoRedoAction::metaObj = 0;
static QMetaObjectCleanUp cleanUp_UndoRedoAction( "UndoRedoAction", &UndoRedoAction::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString UndoRedoAction::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "UndoRedoAction", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString UndoRedoAction::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "UndoRedoAction", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* UndoRedoAction::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QAction::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"setTextSlot", 1, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "setTextSlot(const QString&)", &slot_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"UndoRedoAction", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_UndoRedoAction.setMetaObject( metaObj );
    return metaObj;
}

void* UndoRedoAction::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "UndoRedoAction" ) )
	return this;
    return QAction::qt_cast( clname );
}

bool UndoRedoAction::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: setTextSlot((const QString&)static_QUType_QString.get(_o+1)); break;
    default:
	return QAction::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool UndoRedoAction::qt_emit( int _id, QUObject* _o )
{
    return QAction::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool UndoRedoAction::qt_property( int id, int f, QVariant* v)
{
    return QAction::qt_property( id, f, v);
}

bool UndoRedoAction::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
