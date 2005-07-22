/****************************************************************************
** ADN_EditLine_ABC meta object code from reading C++ file 'ADN_EditLine_ABC.h'
**
** Created: jeu. 21. juil. 11:15:29 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ADN_EditLine_ABC.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *ADN_EditLine_ABC::className() const
{
    return "ADN_EditLine_ABC";
}

QMetaObject *ADN_EditLine_ABC::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ADN_EditLine_ABC( "ADN_EditLine_ABC", &ADN_EditLine_ABC::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ADN_EditLine_ABC::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_EditLine_ABC", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ADN_EditLine_ABC::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_EditLine_ABC", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ADN_EditLine_ABC::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QLineEdit::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "string", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"TextChanged", 1, param_slot_0 };
    static const QUMethod slot_1 = {"UpdateEnableState", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "TextChanged(const QString&)", &slot_0, QMetaData::Protected },
	{ "UpdateEnableState()", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"ADN_EditLine_ABC", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ADN_EditLine_ABC.setMetaObject( metaObj );
    return metaObj;
}

void* ADN_EditLine_ABC::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ADN_EditLine_ABC" ) )
	return this;
    if ( !qstrcmp( clname, "ADN_Gfx_ABC" ) )
	return (ADN_Gfx_ABC*)this;
    return QLineEdit::qt_cast( clname );
}

bool ADN_EditLine_ABC::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: TextChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: UpdateEnableState(); break;
    default:
	return QLineEdit::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ADN_EditLine_ABC::qt_emit( int _id, QUObject* _o )
{
    return QLineEdit::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ADN_EditLine_ABC::qt_property( int id, int f, QVariant* v)
{
    return QLineEdit::qt_property( id, f, v);
}

bool ADN_EditLine_ABC::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
