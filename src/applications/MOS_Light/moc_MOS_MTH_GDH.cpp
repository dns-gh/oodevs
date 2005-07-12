/****************************************************************************
** MOS_MTH_GDH meta object code from reading C++ file 'MOS_MTH_GDH.h'
**
** Created: lun. 11. juil. 18:50:11 2005
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

const char *MOS_MTH_GDH::className() const
{
    return "MOS_MTH_GDH";
}

QMetaObject *MOS_MTH_GDH::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_MTH_GDH( "MOS_MTH_GDH", &MOS_MTH_GDH::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_MTH_GDH::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_MTH_GDH", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_MTH_GDH::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_MTH_GDH", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_MTH_GDH::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QFrame::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "bToggled", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"SlotUseGDHToogled", 1, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "SlotUseGDHToogled(bool)", &slot_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_MTH_GDH", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_MTH_GDH.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_MTH_GDH::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_MTH_GDH" ) )
	return this;
    if ( !qstrcmp( clname, "MOS_MissionTool_ABC" ) )
	return (MOS_MissionTool_ABC*)this;
    return QFrame::qt_cast( clname );
}

bool MOS_MTH_GDH::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SlotUseGDHToogled((bool)static_QUType_bool.get(_o+1)); break;
    default:
	return QFrame::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_MTH_GDH::qt_emit( int _id, QUObject* _o )
{
    return QFrame::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_MTH_GDH::qt_property( int id, int f, QVariant* v)
{
    return QFrame::qt_property( id, f, v);
}

bool MOS_MTH_GDH::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
