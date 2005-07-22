/****************************************************************************
** MOS_ParamLimits meta object code from reading C++ file 'MOS_ParamLimits.h'
**
** Created: ven. 22. juil. 13:35:03 2005
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

const char *MOS_ParamLimits::className() const
{
    return "MOS_ParamLimits";
}

QMetaObject *MOS_ParamLimits::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_ParamLimits( "MOS_ParamLimits", &MOS_ParamLimits::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_ParamLimits::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ParamLimits", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_ParamLimits::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_ParamLimits", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_ParamLimits::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QHBox::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "n", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"AcceptPopupMenuLimit", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "line", &static_QUType_ptr, "MOS_TacticalLine_ABC", QUParameter::InOut }
    };
    static const QUMethod slot_1 = {"OnTacticalLineDeleted", 1, param_slot_1 };
    static const QUMethod slot_2 = {"ClearSelectedLimits", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "AcceptPopupMenuLimit(int)", &slot_0, QMetaData::Private },
	{ "OnTacticalLineDeleted(MOS_TacticalLine_ABC&)", &slot_1, QMetaData::Private },
	{ "ClearSelectedLimits()", &slot_2, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_ParamLimits", parentObject,
	slot_tbl, 3,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_ParamLimits.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_ParamLimits::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_ParamLimits" ) )
	return this;
    if ( !qstrcmp( clname, "MOS_Param_ABC" ) )
	return (MOS_Param_ABC*)this;
    return QHBox::qt_cast( clname );
}

bool MOS_ParamLimits::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: AcceptPopupMenuLimit((int)static_QUType_int.get(_o+1)); break;
    case 1: OnTacticalLineDeleted((MOS_TacticalLine_ABC&)*((MOS_TacticalLine_ABC*)static_QUType_ptr.get(_o+1))); break;
    case 2: ClearSelectedLimits(); break;
    default:
	return QHBox::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_ParamLimits::qt_emit( int _id, QUObject* _o )
{
    return QHBox::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_ParamLimits::qt_property( int id, int f, QVariant* v)
{
    return QHBox::qt_property( id, f, v);
}

bool MOS_ParamLimits::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
