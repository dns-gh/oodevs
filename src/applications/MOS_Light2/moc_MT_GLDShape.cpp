/****************************************************************************
** MT_GLDShape meta object code from reading C++ file 'MT_GLDShape.h'
**
** Created: ven. 22. juil. 13:35:02 2005
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

const char *MT_GLDShape::className() const
{
    return "MT_GLDShape";
}

QMetaObject *MT_GLDShape::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MT_GLDShape( "MT_GLDShape", &MT_GLDShape::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MT_GLDShape::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MT_GLDShape", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MT_GLDShape::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MT_GLDShape", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MT_GLDShape::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = MT_GLDShape_ABC::staticMetaObject();
    static const QUMethod signal_0 = {"CreationDone", 0, 0 };
    static const QUMethod signal_1 = {"CreationCanceled", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "CreationDone()", &signal_0, QMetaData::Public },
	{ "CreationCanceled()", &signal_1, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"MT_GLDShape", parentObject,
	0, 0,
	signal_tbl, 2,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MT_GLDShape.setMetaObject( metaObj );
    return metaObj;
}

void* MT_GLDShape::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MT_GLDShape" ) )
	return this;
    return MT_GLDShape_ABC::qt_cast( clname );
}

// SIGNAL CreationDone
void MT_GLDShape::CreationDone()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

// SIGNAL CreationCanceled
void MT_GLDShape::CreationCanceled()
{
    activate_signal( staticMetaObject()->signalOffset() + 1 );
}

bool MT_GLDShape::qt_invoke( int _id, QUObject* _o )
{
    return MT_GLDShape_ABC::qt_invoke(_id,_o);
}

bool MT_GLDShape::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: CreationDone(); break;
    case 1: CreationCanceled(); break;
    default:
	return MT_GLDShape_ABC::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool MT_GLDShape::qt_property( int id, int f, QVariant* v)
{
    return MT_GLDShape_ABC::qt_property( id, f, v);
}

bool MT_GLDShape::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
