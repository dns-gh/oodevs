/****************************************************************************
** MOS_AgentMission meta object code from reading C++ file 'MOS_AgentMission.h'
**
** Created: jeu. 21. juil. 13:57:07 2005
**      by: The Qt MOC ($Id: $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "MOS_AgentMission.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MOS_AgentMission::className() const
{
    return "MOS_AgentMission";
}

QMetaObject *MOS_AgentMission::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_AgentMission( "MOS_AgentMission", &MOS_AgentMission::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_AgentMission::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_AgentMission", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_AgentMission::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_AgentMission", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_AgentMission::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"MOS_AgentMission", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_AgentMission.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_AgentMission::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_AgentMission" ) )
	return this;
    if ( !qstrcmp( clname, "MOS_MissionTool_ABC" ) )
	return (MOS_MissionTool_ABC*)this;
    return QWidget::qt_cast( clname );
}

bool MOS_AgentMission::qt_invoke( int _id, QUObject* _o )
{
    return QWidget::qt_invoke(_id,_o);
}

bool MOS_AgentMission::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_AgentMission::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool MOS_AgentMission::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
