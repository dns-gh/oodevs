/****************************************************************************
** ADN_CheckBox meta object code from reading C++ file 'ADN_CheckBox.h'
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

const char *ADN_CheckBox::className() const
{
    return "ADN_CheckBox";
}

QMetaObject *ADN_CheckBox::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ADN_CheckBox( "ADN_CheckBox", &ADN_CheckBox::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ADN_CheckBox::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_CheckBox", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ADN_CheckBox::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_CheckBox", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ADN_CheckBox::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QCheckBox::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "b", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"BoolChanged", 1, param_slot_0 };
    static const QUMethod slot_1 = {"UpdateEnableState", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "BoolChanged(bool)", &slot_0, QMetaData::Private },
	{ "UpdateEnableState()", &slot_1, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"ADN_CheckBox", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ADN_CheckBox.setMetaObject( metaObj );
    return metaObj;
}

void* ADN_CheckBox::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ADN_CheckBox" ) )
	return this;
    if ( !qstrcmp( clname, "ADN_Gfx_ABC" ) )
	return (ADN_Gfx_ABC*)this;
    return QCheckBox::qt_cast( clname );
}

bool ADN_CheckBox::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: BoolChanged((bool)static_QUType_bool.get(_o+1)); break;
    case 1: UpdateEnableState(); break;
    default:
	return QCheckBox::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ADN_CheckBox::qt_emit( int _id, QUObject* _o )
{
    return QCheckBox::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ADN_CheckBox::qt_property( int id, int f, QVariant* v)
{
    return QCheckBox::qt_property( id, f, v);
}

bool ADN_CheckBox::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
