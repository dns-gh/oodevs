/****************************************************************************
** ADN_Connector_Vector_ABC meta object code from reading C++ file 'ADN_Connector_Vector_ABC.h'
**
** Created: jeu. 21. juil. 11:15:29 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ADN_Connector_Vector_ABC.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ADN_Connector_Vector_ABC::className() const
{
    return "ADN_Connector_Vector_ABC";
}

QMetaObject *ADN_Connector_Vector_ABC::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ADN_Connector_Vector_ABC( "ADN_Connector_Vector_ABC", &ADN_Connector_Vector_ABC::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ADN_Connector_Vector_ABC::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_Connector_Vector_ABC", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ADN_Connector_Vector_ABC::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_Connector_Vector_ABC", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ADN_Connector_Vector_ABC::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = ADN_Connector_ABC::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "pObj", &static_QUType_ptr, "void", QUParameter::In }
    };
    static const QUMethod slot_0 = {"AddItem", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "pObj", &static_QUType_ptr, "void", QUParameter::In }
    };
    static const QUMethod slot_1 = {"RemItem", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "pObj", &static_QUType_ptr, "void", QUParameter::In }
    };
    static const QUMethod slot_2 = {"AddItemNoEmit", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "pObj", &static_QUType_ptr, "void", QUParameter::In }
    };
    static const QUMethod slot_3 = {"RemItemNoEmit", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "i", &static_QUType_int, 0, QUParameter::In },
	{ "j", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"SwapItem", 2, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "lessComp", &static_QUType_ptr, "ADN_BinaryPredicateWrapper", QUParameter::InOut }
    };
    static const QUMethod slot_5 = {"Sort", 1, param_slot_5 };
    static const QUMethod slot_6 = {"Clear", 0, 0 };
    static const QUParameter param_slot_7[] = {
	{ "bInConnection", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_7 = {"Clear", 1, param_slot_7 };
    static const QMetaData slot_tbl[] = {
	{ "AddItem(void*)", &slot_0, QMetaData::Public },
	{ "RemItem(void*)", &slot_1, QMetaData::Public },
	{ "AddItemNoEmit(void*)", &slot_2, QMetaData::Public },
	{ "RemItemNoEmit(void*)", &slot_3, QMetaData::Public },
	{ "SwapItem(int,int)", &slot_4, QMetaData::Public },
	{ "Sort(ADN_BinaryPredicateWrapper&)", &slot_5, QMetaData::Public },
	{ "Clear()", &slot_6, QMetaData::Public },
	{ "Clear(bool)", &slot_7, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ "pObj", &static_QUType_ptr, "void", QUParameter::In }
    };
    static const QUMethod signal_0 = {"ItemAdded", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ "pObj", &static_QUType_ptr, "void", QUParameter::In }
    };
    static const QUMethod signal_1 = {"ItemRemoved", 1, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ "i", &static_QUType_int, 0, QUParameter::In },
	{ "j", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_2 = {"ItemSwapped", 2, param_signal_2 };
    static const QUParameter param_signal_3[] = {
	{ "lessComp", &static_QUType_ptr, "ADN_BinaryPredicateWrapper", QUParameter::InOut }
    };
    static const QUMethod signal_3 = {"Sorted", 1, param_signal_3 };
    static const QUParameter param_signal_4[] = {
	{ "bInConnection", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod signal_4 = {"Cleared", 1, param_signal_4 };
    static const QMetaData signal_tbl[] = {
	{ "ItemAdded(void*)", &signal_0, QMetaData::Public },
	{ "ItemRemoved(void*)", &signal_1, QMetaData::Public },
	{ "ItemSwapped(int,int)", &signal_2, QMetaData::Public },
	{ "Sorted(ADN_BinaryPredicateWrapper&)", &signal_3, QMetaData::Public },
	{ "Cleared(bool)", &signal_4, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"ADN_Connector_Vector_ABC", parentObject,
	slot_tbl, 8,
	signal_tbl, 5,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ADN_Connector_Vector_ABC.setMetaObject( metaObj );
    return metaObj;
}

void* ADN_Connector_Vector_ABC::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ADN_Connector_Vector_ABC" ) )
	return this;
    return ADN_Connector_ABC::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL ItemAdded
void ADN_Connector_Vector_ABC::ItemAdded( void* t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,t0);
    activate_signal( clist, o );
}

// SIGNAL ItemRemoved
void ADN_Connector_Vector_ABC::ItemRemoved( void* t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 1 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,t0);
    activate_signal( clist, o );
}

// SIGNAL ItemSwapped
void ADN_Connector_Vector_ABC::ItemSwapped( int t0, int t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 2 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_int.set(o+1,t0);
    static_QUType_int.set(o+2,t1);
    activate_signal( clist, o );
}

// SIGNAL Sorted
void ADN_Connector_Vector_ABC::Sorted( ADN_BinaryPredicateWrapper& t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 3 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL Cleared
void ADN_Connector_Vector_ABC::Cleared( bool t0 )
{
    activate_signal_bool( staticMetaObject()->signalOffset() + 4, t0 );
}

bool ADN_Connector_Vector_ABC::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: AddItem((void*)static_QUType_ptr.get(_o+1)); break;
    case 1: RemItem((void*)static_QUType_ptr.get(_o+1)); break;
    case 2: AddItemNoEmit((void*)static_QUType_ptr.get(_o+1)); break;
    case 3: RemItemNoEmit((void*)static_QUType_ptr.get(_o+1)); break;
    case 4: SwapItem((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 5: Sort((ADN_BinaryPredicateWrapper&)*((ADN_BinaryPredicateWrapper*)static_QUType_ptr.get(_o+1))); break;
    case 6: Clear(); break;
    case 7: Clear((bool)static_QUType_bool.get(_o+1)); break;
    default:
	return ADN_Connector_ABC::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ADN_Connector_Vector_ABC::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: ItemAdded((void*)static_QUType_ptr.get(_o+1)); break;
    case 1: ItemRemoved((void*)static_QUType_ptr.get(_o+1)); break;
    case 2: ItemSwapped((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 3: Sorted((ADN_BinaryPredicateWrapper&)*((ADN_BinaryPredicateWrapper*)static_QUType_ptr.get(_o+1))); break;
    case 4: Cleared((bool)static_QUType_bool.get(_o+1)); break;
    default:
	return ADN_Connector_ABC::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool ADN_Connector_Vector_ABC::qt_property( int id, int f, QVariant* v)
{
    return ADN_Connector_ABC::qt_property( id, f, v);
}

bool ADN_Connector_Vector_ABC::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
