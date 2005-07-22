/****************************************************************************
** MOS_App meta object code from reading C++ file 'MOS_App.h'
**
** Created: jeu. 21. juil. 13:57:07 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "MOS_App.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MOS_App::className() const
{
    return "MOS_App";
}

QMetaObject *MOS_App::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_App( "MOS_App", &MOS_App::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_App::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_App", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_App::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_App", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_App::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QApplication::staticMetaObject();
    static const QUMethod slot_0 = {"OnTimeOut", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "OnTimeOut()", &slot_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_App", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_App.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_App::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_App" ) )
	return this;
    return QApplication::qt_cast( clname );
}

bool MOS_App::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnTimeOut(); break;
    default:
	return QApplication::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_App::qt_emit( int _id, QUObject* _o )
{
    return QApplication::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_App::qt_property( int id, int f, QVariant* v)
{
    return QApplication::qt_property( id, f, v);
}

bool MOS_App::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
