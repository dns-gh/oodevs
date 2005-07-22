/****************************************************************************
** ADN_MainWindow meta object code from reading C++ file 'ADN_MainWindow.h'
**
** Created: jeu. 21. juil. 11:15:55 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ADN_MainWindow.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ADN_MainWindow::className() const
{
    return "ADN_MainWindow";
}

QMetaObject *ADN_MainWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ADN_MainWindow( "ADN_MainWindow", &ADN_MainWindow::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ADN_MainWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_MainWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ADN_MainWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_MainWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ADN_MainWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    static const QUMethod slot_0 = {"NewProject", 0, 0 };
    static const QUMethod slot_1 = {"OpenProject", 0, 0 };
    static const QUMethod slot_2 = {"SaveProject", 0, 0 };
    static const QUMethod slot_3 = {"SaveAsProject", 0, 0 };
    static const QUMethod slot_4 = {"ExportHtml", 0, 0 };
    static const QUMethod slot_5 = {"CloseProject", 0, 0 };
    static const QUParameter param_slot_6[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out }
    };
    static const QUMethod slot_6 = {"SelectOpenMode", 1, param_slot_6 };
    static const QUMethod slot_7 = {"About", 0, 0 };
    static const QUParameter param_slot_8[] = {
	{ "bNoCommand", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_8 = {"ChangeSaveState", 1, param_slot_8 };
    static const QUParameter param_slot_9[] = {
	{ "nId", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_9 = {"ShowCoheranceTable", 1, param_slot_9 };
    static const QMetaData slot_tbl[] = {
	{ "NewProject()", &slot_0, QMetaData::Private },
	{ "OpenProject()", &slot_1, QMetaData::Private },
	{ "SaveProject()", &slot_2, QMetaData::Private },
	{ "SaveAsProject()", &slot_3, QMetaData::Private },
	{ "ExportHtml()", &slot_4, QMetaData::Private },
	{ "CloseProject()", &slot_5, QMetaData::Private },
	{ "SelectOpenMode()", &slot_6, QMetaData::Private },
	{ "About()", &slot_7, QMetaData::Private },
	{ "ChangeSaveState(bool)", &slot_8, QMetaData::Private },
	{ "ShowCoheranceTable(int)", &slot_9, QMetaData::Private }
    };
    static const QUMethod signal_0 = {"OpenModeToggled", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "OpenModeToggled()", &signal_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ADN_MainWindow", parentObject,
	slot_tbl, 10,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ADN_MainWindow.setMetaObject( metaObj );
    return metaObj;
}

void* ADN_MainWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ADN_MainWindow" ) )
	return this;
    return QMainWindow::qt_cast( clname );
}

// SIGNAL OpenModeToggled
void ADN_MainWindow::OpenModeToggled()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

bool ADN_MainWindow::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: NewProject(); break;
    case 1: OpenProject(); break;
    case 2: SaveProject(); break;
    case 3: SaveAsProject(); break;
    case 4: ExportHtml(); break;
    case 5: CloseProject(); break;
    case 6: static_QUType_bool.set(_o,SelectOpenMode()); break;
    case 7: About(); break;
    case 8: ChangeSaveState((bool)static_QUType_bool.get(_o+1)); break;
    case 9: ShowCoheranceTable((int)static_QUType_int.get(_o+1)); break;
    default:
	return QMainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ADN_MainWindow::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: OpenModeToggled(); break;
    default:
	return QMainWindow::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool ADN_MainWindow::qt_property( int id, int f, QVariant* v)
{
    return QMainWindow::qt_property( id, f, v);
}

bool ADN_MainWindow::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
