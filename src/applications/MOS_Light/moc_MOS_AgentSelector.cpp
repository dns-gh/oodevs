/****************************************************************************
** MOS_AgentSelector meta object code from reading C++ file 'MOS_AgentSelector.h'
**
** Created: lun. 11. juil. 18:50:10 2005
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

const char *MOS_AgentSelector::className() const
{
    return "MOS_AgentSelector";
}

QMetaObject *MOS_AgentSelector::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_AgentSelector( "MOS_AgentSelector", &MOS_AgentSelector::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_AgentSelector::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_AgentSelector", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_AgentSelector::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_AgentSelector", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_AgentSelector::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QFrame::staticMetaObject();
    static const QUMethod slot_0 = {"SlotAccept", 0, 0 };
    static const QUMethod slot_1 = {"SlotReject", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "SlotAccept()", &slot_0, QMetaData::Protected },
	{ "SlotReject()", &slot_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_AgentSelector", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_AgentSelector.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_AgentSelector::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_AgentSelector" ) )
	return this;
    return QFrame::qt_cast( clname );
}

bool MOS_AgentSelector::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SlotAccept(); break;
    case 1: SlotReject(); break;
    default:
	return QFrame::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_AgentSelector::qt_emit( int _id, QUObject* _o )
{
    return QFrame::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MOS_AgentSelector::qt_property( int id, int f, QVariant* v)
{
    return QFrame::qt_property( id, f, v);
}

bool MOS_AgentSelector::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
