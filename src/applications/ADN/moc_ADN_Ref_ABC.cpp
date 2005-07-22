/****************************************************************************
** ADN_Ref_ABC meta object code from reading C++ file 'ADN_Ref_ABC.h'
**
** Created: jeu. 21. juil. 11:15:55 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ADN_Ref_ABC.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ADN_Ref_ABC::className() const
{
    return "ADN_Ref_ABC";
}

QMetaObject *ADN_Ref_ABC::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ADN_Ref_ABC( "ADN_Ref_ABC", &ADN_Ref_ABC::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ADN_Ref_ABC::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_Ref_ABC", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ADN_Ref_ABC::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_Ref_ABC", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ADN_Ref_ABC::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "pObj", &static_QUType_ptr, "void", QUParameter::In }
    };
    static const QUMethod slot_0 = {"Invalidate", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "pObj", &static_QUType_ptr, "void", QUParameter::In },
	{ "bDel", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"Invalidate", 2, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "pObj", &static_QUType_ptr, "void", QUParameter::In }
    };
    static const QUMethod slot_2 = {"AutoCreate", 1, param_slot_2 };
    static const QUMethod slot_3 = {"AutoDelete", 0, 0 };
    static const QUMethod slot_4 = {"AutoDeleteEx", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "Invalidate(void*)", &slot_0, QMetaData::Public },
	{ "Invalidate(void*,bool)", &slot_1, QMetaData::Public },
	{ "AutoCreate(void*)", &slot_2, QMetaData::Protected },
	{ "AutoDelete()", &slot_3, QMetaData::Private },
	{ "AutoDeleteEx()", &slot_4, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ "pObj", &static_QUType_ptr, "void", QUParameter::In },
	{ "bDel", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"Invalidated", 2, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "Invalidated(void*,bool)", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"ADN_Ref_ABC", parentObject,
	slot_tbl, 5,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ADN_Ref_ABC.setMetaObject( metaObj );
    return metaObj;
}

void* ADN_Ref_ABC::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ADN_Ref_ABC" ) )
	return this;
    return QObject::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL Invalidated
void ADN_Ref_ABC::Invalidated( void* t0, bool t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_ptr.set(o+1,t0);
    static_QUType_bool.set(o+2,t1);
    activate_signal( clist, o );
}

bool ADN_Ref_ABC::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: Invalidate((void*)static_QUType_ptr.get(_o+1)); break;
    case 1: Invalidate((void*)static_QUType_ptr.get(_o+1),(bool)static_QUType_bool.get(_o+2)); break;
    case 2: AutoCreate((void*)static_QUType_ptr.get(_o+1)); break;
    case 3: AutoDelete(); break;
    case 4: AutoDeleteEx(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ADN_Ref_ABC::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: Invalidated((void*)static_QUType_ptr.get(_o+1),(bool)static_QUType_bool.get(_o+2)); break;
    default:
	return QObject::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool ADN_Ref_ABC::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool ADN_Ref_ABC::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
