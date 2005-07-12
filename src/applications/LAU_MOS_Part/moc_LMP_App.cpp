/****************************************************************************
** LMP_App meta object code from reading C++ file 'LMP_App.h'
**
** Created: mar. 12. juil. 13:30:04 2005
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

const char *LMP_App::className() const
{
    return "LMP_App";
}

QMetaObject *LMP_App::metaObj = 0;
static QMetaObjectCleanUp cleanUp_LMP_App( "LMP_App", &LMP_App::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString LMP_App::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "LMP_App", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString LMP_App::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "LMP_App", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* LMP_App::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QApplication::staticMetaObject();
    static const QUMethod slot_0 = {"SlotQuit", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "SlotQuit()", &slot_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"LMP_App", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_LMP_App.setMetaObject( metaObj );
    return metaObj;
}

void* LMP_App::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "LMP_App" ) )
	return this;
    return QApplication::qt_cast( clname );
}

bool LMP_App::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SlotQuit(); break;
    default:
	return QApplication::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool LMP_App::qt_emit( int _id, QUObject* _o )
{
    return QApplication::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool LMP_App::qt_property( int id, int f, QVariant* v)
{
    return QApplication::qt_property( id, f, v);
}

bool LMP_App::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
