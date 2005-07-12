/****************************************************************************
** ADN_ChangeValueCommand_String meta object code from reading C++ file 'ADN_ChangeValueCommand_String.h'
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

const char *ADN_ChangeValueCommand_String::className() const
{
    return "ADN_ChangeValueCommand_String";
}

QMetaObject *ADN_ChangeValueCommand_String::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ADN_ChangeValueCommand_String( "ADN_ChangeValueCommand_String", &ADN_ChangeValueCommand_String::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ADN_ChangeValueCommand_String::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_ChangeValueCommand_String", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ADN_ChangeValueCommand_String::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_ChangeValueCommand_String", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ADN_ChangeValueCommand_String::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QtCommand::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"ADN_ChangeValueCommand_String", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ADN_ChangeValueCommand_String.setMetaObject( metaObj );
    return metaObj;
}

void* ADN_ChangeValueCommand_String::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ADN_ChangeValueCommand_String" ) )
	return this;
    return QtCommand::qt_cast( clname );
}

bool ADN_ChangeValueCommand_String::qt_invoke( int _id, QUObject* _o )
{
    return QtCommand::qt_invoke(_id,_o);
}

bool ADN_ChangeValueCommand_String::qt_emit( int _id, QUObject* _o )
{
    return QtCommand::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ADN_ChangeValueCommand_String::qt_property( int id, int f, QVariant* v)
{
    return QtCommand::qt_property( id, f, v);
}

bool ADN_ChangeValueCommand_String::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
