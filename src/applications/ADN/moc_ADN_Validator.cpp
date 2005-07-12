/****************************************************************************
** ADN_IntValidator meta object code from reading C++ file 'ADN_Validator.h'
**
** Created: mer. 13. avr. 10:59:33 2005
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

#include <qvariant.h>
const char *ADN_IntValidator::className() const
{
    return "ADN_IntValidator";
}

QMetaObject *ADN_IntValidator::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ADN_IntValidator( "ADN_IntValidator", &ADN_IntValidator::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ADN_IntValidator::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_IntValidator", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ADN_IntValidator::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_IntValidator", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ADN_IntValidator::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QValidator::staticMetaObject();
#ifndef QT_NO_PROPERTIES
    static const QMetaProperty props_tbl[2] = {
 	{ "int","bottom", 0x10000103, &ADN_IntValidator::metaObj, 0, -1 },
	{ "int","top", 0x10000103, &ADN_IntValidator::metaObj, 0, -1 }
    };
#endif // QT_NO_PROPERTIES
    metaObj = QMetaObject::new_metaobject(
	"ADN_IntValidator", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	props_tbl, 2,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ADN_IntValidator.setMetaObject( metaObj );
    return metaObj;
}

void* ADN_IntValidator::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ADN_IntValidator" ) )
	return this;
    return QValidator::qt_cast( clname );
}

bool ADN_IntValidator::qt_invoke( int _id, QUObject* _o )
{
    return QValidator::qt_invoke(_id,_o);
}

bool ADN_IntValidator::qt_emit( int _id, QUObject* _o )
{
    return QValidator::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ADN_IntValidator::qt_property( int id, int f, QVariant* v)
{
    switch ( id - staticMetaObject()->propertyOffset() ) {
    case 0: switch( f ) {
	case 0: setBottom(v->asInt()); break;
	case 1: *v = QVariant( this->bottom() ); break;
	case 3: case 4: case 5: break;
	default: return FALSE;
    } break;
    case 1: switch( f ) {
	case 0: setTop(v->asInt()); break;
	case 1: *v = QVariant( this->top() ); break;
	case 3: case 4: case 5: break;
	default: return FALSE;
    } break;
    default:
	return QValidator::qt_property( id, f, v );
    }
    return TRUE;
}

bool ADN_IntValidator::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *ADN_DoubleValidator::className() const
{
    return "ADN_DoubleValidator";
}

QMetaObject *ADN_DoubleValidator::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ADN_DoubleValidator( "ADN_DoubleValidator", &ADN_DoubleValidator::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ADN_DoubleValidator::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_DoubleValidator", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ADN_DoubleValidator::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_DoubleValidator", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ADN_DoubleValidator::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QValidator::staticMetaObject();
#ifndef QT_NO_PROPERTIES
    static const QMetaProperty props_tbl[3] = {
 	{ "double","bottom", 0x13000103, &ADN_DoubleValidator::metaObj, 0, -1 },
	{ "double","top", 0x13000103, &ADN_DoubleValidator::metaObj, 0, -1 },
	{ "int","decimals", 0x10000103, &ADN_DoubleValidator::metaObj, 0, -1 }
    };
#endif // QT_NO_PROPERTIES
    metaObj = QMetaObject::new_metaobject(
	"ADN_DoubleValidator", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	props_tbl, 3,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ADN_DoubleValidator.setMetaObject( metaObj );
    return metaObj;
}

void* ADN_DoubleValidator::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ADN_DoubleValidator" ) )
	return this;
    return QValidator::qt_cast( clname );
}

bool ADN_DoubleValidator::qt_invoke( int _id, QUObject* _o )
{
    return QValidator::qt_invoke(_id,_o);
}

bool ADN_DoubleValidator::qt_emit( int _id, QUObject* _o )
{
    return QValidator::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ADN_DoubleValidator::qt_property( int id, int f, QVariant* v)
{
    switch ( id - staticMetaObject()->propertyOffset() ) {
    case 0: switch( f ) {
	case 0: setBottom(v->asDouble()); break;
	case 1: *v = QVariant( this->bottom() ); break;
	case 3: case 4: case 5: break;
	default: return FALSE;
    } break;
    case 1: switch( f ) {
	case 0: setTop(v->asDouble()); break;
	case 1: *v = QVariant( this->top() ); break;
	case 3: case 4: case 5: break;
	default: return FALSE;
    } break;
    case 2: switch( f ) {
	case 0: setDecimals(v->asInt()); break;
	case 1: *v = QVariant( this->decimals() ); break;
	case 3: case 4: case 5: break;
	default: return FALSE;
    } break;
    default:
	return QValidator::qt_property( id, f, v );
    }
    return TRUE;
}

bool ADN_DoubleValidator::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
