/****************************************************************************
** MOS_ODBToolbar meta object code from reading C++ file 'MOS_ODBToolbar.h'
**
** Created: ven. 22. juil. 13:35:02 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MOS_ODBToolbar::className() const
{
    return "MOS_ODBToolbar";
}

QMetaObject *MOS_ODBToolbar::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_ODBToolbar( "MOS_ODBToolbar", &MOS_ODBToolbar::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_ODBToolbar::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ODBToolbar", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_ODBToolbar::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ODBToolbar", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_ODBToolbar::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QToolBar::staticMetaObject();
    static const QUMethod slot_0 = {"OnOpen", 0, 0 };
    static const QUMethod slot_1 = {"OnSave", 0, 0 };
    static const QUMethod slot_2 = {"OnSaveAs", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "OnOpen()", &slot_0, QMetaData::Private },
	{ "OnSave()", &slot_1, QMetaData::Private },
	{ "OnSaveAs()", &slot_2, QMetaData::Private }
    };
    static const QUMethod signal_0 = {"ODBLoaded", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "ODBLoaded()", &signal_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_ODBToolbar", parentObject,
	slot_tbl, 3,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_ODBToolbar.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_ODBToolbar::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_ODBToolbar" ) )
	return this;
    return QToolBar::qt_cast( clname );
}

// SIGNAL ODBLoaded
void MOS_ODBToolbar::ODBLoaded()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

bool MOS_ODBToolbar::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnOpen(); break;
    case 1: OnSave(); break;
    case 2: OnSaveAs(); break;
    default:
	return QToolBar::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_ODBToolbar::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: ODBLoaded(); break;
    default:
	return QToolBar::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool MOS_ODBToolbar::qt_property( int id, int f, QVariant* v)
{
    return QToolBar::qt_property( id, f, v);
}

bool MOS_ODBToolbar::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
