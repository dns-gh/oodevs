/****************************************************************************
** MOS_MissionAction meta object code from reading C++ file 'MOS_MissionAction.h'
**
** Created: lun. 11. juil. 18:50:11 2005
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

const char *MOS_MissionAction::className() const
{
    return "MOS_MissionAction";
}

QMetaObject *MOS_MissionAction::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_MissionAction( "MOS_MissionAction", &MOS_MissionAction::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_MissionAction::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_MissionAction", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_MissionAction::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_MissionAction", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_MissionAction::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QAction::staticMetaObject();
    static const QUMethod slot_0 = {"SlotSelectMission", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "SlotSelectMission()", &slot_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_MissionAction", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_MissionAction.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_MissionAction::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_MissionAction" ) )
	return this;
    return QAction::qt_cast( clname );
}

bool MOS_MissionAction::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SlotSelectMission(); break;
    default:
	return QAction::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_MissionAction::qt_emit( int _id, QUObject* _o )
{
    return QAction::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_MissionAction::qt_property( int id, int f, QVariant* v)
{
    return QAction::qt_property( id, f, v);
}

bool MOS_MissionAction::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
