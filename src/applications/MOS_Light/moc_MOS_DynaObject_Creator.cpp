/****************************************************************************
** MOS_DynaObject_Creator meta object code from reading C++ file 'MOS_DynaObject_Creator.h'
**
** Created: jeu. 21. juil. 13:57:40 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "MOS_DynaObject_Creator.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MOS_DynaObject_Creator::className() const
{
    return "MOS_DynaObject_Creator";
}

QMetaObject *MOS_DynaObject_Creator::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_DynaObject_Creator( "MOS_DynaObject_Creator", &MOS_DynaObject_Creator::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_DynaObject_Creator::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_DynaObject_Creator", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_DynaObject_Creator::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_DynaObject_Creator", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_DynaObject_Creator::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QFrame::staticMetaObject();
    static const QUMethod slot_0 = {"SlotValidate", 0, 0 };
    static const QUMethod slot_1 = {"SlotCancel", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "SlotValidate()", &slot_0, QMetaData::Private },
	{ "SlotCancel()", &slot_1, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_DynaObject_Creator", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_DynaObject_Creator.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_DynaObject_Creator::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_DynaObject_Creator" ) )
	return this;
    return QFrame::qt_cast( clname );
}

bool MOS_DynaObject_Creator::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SlotValidate(); break;
    case 1: SlotCancel(); break;
    default:
	return QFrame::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_DynaObject_Creator::qt_emit( int _id, QUObject* _o )
{
    return QFrame::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_DynaObject_Creator::qt_property( int id, int f, QVariant* v)
{
    return QFrame::qt_property( id, f, v);
}

bool MOS_DynaObject_Creator::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
