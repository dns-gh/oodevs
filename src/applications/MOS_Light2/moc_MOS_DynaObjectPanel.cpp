/****************************************************************************
** MOS_DynaObjectPanel meta object code from reading C++ file 'MOS_DynaObjectPanel.h'
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

const char *MOS_DynaObjectPanel::className() const
{
    return "MOS_DynaObjectPanel";
}

QMetaObject *MOS_DynaObjectPanel::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_DynaObjectPanel( "MOS_DynaObjectPanel", &MOS_DynaObjectPanel::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_DynaObjectPanel::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_DynaObjectPanel", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_DynaObjectPanel::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_DynaObjectPanel", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_DynaObjectPanel::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = MOS_InfoPanel_ABC::staticMetaObject();
    static const QUMethod slot_0 = {"OnUpdate", 0, 0 };
    static const QUParameter param_slot_1[] = {
	{ "object", &static_QUType_ptr, "MOS_DynaObject", QUParameter::InOut }
    };
    static const QUMethod slot_1 = {"OnDynaObjectUpdated", 1, param_slot_1 };
    static const QUMethod slot_2 = {"OnApply", 0, 0 };
    static const QUMethod slot_3 = {"OnCancel", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "OnUpdate()", &slot_0, QMetaData::Private },
	{ "OnDynaObjectUpdated(MOS_DynaObject&)", &slot_1, QMetaData::Private },
	{ "OnApply()", &slot_2, QMetaData::Private },
	{ "OnCancel()", &slot_3, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_DynaObjectPanel", parentObject,
	slot_tbl, 4,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_DynaObjectPanel.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_DynaObjectPanel::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_DynaObjectPanel" ) )
	return this;
    return MOS_InfoPanel_ABC::qt_cast( clname );
}

bool MOS_DynaObjectPanel::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnUpdate(); break;
    case 1: OnDynaObjectUpdated((MOS_DynaObject&)*((MOS_DynaObject*)static_QUType_ptr.get(_o+1))); break;
    case 2: OnApply(); break;
    case 3: OnCancel(); break;
    default:
	return MOS_InfoPanel_ABC::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_DynaObjectPanel::qt_emit( int _id, QUObject* _o )
{
    return MOS_InfoPanel_ABC::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_DynaObjectPanel::qt_property( int id, int f, QVariant* v)
{
    return MOS_InfoPanel_ABC::qt_property( id, f, v);
}

bool MOS_DynaObjectPanel::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
