/****************************************************************************
** ADN_Graph meta object code from reading C++ file 'ADN_Graph.h'
**
** Created: jeu. 21. juil. 11:15:29 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ADN_Graph.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ADN_Graph::className() const
{
    return "ADN_Graph";
}

QMetaObject *ADN_Graph::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ADN_Graph( "ADN_Graph", &ADN_Graph::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ADN_Graph::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_Graph", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ADN_Graph::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_Graph", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ADN_Graph::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = GQ_Plot::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "pObject", &static_QUType_ptr, "void", QUParameter::In }
    };
    static const QUMethod slot_0 = {"SetSelectedItem", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_ptr, "T_DataItemVector", QUParameter::In }
    };
    static const QUMethod slot_1 = {"OnSelectionChanged", 1, param_slot_1 };
    static const QMetaData slot_tbl[] = {
	{ "SetSelectedItem(void*)", &slot_0, QMetaData::Public },
	{ "OnSelectionChanged(const T_DataItemVector&)", &slot_1, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ "pObject", &static_QUType_ptr, "void", QUParameter::In }
    };
    static const QUMethod signal_0 = {"ItemSelected", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "ItemSelected(void*)", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"ADN_Graph", parentObject,
	slot_tbl, 2,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ADN_Graph.setMetaObject( metaObj );
    return metaObj;
}

void* ADN_Graph::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ADN_Graph" ) )
	return this;
    return GQ_Plot::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL ItemSelected
void ADN_Graph::ItemSelected( void* t0 )
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

bool ADN_Graph::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SetSelectedItem((void*)static_QUType_ptr.get(_o+1)); break;
    case 1: OnSelectionChanged((const T_DataItemVector&)*((const T_DataItemVector*)static_QUType_ptr.get(_o+1))); break;
    default:
	return GQ_Plot::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ADN_Graph::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: ItemSelected((void*)static_QUType_ptr.get(_o+1)); break;
    default:
	return GQ_Plot::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool ADN_Graph::qt_property( int id, int f, QVariant* v)
{
    return GQ_Plot::qt_property( id, f, v);
}

bool ADN_Graph::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
