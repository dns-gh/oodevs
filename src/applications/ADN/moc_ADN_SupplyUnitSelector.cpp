/****************************************************************************
** ADN_SupplyUnitSelector meta object code from reading C++ file 'ADN_SupplyUnitSelector.h'
**
** Created: jeu. 7. juil. 13:46:08 2005
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

const char *ADN_SupplyUnitSelector::className() const
{
    return "ADN_SupplyUnitSelector";
}

QMetaObject *ADN_SupplyUnitSelector::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ADN_SupplyUnitSelector( "ADN_SupplyUnitSelector", &ADN_SupplyUnitSelector::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ADN_SupplyUnitSelector::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_SupplyUnitSelector", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ADN_SupplyUnitSelector::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_SupplyUnitSelector", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ADN_SupplyUnitSelector::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QPushButton::staticMetaObject();
    static const QUMethod slot_0 = {"OnButtonPressed", 0, 0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_ptr, "void", QUParameter::In }
    };
    static const QUMethod slot_1 = {"ItemRemoved", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ 0, &static_QUType_ptr, "void", QUParameter::In }
    };
    static const QUMethod slot_2 = {"SetItem", 1, param_slot_2 };
    static const QMetaData slot_tbl[] = {
	{ "OnButtonPressed()", &slot_0, QMetaData::Private },
	{ "ItemRemoved(void*)", &slot_1, QMetaData::Private },
	{ "SetItem(void*)", &slot_2, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"ADN_SupplyUnitSelector", parentObject,
	slot_tbl, 3,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ADN_SupplyUnitSelector.setMetaObject( metaObj );
    return metaObj;
}

void* ADN_SupplyUnitSelector::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ADN_SupplyUnitSelector" ) )
	return this;
    if ( !qstrcmp( clname, "ADN_Gfx_ABC" ) )
	return (ADN_Gfx_ABC*)this;
    return QPushButton::qt_cast( clname );
}

bool ADN_SupplyUnitSelector::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnButtonPressed(); break;
    case 1: ItemRemoved((void*)static_QUType_ptr.get(_o+1)); break;
    case 2: SetItem((void*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QPushButton::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ADN_SupplyUnitSelector::qt_emit( int _id, QUObject* _o )
{
    return QPushButton::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ADN_SupplyUnitSelector::qt_property( int id, int f, QVariant* v)
{
    return QPushButton::qt_property( id, f, v);
}

bool ADN_SupplyUnitSelector::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
