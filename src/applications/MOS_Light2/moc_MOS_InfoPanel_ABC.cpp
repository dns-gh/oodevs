/****************************************************************************
** MOS_InfoPanel_ABC meta object code from reading C++ file 'MOS_InfoPanel_ABC.h'
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

const char *MOS_InfoPanel_ABC::className() const
{
    return "MOS_InfoPanel_ABC";
}

QMetaObject *MOS_InfoPanel_ABC::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_InfoPanel_ABC( "MOS_InfoPanel_ABC", &MOS_InfoPanel_ABC::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_InfoPanel_ABC::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_InfoPanel_ABC", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_InfoPanel_ABC::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_InfoPanel_ABC", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_InfoPanel_ABC::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QVBox::staticMetaObject();
    static const QUMethod slot_0 = {"OnUpdate", 0, 0 };
    static const QUParameter param_slot_1[] = {
	{ "agent", &static_QUType_ptr, "MOS_Agent", QUParameter::InOut }
    };
    static const QUMethod slot_1 = {"OnAgentUpdated", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "object", &static_QUType_ptr, "MOS_DynaObject", QUParameter::InOut }
    };
    static const QUMethod slot_2 = {"OnDynaObjectUpdated", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ 0, &static_QUType_ptr, "QShowEvent", QUParameter::In }
    };
    static const QUMethod slot_3 = {"showEvent", 1, param_slot_3 };
    static const QMetaData slot_tbl[] = {
	{ "OnUpdate()", &slot_0, QMetaData::Protected },
	{ "OnAgentUpdated(MOS_Agent&)", &slot_1, QMetaData::Protected },
	{ "OnDynaObjectUpdated(MOS_DynaObject&)", &slot_2, QMetaData::Protected },
	{ "showEvent(QShowEvent*)", &slot_3, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_InfoPanel_ABC", parentObject,
	slot_tbl, 4,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_InfoPanel_ABC.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_InfoPanel_ABC::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_InfoPanel_ABC" ) )
	return this;
    return QVBox::qt_cast( clname );
}

bool MOS_InfoPanel_ABC::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnUpdate(); break;
    case 1: OnAgentUpdated((MOS_Agent&)*((MOS_Agent*)static_QUType_ptr.get(_o+1))); break;
    case 2: OnDynaObjectUpdated((MOS_DynaObject&)*((MOS_DynaObject*)static_QUType_ptr.get(_o+1))); break;
    case 3: showEvent((QShowEvent*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QVBox::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_InfoPanel_ABC::qt_emit( int _id, QUObject* _o )
{
    return QVBox::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_InfoPanel_ABC::qt_property( int id, int f, QVariant* v)
{
    return QVBox::qt_property( id, f, v);
}

bool MOS_InfoPanel_ABC::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
