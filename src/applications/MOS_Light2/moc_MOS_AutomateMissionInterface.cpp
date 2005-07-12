/****************************************************************************
** MOS_AutomateMissionInterface meta object code from reading C++ file 'MOS_AutomateMissionInterface.h'
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

const char *MOS_AutomateMissionInterface::className() const
{
    return "MOS_AutomateMissionInterface";
}

QMetaObject *MOS_AutomateMissionInterface::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_AutomateMissionInterface( "MOS_AutomateMissionInterface", &MOS_AutomateMissionInterface::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_AutomateMissionInterface::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_AutomateMissionInterface", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_AutomateMissionInterface::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_AutomateMissionInterface", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_AutomateMissionInterface::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = MOS_MissionInterface_ABC::staticMetaObject();
    static const QUMethod slot_0 = {"OnOk", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "OnOk()", &slot_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_AutomateMissionInterface", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_AutomateMissionInterface.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_AutomateMissionInterface::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_AutomateMissionInterface" ) )
	return this;
    return MOS_MissionInterface_ABC::qt_cast( clname );
}

bool MOS_AutomateMissionInterface::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnOk(); break;
    default:
	return MOS_MissionInterface_ABC::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_AutomateMissionInterface::qt_emit( int _id, QUObject* _o )
{
    return MOS_MissionInterface_ABC::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_AutomateMissionInterface::qt_property( int id, int f, QVariant* v)
{
    return MOS_MissionInterface_ABC::qt_property( id, f, v);
}

bool MOS_AutomateMissionInterface::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
