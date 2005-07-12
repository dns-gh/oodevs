/****************************************************************************
** MOS_UnitMissionInterface meta object code from reading C++ file 'MOS_UnitMissionInterface.h'
**
** Created: mar. 12. juil. 12:42:02 2005
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

const char *MOS_UnitMissionInterface::className() const
{
    return "MOS_UnitMissionInterface";
}

QMetaObject *MOS_UnitMissionInterface::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_UnitMissionInterface( "MOS_UnitMissionInterface", &MOS_UnitMissionInterface::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_UnitMissionInterface::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_UnitMissionInterface", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_UnitMissionInterface::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_UnitMissionInterface", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_UnitMissionInterface::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = MOS_MissionInterface_ABC::staticMetaObject();
    static const QUMethod slot_0 = {"OnOk", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "OnOk()", &slot_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_UnitMissionInterface", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_UnitMissionInterface.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_UnitMissionInterface::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_UnitMissionInterface" ) )
	return this;
    return MOS_MissionInterface_ABC::qt_cast( clname );
}

bool MOS_UnitMissionInterface::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnOk(); break;
    default:
	return MOS_MissionInterface_ABC::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_UnitMissionInterface::qt_emit( int _id, QUObject* _o )
{
    return MOS_MissionInterface_ABC::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_UnitMissionInterface::qt_property( int id, int f, QVariant* v)
{
    return MOS_MissionInterface_ABC::qt_property( id, f, v);
}

bool MOS_UnitMissionInterface::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
