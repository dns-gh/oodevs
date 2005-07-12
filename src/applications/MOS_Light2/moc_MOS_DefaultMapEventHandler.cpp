/****************************************************************************
** MOS_DefaultMapEventHandler meta object code from reading C++ file 'MOS_DefaultMapEventHandler.h'
**
** Created: mar. 12. juil. 12:42:03 2005
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

const char *MOS_DefaultMapEventHandler::className() const
{
    return "MOS_DefaultMapEventHandler";
}

QMetaObject *MOS_DefaultMapEventHandler::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_DefaultMapEventHandler( "MOS_DefaultMapEventHandler", &MOS_DefaultMapEventHandler::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_DefaultMapEventHandler::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_DefaultMapEventHandler", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_DefaultMapEventHandler::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_DefaultMapEventHandler", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_DefaultMapEventHandler::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "selectedElement", &static_QUType_ptr, "MOS_SelectedElement", QUParameter::InOut }
    };
    static const QUMethod slot_0 = {"SetSelectedElement", 1, param_slot_0 };
    static const QUMethod slot_1 = {"ClearSelection", 0, 0 };
    static const QUParameter param_slot_2[] = {
	{ "object", &static_QUType_ptr, "MOS_DynaObject", QUParameter::InOut }
    };
    static const QUMethod slot_2 = {"OnDynaObjectDeleted", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "line", &static_QUType_ptr, "MOS_TacticalLine_ABC", QUParameter::InOut }
    };
    static const QUMethod slot_3 = {"OnTacticalLineDeleted", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "gtia", &static_QUType_ptr, "MOS_Gtia", QUParameter::InOut },
	{ "knowledge", &static_QUType_ptr, "MOS_AgentKnowledge", QUParameter::InOut }
    };
    static const QUMethod slot_4 = {"OnAgentKnowledgeDeleted", 2, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "team", &static_QUType_ptr, "MOS_Team", QUParameter::InOut },
	{ "knowledge", &static_QUType_ptr, "MOS_DynaObjectKnowledge", QUParameter::InOut }
    };
    static const QUMethod slot_5 = {"OnObjectKnowledgeDeleted", 2, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ "nType", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"SwitchToLineCreation", 1, param_slot_6 };
    static const QUMethod slot_7 = {"LineCreated", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "SetSelectedElement(MOS_SelectedElement&)", &slot_0, QMetaData::Public },
	{ "ClearSelection()", &slot_1, QMetaData::Public },
	{ "OnDynaObjectDeleted(MOS_DynaObject&)", &slot_2, QMetaData::Public },
	{ "OnTacticalLineDeleted(MOS_TacticalLine_ABC&)", &slot_3, QMetaData::Public },
	{ "OnAgentKnowledgeDeleted(MOS_Gtia&,MOS_AgentKnowledge&)", &slot_4, QMetaData::Public },
	{ "OnObjectKnowledgeDeleted(MOS_Team&,MOS_DynaObjectKnowledge&)", &slot_5, QMetaData::Public },
	{ "SwitchToLineCreation(int)", &slot_6, QMetaData::Public },
	{ "LineCreated()", &slot_7, QMetaData::Public }
    };
    static const QUParameter param_signal_0[] = {
	{ "selectedElement", &static_QUType_ptr, "MOS_SelectedElement", QUParameter::InOut }
    };
    static const QUMethod signal_0 = {"ElementSelected", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ "popupMenu", &static_QUType_ptr, "QPopupMenu", QUParameter::InOut },
	{ "context", &static_QUType_ptr, "MOS_ActionContext", QUParameter::In }
    };
    static const QUMethod signal_1 = {"NewPopupMenu", 2, param_signal_1 };
    static const QMetaData signal_tbl[] = {
	{ "ElementSelected(MOS_SelectedElement&)", &signal_0, QMetaData::Private },
	{ "NewPopupMenu(QPopupMenu&,const MOS_ActionContext&)", &signal_1, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_DefaultMapEventHandler", parentObject,
	slot_tbl, 8,
	signal_tbl, 2,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_DefaultMapEventHandler.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_DefaultMapEventHandler::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_DefaultMapEventHandler" ) )
	return this;
    if ( !qstrcmp( clname, "MOS_MapEventFilter_ABC" ) )
	return (MOS_MapEventFilter_ABC*)this;
    return QObject::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL ElementSelected
void MOS_DefaultMapEventHandler::ElementSelected( MOS_SelectedElement& t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL NewPopupMenu
void MOS_DefaultMapEventHandler::NewPopupMenu( QPopupMenu& t0, const MOS_ActionContext& t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 1 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_ptr.set(o+1,&t0);
    static_QUType_ptr.set(o+2,&t1);
    activate_signal( clist, o );
}

bool MOS_DefaultMapEventHandler::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SetSelectedElement((MOS_SelectedElement&)*((MOS_SelectedElement*)static_QUType_ptr.get(_o+1))); break;
    case 1: ClearSelection(); break;
    case 2: OnDynaObjectDeleted((MOS_DynaObject&)*((MOS_DynaObject*)static_QUType_ptr.get(_o+1))); break;
    case 3: OnTacticalLineDeleted((MOS_TacticalLine_ABC&)*((MOS_TacticalLine_ABC*)static_QUType_ptr.get(_o+1))); break;
    case 4: OnAgentKnowledgeDeleted((MOS_Gtia&)*((MOS_Gtia*)static_QUType_ptr.get(_o+1)),(MOS_AgentKnowledge&)*((MOS_AgentKnowledge*)static_QUType_ptr.get(_o+2))); break;
    case 5: OnObjectKnowledgeDeleted((MOS_Team&)*((MOS_Team*)static_QUType_ptr.get(_o+1)),(MOS_DynaObjectKnowledge&)*((MOS_DynaObjectKnowledge*)static_QUType_ptr.get(_o+2))); break;
    case 6: SwitchToLineCreation((int)static_QUType_int.get(_o+1)); break;
    case 7: LineCreated(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_DefaultMapEventHandler::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: ElementSelected((MOS_SelectedElement&)*((MOS_SelectedElement*)static_QUType_ptr.get(_o+1))); break;
    case 1: NewPopupMenu((QPopupMenu&)*((QPopupMenu*)static_QUType_ptr.get(_o+1)),(const MOS_ActionContext&)*((const MOS_ActionContext*)static_QUType_ptr.get(_o+2))); break;
    default:
	return QObject::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool MOS_DefaultMapEventHandler::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool MOS_DefaultMapEventHandler::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
