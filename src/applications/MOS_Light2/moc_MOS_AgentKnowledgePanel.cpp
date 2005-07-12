/****************************************************************************
** MOS_AgentKnowledgePanel meta object code from reading C++ file 'MOS_AgentKnowledgePanel.h'
**
** Created: mar. 12. juil. 12:42:02 2005
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

const char *MOS_AgentKnowledgePanel::className() const
{
    return "MOS_AgentKnowledgePanel";
}

QMetaObject *MOS_AgentKnowledgePanel::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_AgentKnowledgePanel( "MOS_AgentKnowledgePanel", &MOS_AgentKnowledgePanel::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_AgentKnowledgePanel::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_AgentKnowledgePanel", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_AgentKnowledgePanel::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_AgentKnowledgePanel", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_AgentKnowledgePanel::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = MOS_InfoPanel_ABC::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "pItem", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_0 = {"OnSelectionChanged", 1, param_slot_0 };
    static const QUMethod slot_1 = {"OnRequestCenter", 0, 0 };
    static const QUParameter param_slot_2[] = {
	{ "gtia", &static_QUType_ptr, "MOS_Gtia", QUParameter::InOut },
	{ "knowledge", &static_QUType_ptr, "MOS_AgentKnowledge", QUParameter::InOut }
    };
    static const QUMethod slot_2 = {"OnKnowledgeCreated", 2, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "gtia", &static_QUType_ptr, "MOS_Gtia", QUParameter::InOut },
	{ "knowledge", &static_QUType_ptr, "MOS_AgentKnowledge", QUParameter::InOut }
    };
    static const QUMethod slot_3 = {"OnKnowledgeUpdated", 2, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "gtia", &static_QUType_ptr, "MOS_Gtia", QUParameter::InOut },
	{ "knowledge", &static_QUType_ptr, "MOS_AgentKnowledge", QUParameter::InOut }
    };
    static const QUMethod slot_4 = {"OnKnowledgeDeleted", 2, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "pItem", &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ "pos", &static_QUType_varptr, "\x0e", QUParameter::In }
    };
    static const QUMethod slot_5 = {"OnContextMenuRequested", 2, param_slot_5 };
    static const QUMethod slot_6 = {"ToggleDisplayOwnTeam", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "OnSelectionChanged(QListViewItem*)", &slot_0, QMetaData::Private },
	{ "OnRequestCenter()", &slot_1, QMetaData::Private },
	{ "OnKnowledgeCreated(MOS_Gtia&,MOS_AgentKnowledge&)", &slot_2, QMetaData::Private },
	{ "OnKnowledgeUpdated(MOS_Gtia&,MOS_AgentKnowledge&)", &slot_3, QMetaData::Private },
	{ "OnKnowledgeDeleted(MOS_Gtia&,MOS_AgentKnowledge&)", &slot_4, QMetaData::Private },
	{ "OnContextMenuRequested(QListViewItem*,const QPoint&)", &slot_5, QMetaData::Private },
	{ "ToggleDisplayOwnTeam()", &slot_6, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ "popupMenu", &static_QUType_ptr, "QPopupMenu", QUParameter::InOut },
	{ "action", &static_QUType_ptr, "MOS_ActionContext", QUParameter::In }
    };
    static const QUMethod signal_0 = {"NewPopupMenu", 2, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ "selectedElement", &static_QUType_ptr, "MOS_SelectedElement", QUParameter::InOut }
    };
    static const QUMethod signal_1 = {"ElementSelected", 1, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ "vPoint", &static_QUType_ptr, "MT_Vector2D", QUParameter::In }
    };
    static const QUMethod signal_2 = {"CenterOnPoint", 1, param_signal_2 };
    static const QMetaData signal_tbl[] = {
	{ "NewPopupMenu(QPopupMenu&,const MOS_ActionContext&)", &signal_0, QMetaData::Private },
	{ "ElementSelected(MOS_SelectedElement&)", &signal_1, QMetaData::Private },
	{ "CenterOnPoint(const MT_Vector2D&)", &signal_2, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_AgentKnowledgePanel", parentObject,
	slot_tbl, 7,
	signal_tbl, 3,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_AgentKnowledgePanel.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_AgentKnowledgePanel::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_AgentKnowledgePanel" ) )
	return this;
    return MOS_InfoPanel_ABC::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL NewPopupMenu
void MOS_AgentKnowledgePanel::NewPopupMenu( QPopupMenu& t0, const MOS_ActionContext& t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_ptr.set(o+1,&t0);
    static_QUType_ptr.set(o+2,&t1);
    activate_signal( clist, o );
}

// SIGNAL ElementSelected
void MOS_AgentKnowledgePanel::ElementSelected( MOS_SelectedElement& t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 1 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL CenterOnPoint
void MOS_AgentKnowledgePanel::CenterOnPoint( const MT_Vector2D& t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 2 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

bool MOS_AgentKnowledgePanel::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnSelectionChanged((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 1: OnRequestCenter(); break;
    case 2: OnKnowledgeCreated((MOS_Gtia&)*((MOS_Gtia*)static_QUType_ptr.get(_o+1)),(MOS_AgentKnowledge&)*((MOS_AgentKnowledge*)static_QUType_ptr.get(_o+2))); break;
    case 3: OnKnowledgeUpdated((MOS_Gtia&)*((MOS_Gtia*)static_QUType_ptr.get(_o+1)),(MOS_AgentKnowledge&)*((MOS_AgentKnowledge*)static_QUType_ptr.get(_o+2))); break;
    case 4: OnKnowledgeDeleted((MOS_Gtia&)*((MOS_Gtia*)static_QUType_ptr.get(_o+1)),(MOS_AgentKnowledge&)*((MOS_AgentKnowledge*)static_QUType_ptr.get(_o+2))); break;
    case 5: OnContextMenuRequested((QListViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2))); break;
    case 6: ToggleDisplayOwnTeam(); break;
    default:
	return MOS_InfoPanel_ABC::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_AgentKnowledgePanel::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: NewPopupMenu((QPopupMenu&)*((QPopupMenu*)static_QUType_ptr.get(_o+1)),(const MOS_ActionContext&)*((const MOS_ActionContext*)static_QUType_ptr.get(_o+2))); break;
    case 1: ElementSelected((MOS_SelectedElement&)*((MOS_SelectedElement*)static_QUType_ptr.get(_o+1))); break;
    case 2: CenterOnPoint((const MT_Vector2D&)*((const MT_Vector2D*)static_QUType_ptr.get(_o+1))); break;
    default:
	return MOS_InfoPanel_ABC::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool MOS_AgentKnowledgePanel::qt_property( int id, int f, QVariant* v)
{
    return MOS_InfoPanel_ABC::qt_property( id, f, v);
}

bool MOS_AgentKnowledgePanel::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
