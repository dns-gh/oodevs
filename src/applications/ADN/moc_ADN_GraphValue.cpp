/****************************************************************************
** ADN_GraphValue meta object code from reading C++ file 'ADN_GraphValue.h'
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

const char *ADN_GraphValue::className() const
{
    return "ADN_GraphValue";
}

QMetaObject *ADN_GraphValue::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ADN_GraphValue( "ADN_GraphValue", &ADN_GraphValue::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ADN_GraphValue::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_GraphValue", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ADN_GraphValue::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_GraphValue", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ADN_GraphValue::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"OnDataChanged", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "OnDataChanged()", &slot_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"ADN_GraphValue", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ADN_GraphValue.setMetaObject( metaObj );
    return metaObj;
}

void* ADN_GraphValue::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ADN_GraphValue" ) )
	return this;
    return QObject::qt_cast( clname );
}

bool ADN_GraphValue::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnDataChanged(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool ADN_GraphValue::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ADN_GraphValue::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool ADN_GraphValue::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
