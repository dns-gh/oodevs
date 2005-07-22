/****************************************************************************
** MOS_Logger meta object code from reading C++ file 'MOS_Logger.h'
**
** Created: jeu. 21. juil. 15:28:03 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "MOS_Logger.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MOS_Logger::className() const
{
    return "MOS_Logger";
}

QMetaObject *MOS_Logger::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_Logger( "MOS_Logger", &MOS_Logger::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_Logger::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_Logger", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_Logger::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_Logger", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_Logger::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QListView::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "pItem", &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ "pos", &static_QUType_varptr, "\x0e", QUParameter::In }
    };
    static const QUMethod slot_0 = {"OnRequestPopup", 2, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "OnRequestPopup(QListViewItem*,const QPoint&)", &slot_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_Logger", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_Logger.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_Logger::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_Logger" ) )
	return this;
    if ( !qstrcmp( clname, "MT_Logger_ABC" ) )
	return (MT_Logger_ABC*)this;
    return QListView::qt_cast( clname );
}

bool MOS_Logger::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnRequestPopup((QListViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2))); break;
    default:
	return QListView::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_Logger::qt_emit( int _id, QUObject* _o )
{
    return QListView::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_Logger::qt_property( int id, int f, QVariant* v)
{
    return QListView::qt_property( id, f, v);
}

bool MOS_Logger::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
