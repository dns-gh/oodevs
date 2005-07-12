/****************************************************************************
** MOS_LimaMission meta object code from reading C++ file 'MOS_LimaMission.h'
**
** Created: lun. 11. juil. 18:50:12 2005
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

const char *MOS_LimaMission::className() const
{
    return "MOS_LimaMission";
}

QMetaObject *MOS_LimaMission::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_LimaMission( "MOS_LimaMission", &MOS_LimaMission::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_LimaMission::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_LimaMission", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_LimaMission::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_LimaMission", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_LimaMission::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QFrame::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"MOS_LimaMission", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_LimaMission.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_LimaMission::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_LimaMission" ) )
	return this;
    if ( !qstrcmp( clname, "MOS_MissionTool_ABC" ) )
	return (MOS_MissionTool_ABC*)this;
    return QFrame::qt_cast( clname );
}

bool MOS_LimaMission::qt_invoke( int _id, QUObject* _o )
{
    return QFrame::qt_invoke(_id,_o);
}

bool MOS_LimaMission::qt_emit( int _id, QUObject* _o )
{
    return QFrame::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_LimaMission::qt_property( int id, int f, QVariant* v)
{
    return QFrame::qt_property( id, f, v);
}

bool MOS_LimaMission::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
