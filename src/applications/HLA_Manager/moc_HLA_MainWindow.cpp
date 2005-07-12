/****************************************************************************
** HLA_MainWindow meta object code from reading C++ file 'HLA_MainWindow.h'
**
** Created: mar. 12. juil. 13:37:34 2005
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

const char *HLA_MainWindow::className() const
{
    return "HLA_MainWindow";
}

QMetaObject *HLA_MainWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_HLA_MainWindow( "HLA_MainWindow", &HLA_MainWindow::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString HLA_MainWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "HLA_MainWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString HLA_MainWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "HLA_MainWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* HLA_MainWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    static const QUMethod slot_0 = {"OnCreate", 0, 0 };
    static const QUMethod slot_1 = {"OnDestroy", 0, 0 };
    static const QUMethod slot_2 = {"OnStart", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ "strName", &static_QUType_ptr, "std::string", QUParameter::In },
	{ "strFOM", &static_QUType_ptr, "std::string", QUParameter::In }
    };
    static const QUMethod slot_3 = {"ChangeFederationSettings", 2, param_slot_3 };
    static const QMetaData slot_tbl[] = {
	{ "OnCreate()", &slot_0, QMetaData::Private },
	{ "OnDestroy()", &slot_1, QMetaData::Private },
	{ "OnStart()", &slot_2, QMetaData::Private },
	{ "ChangeFederationSettings(const std::string&,const std::string&)", &slot_3, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"HLA_MainWindow", parentObject,
	slot_tbl, 4,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_HLA_MainWindow.setMetaObject( metaObj );
    return metaObj;
}

void* HLA_MainWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "HLA_MainWindow" ) )
	return this;
    return QMainWindow::qt_cast( clname );
}

bool HLA_MainWindow::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnCreate(); break;
    case 1: OnDestroy(); break;
    case 2: OnStart(); break;
    case 3: ChangeFederationSettings((const std::string&)*((const std::string*)static_QUType_ptr.get(_o+1)),(const std::string&)*((const std::string*)static_QUType_ptr.get(_o+2))); break;
    default:
	return QMainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool HLA_MainWindow::qt_emit( int _id, QUObject* _o )
{
    return QMainWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool HLA_MainWindow::qt_property( int id, int f, QVariant* v)
{
    return QMainWindow::qt_property( id, f, v);
}

bool HLA_MainWindow::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
