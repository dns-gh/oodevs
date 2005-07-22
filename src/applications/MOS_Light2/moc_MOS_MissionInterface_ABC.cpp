/****************************************************************************
** MOS_MissionInterface_ABC meta object code from reading C++ file 'MOS_MissionInterface_ABC.h'
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

const char *MOS_MissionInterface_ABC::className() const
{
    return "MOS_MissionInterface_ABC";
}

QMetaObject *MOS_MissionInterface_ABC::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_MissionInterface_ABC( "MOS_MissionInterface_ABC", &MOS_MissionInterface_ABC::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_MissionInterface_ABC::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_MissionInterface_ABC", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_MissionInterface_ABC::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_MissionInterface_ABC", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_MissionInterface_ABC::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QVBox::staticMetaObject();
    static const QUMethod slot_0 = {"OnOk", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "OnOk()", &slot_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_MissionInterface_ABC", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_MissionInterface_ABC.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_MissionInterface_ABC::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_MissionInterface_ABC" ) )
	return this;
    return QVBox::qt_cast( clname );
}

bool MOS_MissionInterface_ABC::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnOk(); break;
    default:
	return QVBox::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_MissionInterface_ABC::qt_emit( int _id, QUObject* _o )
{
    return QVBox::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_MissionInterface_ABC::qt_property( int id, int f, QVariant* v)
{
    return QVBox::qt_property( id, f, v);
}

bool MOS_MissionInterface_ABC::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
