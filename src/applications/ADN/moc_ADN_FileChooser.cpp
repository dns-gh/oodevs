/****************************************************************************
** ADN_FileChooser meta object code from reading C++ file 'ADN_FileChooser.h'
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

const char *ADN_FileChooser::className() const
{
    return "ADN_FileChooser";
}

QMetaObject *ADN_FileChooser::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ADN_FileChooser( "ADN_FileChooser", &ADN_FileChooser::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ADN_FileChooser::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_FileChooser", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ADN_FileChooser::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_FileChooser", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ADN_FileChooser::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "m", &static_QUType_ptr, "E_Mode", QUParameter::In }
    };
    static const QUMethod slot_0 = {"SetMode", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "file", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"SetFilename", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "dir", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"SetDirectory", 1, param_slot_2 };
    static const QUMethod slot_3 = {"ChooseFile", 0, 0 };
    static const QUParameter param_slot_4[] = {
	{ "file", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"FilenameChanged", 1, param_slot_4 };
    static const QMetaData slot_tbl[] = {
	{ "SetMode(E_Mode)", &slot_0, QMetaData::Public },
	{ "SetFilename(const QString&)", &slot_1, QMetaData::Public },
	{ "SetDirectory(const QString&)", &slot_2, QMetaData::Public },
	{ "ChooseFile()", &slot_3, QMetaData::Private },
	{ "FilenameChanged(const QString&)", &slot_4, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"ADN_FileChooser", parentObject,
	slot_tbl, 5,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ADN_FileChooser.setMetaObject( metaObj );
    return metaObj;
}

void* ADN_FileChooser::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ADN_FileChooser" ) )
	return this;
    return QWidget::qt_cast( clname );
}

bool ADN_FileChooser::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SetMode((E_Mode)(*((E_Mode*)static_QUType_ptr.get(_o+1)))); break;
    case 1: SetFilename((const QString&)static_QUType_QString.get(_o+1)); break;
    case 2: SetDirectory((const QString&)static_QUType_QString.get(_o+1)); break;
    case 3: ChooseFile(); break;
    case 4: FilenameChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ADN_FileChooser::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ADN_FileChooser::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool ADN_FileChooser::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
