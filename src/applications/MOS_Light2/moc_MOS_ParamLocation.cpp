/****************************************************************************
** MOS_ParamLocation meta object code from reading C++ file 'MOS_ParamLocation.h'
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

const char *MOS_ParamLocation::className() const
{
    return "MOS_ParamLocation";
}

QMetaObject *MOS_ParamLocation::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_ParamLocation( "MOS_ParamLocation", &MOS_ParamLocation::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_ParamLocation::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ParamLocation", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_ParamLocation::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ParamLocation", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_ParamLocation::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QHBox::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "nLocationType", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"StartTracing", 1, param_slot_0 };
    static const QUMethod slot_1 = {"TracingDone", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "StartTracing(int)", &slot_0, QMetaData::Private },
	{ "TracingDone()", &slot_1, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_ParamLocation", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_ParamLocation.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_ParamLocation::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_ParamLocation" ) )
	return this;
    if ( !qstrcmp( clname, "MOS_Param_ABC" ) )
	return (MOS_Param_ABC*)this;
    return QHBox::qt_cast( clname );
}

bool MOS_ParamLocation::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: StartTracing((int)static_QUType_int.get(_o+1)); break;
    case 1: TracingDone(); break;
    default:
	return QHBox::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_ParamLocation::qt_emit( int _id, QUObject* _o )
{
    return QHBox::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_ParamLocation::qt_property( int id, int f, QVariant* v)
{
    return QHBox::qt_property( id, f, v);
}

bool MOS_ParamLocation::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
