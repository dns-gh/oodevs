/****************************************************************************
** ADN_GUI_ABC meta object code from reading C++ file 'ADN_GUI_ABC.h'
**
** Created: jeu. 7. avr. 11:52:41 2005
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

const char *ADN_GUI_ABC::className() const
{
    return "ADN_GUI_ABC";
}

QMetaObject *ADN_GUI_ABC::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ADN_GUI_ABC( "ADN_GUI_ABC", &ADN_GUI_ABC::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ADN_GUI_ABC::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_GUI_ABC", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ADN_GUI_ABC::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_GUI_ABC", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ADN_GUI_ABC::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"ADN_GUI_ABC", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ADN_GUI_ABC.setMetaObject( metaObj );
    return metaObj;
}

void* ADN_GUI_ABC::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ADN_GUI_ABC" ) )
	return this;
    return QObject::qt_cast( clname );
}

bool ADN_GUI_ABC::qt_invoke( int _id, QUObject* _o )
{
    return QObject::qt_invoke(_id,_o);
}

bool ADN_GUI_ABC::qt_emit( int _id, QUObject* _o )
{
    return QObject::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ADN_GUI_ABC::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool ADN_GUI_ABC::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
