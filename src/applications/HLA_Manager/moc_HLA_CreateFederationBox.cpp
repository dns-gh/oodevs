/****************************************************************************
** HLA_CreateFederationBox meta object code from reading C++ file 'HLA_CreateFederationBox.h'
**
** Created: mar. 12. juil. 13:37:52 2005
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

const char *HLA_CreateFederationBox::className() const
{
    return "HLA_CreateFederationBox";
}

QMetaObject *HLA_CreateFederationBox::metaObj = 0;
static QMetaObjectCleanUp cleanUp_HLA_CreateFederationBox( "HLA_CreateFederationBox", &HLA_CreateFederationBox::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString HLA_CreateFederationBox::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "HLA_CreateFederationBox", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString HLA_CreateFederationBox::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "HLA_CreateFederationBox", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* HLA_CreateFederationBox::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"OnOk", 0, 0 };
    static const QUMethod slot_1 = {"OnCancel", 0, 0 };
    static const QUMethod slot_2 = {"OnBrowse", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "OnOk()", &slot_0, QMetaData::Private },
	{ "OnCancel()", &slot_1, QMetaData::Private },
	{ "OnBrowse()", &slot_2, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ "strName", &static_QUType_ptr, "std::string", QUParameter::In },
	{ "strFomFile", &static_QUType_ptr, "std::string", QUParameter::In }
    };
    static const QUMethod signal_0 = {"FederationSettings", 2, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "FederationSettings(const std::string&,const std::string&)", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"HLA_CreateFederationBox", parentObject,
	slot_tbl, 3,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_HLA_CreateFederationBox.setMetaObject( metaObj );
    return metaObj;
}

void* HLA_CreateFederationBox::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "HLA_CreateFederationBox" ) )
	return this;
    return QDialog::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL FederationSettings
void HLA_CreateFederationBox::FederationSettings( const std::string& t0, const std::string& t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_ptr.set(o+1,&t0);
    static_QUType_ptr.set(o+2,&t1);
    activate_signal( clist, o );
}

bool HLA_CreateFederationBox::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnOk(); break;
    case 1: OnCancel(); break;
    case 2: OnBrowse(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool HLA_CreateFederationBox::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: FederationSettings((const std::string&)*((const std::string*)static_QUType_ptr.get(_o+1)),(const std::string&)*((const std::string*)static_QUType_ptr.get(_o+2))); break;
    default:
	return QDialog::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool HLA_CreateFederationBox::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool HLA_CreateFederationBox::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
