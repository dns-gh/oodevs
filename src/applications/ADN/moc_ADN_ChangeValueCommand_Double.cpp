/****************************************************************************
** ADN_ChangeValueCommand_Double meta object code from reading C++ file 'ADN_ChangeValueCommand_Double.h'
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

const char *ADN_ChangeValueCommand_Double::className() const
{
    return "ADN_ChangeValueCommand_Double";
}

QMetaObject *ADN_ChangeValueCommand_Double::metaObj = 0;
static QMetaObjectCleanUp cleanUp_ADN_ChangeValueCommand_Double( "ADN_ChangeValueCommand_Double", &ADN_ChangeValueCommand_Double::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString ADN_ChangeValueCommand_Double::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_ChangeValueCommand_Double", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString ADN_ChangeValueCommand_Double::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "ADN_ChangeValueCommand_Double", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* ADN_ChangeValueCommand_Double::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QtCommand::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"ADN_ChangeValueCommand_Double", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_ADN_ChangeValueCommand_Double.setMetaObject( metaObj );
    return metaObj;
}

void* ADN_ChangeValueCommand_Double::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "ADN_ChangeValueCommand_Double" ) )
	return this;
    return QtCommand::qt_cast( clname );
}

bool ADN_ChangeValueCommand_Double::qt_invoke( int _id, QUObject* _o )
{
    return QtCommand::qt_invoke(_id,_o);
}

bool ADN_ChangeValueCommand_Double::qt_emit( int _id, QUObject* _o )
{
    return QtCommand::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool ADN_ChangeValueCommand_Double::qt_property( int id, int f, QVariant* v)
{
    return QtCommand::qt_property( id, f, v);
}

bool ADN_ChangeValueCommand_Double::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
