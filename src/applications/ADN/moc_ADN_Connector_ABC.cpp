/****************************************************************************
** ADN_Connector_ABC meta object code from reading C++ file 'ADN_Connector_ABC.h'
**
** Created: jeu. 7. juil. 13:46:09 2005
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

const char *ADN_Connector_ABC::className() const
{
    return "ADN_Connector_ABC";
}

QMetaObject *ADN_Connector_ABC::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ADN_Connector_ABC( "ADN_Connector_ABC", &ADN_Connector_ABC::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ADN_Connector_ABC::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_Connector_ABC", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ADN_Connector_ABC::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_Connector_ABC", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ADN_Connector_ABC::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = ADN_Ref_ABC::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "pData", &static_QUType_ptr, "void", QUParameter::In }
    };
    static const QUMethod slot_0 = {"SetData", 1, param_slot_0 };
    static const QMetaData slot_tbl[] = {
	{ "SetData(void*)", &slot_0, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ "pData", &static_QUType_ptr, "void", QUParameter::In }
    };
    static const QUMethod signal_0 = {"DataChanged", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "DataChanged(void*)", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"ADN_Connector_ABC", parentObject,
	slot_tbl, 1,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ADN_Connector_ABC.setMetaObject( metaObj );
    return metaObj;
}

void* ADN_Connector_ABC::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ADN_Connector_ABC" ) )
	return this;
    return ADN_Ref_ABC::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL DataChanged
void ADN_Connector_ABC::DataChanged( void* t0 )
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

bool ADN_Connector_ABC::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SetData((void*)static_QUType_ptr.get(_o+1)); break;
    default:
	return ADN_Ref_ABC::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ADN_Connector_ABC::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: DataChanged((void*)static_QUType_ptr.get(_o+1)); break;
    default:
	return ADN_Ref_ABC::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool ADN_Connector_ABC::qt_property( int id, int f, QVariant* v)
{
    return ADN_Ref_ABC::qt_property( id, f, v);
}

bool ADN_Connector_ABC::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
