/****************************************************************************
** MOS_AgentListView meta object code from reading C++ file 'MOS_AgentListView.h'
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

const char *MOS_AgentListView::className() const
{
    return "MOS_AgentListView";
}

QMetaObject *MOS_AgentListView::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_AgentListView( "MOS_AgentListView", &MOS_AgentListView::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_AgentListView::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_AgentListView", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_AgentListView::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_AgentListView", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_AgentListView::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QListView::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "agent", &static_QUType_ptr, "MOS_Agent", QUParameter::InOut }
    };
    static const QUMethod slot_0 = {"AddAgent", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "gtia", &static_QUType_ptr, "MOS_Gtia", QUParameter::InOut }
    };
    static const QUMethod slot_1 = {"AddGtia", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "selectedElement", &static_QUType_ptr, "MOS_SelectedElement", QUParameter::InOut }
    };
    static const QUMethod slot_2 = {"SetSelectedElement", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "agent", &static_QUType_ptr, "MOS_Agent", QUParameter::InOut }
    };
    static const QUMethod slot_3 = {"OnAgentUpdated", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "pItem", &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ "pos", &static_QUType_varptr, "\x0e", QUParameter::In },
	{ "nCol", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"OnRequestPopup", 3, param_slot_4 };
    static const QUMethod slot_5 = {"OnRequestCenter", 0, 0 };
    static const QUParameter param_slot_6[] = {
	{ "pItem", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_6 = {"OnSelectionChange", 1, param_slot_6 };
    static const QUMethod slot_7 = {"OnTeamChanged", 0, 0 };
    static const QUParameter param_slot_8[] = {
	{ "agent", &static_QUType_ptr, "MOS_Agent", QUParameter::InOut }
    };
    static const QUMethod slot_8 = {"OnAgentReparented", 1, param_slot_8 };
    static const QMetaData slot_tbl[] = {
	{ "AddAgent(MOS_Agent&)", &slot_0, QMetaData::Public },
	{ "AddGtia(MOS_Gtia&)", &slot_1, QMetaData::Public },
	{ "SetSelectedElement(MOS_SelectedElement&)", &slot_2, QMetaData::Public },
	{ "OnAgentUpdated(MOS_Agent&)", &slot_3, QMetaData::Public },
	{ "OnRequestPopup(QListViewItem*,const QPoint&,int)", &slot_4, QMetaData::Private },
	{ "OnRequestCenter()", &slot_5, QMetaData::Private },
	{ "OnSelectionChange(QListViewItem*)", &slot_6, QMetaData::Private },
	{ "OnTeamChanged()", &slot_7, QMetaData::Private },
	{ "OnAgentReparented(MOS_Agent&)", &slot_8, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ "vPoint", &static_QUType_ptr, "MT_Vector2D", QUParameter::In }
    };
    static const QUMethod signal_0 = {"CenterOnPoint", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ "selectedElement", &static_QUType_ptr, "MOS_SelectedElement", QUParameter::InOut }
    };
    static const QUMethod signal_1 = {"ElementSelected", 1, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ "popupMenu", &static_QUType_ptr, "QPopupMenu", QUParameter::InOut },
	{ "context", &static_QUType_ptr, "MOS_ActionContext", QUParameter::In }
    };
    static const QUMethod signal_2 = {"NewPopupMenu", 2, param_signal_2 };
    static const QMetaData signal_tbl[] = {
	{ "CenterOnPoint(const MT_Vector2D&)", &signal_0, QMetaData::Private },
	{ "ElementSelected(MOS_SelectedElement&)", &signal_1, QMetaData::Private },
	{ "NewPopupMenu(QPopupMenu&,const MOS_ActionContext&)", &signal_2, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_AgentListView", parentObject,
	slot_tbl, 9,
	signal_tbl, 3,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_AgentListView.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_AgentListView::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_AgentListView" ) )
	return this;
    return QListView::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL CenterOnPoint
void MOS_AgentListView::CenterOnPoint( const MT_Vector2D& t0 )
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

// SIGNAL ElementSelected
void MOS_AgentListView::ElementSelected( MOS_SelectedElement& t0 )
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

// SIGNAL NewPopupMenu
void MOS_AgentListView::NewPopupMenu( QPopupMenu& t0, const MOS_ActionContext& t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 2 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_ptr.set(o+1,&t0);
    static_QUType_ptr.set(o+2,&t1);
    activate_signal( clist, o );
}

bool MOS_AgentListView::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: AddAgent((MOS_Agent&)*((MOS_Agent*)static_QUType_ptr.get(_o+1))); break;
    case 1: AddGtia((MOS_Gtia&)*((MOS_Gtia*)static_QUType_ptr.get(_o+1))); break;
    case 2: SetSelectedElement((MOS_SelectedElement&)*((MOS_SelectedElement*)static_QUType_ptr.get(_o+1))); break;
    case 3: OnAgentUpdated((MOS_Agent&)*((MOS_Agent*)static_QUType_ptr.get(_o+1))); break;
    case 4: OnRequestPopup((QListViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2)),(int)static_QUType_int.get(_o+3)); break;
    case 5: OnRequestCenter(); break;
    case 6: OnSelectionChange((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 7: OnTeamChanged(); break;
    case 8: OnAgentReparented((MOS_Agent&)*((MOS_Agent*)static_QUType_ptr.get(_o+1))); break;
    default:
	return QListView::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_AgentListView::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: CenterOnPoint((const MT_Vector2D&)*((const MT_Vector2D*)static_QUType_ptr.get(_o+1))); break;
    case 1: ElementSelected((MOS_SelectedElement&)*((MOS_SelectedElement*)static_QUType_ptr.get(_o+1))); break;
    case 2: NewPopupMenu((QPopupMenu&)*((QPopupMenu*)static_QUType_ptr.get(_o+1)),(const MOS_ActionContext&)*((const MOS_ActionContext*)static_QUType_ptr.get(_o+2))); break;
    default:
	return QListView::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool MOS_AgentListView::qt_property( int id, int f, QVariant* v)
{
    return QListView::qt_property( id, f, v);
}

bool MOS_AgentListView::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
