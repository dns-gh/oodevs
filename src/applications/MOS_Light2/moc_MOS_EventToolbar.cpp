/****************************************************************************
** MOS_EventToolbar meta object code from reading C++ file 'MOS_EventToolbar.h'
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

const char *MOS_EventToolbar::className() const
{
    return "MOS_EventToolbar";
}

QMetaObject *MOS_EventToolbar::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_EventToolbar( "MOS_EventToolbar", &MOS_EventToolbar::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_EventToolbar::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_EventToolbar", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_EventToolbar::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_EventToolbar", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_EventToolbar::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QToolBar::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "popupMenu", &static_QUType_ptr, "QPopupMenu", QUParameter::InOut },
	{ "context", &static_QUType_ptr, "MOS_ActionContext", QUParameter::In }
    };
    static const QUMethod slot_0 = {"FillRemotePopup", 2, param_slot_0 };
    static const QUMethod slot_1 = {"SubscribeToAgent", 0, 0 };
    static const QUMethod slot_2 = {"UnsubscribeFromAgent", 0, 0 };
    static const QUMethod slot_3 = {"OnTeamChanged", 0, 0 };
    static const QUMethod slot_4 = {"ClearSubscriptions", 0, 0 };
    static const QUParameter param_slot_5[] = {
	{ "agent", &static_QUType_ptr, "MOS_Agent", QUParameter::InOut }
    };
    static const QUMethod slot_5 = {"OnAgentOutOfGas", 1, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ "agent", &static_QUType_ptr, "MOS_Agent", QUParameter::InOut }
    };
    static const QUMethod slot_6 = {"OnAgentRefueled", 1, param_slot_6 };
    static const QUParameter param_slot_7[] = {
	{ "agent", &static_QUType_ptr, "MOS_Agent", QUParameter::InOut }
    };
    static const QUMethod slot_7 = {"OnConflictStarted", 1, param_slot_7 };
    static const QUParameter param_slot_8[] = {
	{ "agent", &static_QUType_ptr, "MOS_Agent", QUParameter::InOut }
    };
    static const QUMethod slot_8 = {"OnConflictEnded", 1, param_slot_8 };
    static const QUParameter param_slot_9[] = {
	{ "agent", &static_QUType_ptr, "MOS_Agent", QUParameter::InOut }
    };
    static const QUMethod slot_9 = {"OnReportCreated", 1, param_slot_9 };
    static const QUParameter param_slot_10[] = {
	{ "agent", &static_QUType_ptr, "MOS_Agent", QUParameter::InOut }
    };
    static const QUMethod slot_10 = {"OnReadingReports", 1, param_slot_10 };
    static const QUParameter param_slot_11[] = {
	{ "nId", &static_QUType_int, 0, QUParameter::In },
	{ "bCenter", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_11 = {"FocusOnAgent", 2, param_slot_11 };
    static const QMetaData slot_tbl[] = {
	{ "FillRemotePopup(QPopupMenu&,const MOS_ActionContext&)", &slot_0, QMetaData::Private },
	{ "SubscribeToAgent()", &slot_1, QMetaData::Private },
	{ "UnsubscribeFromAgent()", &slot_2, QMetaData::Private },
	{ "OnTeamChanged()", &slot_3, QMetaData::Private },
	{ "ClearSubscriptions()", &slot_4, QMetaData::Private },
	{ "OnAgentOutOfGas(MOS_Agent&)", &slot_5, QMetaData::Private },
	{ "OnAgentRefueled(MOS_Agent&)", &slot_6, QMetaData::Private },
	{ "OnConflictStarted(MOS_Agent&)", &slot_7, QMetaData::Private },
	{ "OnConflictEnded(MOS_Agent&)", &slot_8, QMetaData::Private },
	{ "OnReportCreated(MOS_Agent&)", &slot_9, QMetaData::Private },
	{ "OnReadingReports(MOS_Agent&)", &slot_10, QMetaData::Private },
	{ "FocusOnAgent(int,bool)", &slot_11, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ "nEventId", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"AgentOutOfGas", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ "nEventId", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_1 = {"AgentRefueled", 1, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ "nEventId", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_2 = {"ConflictStarted", 1, param_signal_2 };
    static const QUParameter param_signal_3[] = {
	{ "nEventId", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_3 = {"ConflictEnded", 1, param_signal_3 };
    static const QUParameter param_signal_4[] = {
	{ "nEventId", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_4 = {"ReportCreated", 1, param_signal_4 };
    static const QUParameter param_signal_5[] = {
	{ "nEventId", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_5 = {"ReadingReports", 1, param_signal_5 };
    static const QUParameter param_signal_6[] = {
	{ "selectedElement", &static_QUType_ptr, "MOS_SelectedElement", QUParameter::InOut }
    };
    static const QUMethod signal_6 = {"ElementSelected", 1, param_signal_6 };
    static const QUParameter param_signal_7[] = {
	{ "vPoint", &static_QUType_ptr, "MT_Vector2D", QUParameter::In }
    };
    static const QUMethod signal_7 = {"CenterOnPoint", 1, param_signal_7 };
    static const QMetaData signal_tbl[] = {
	{ "AgentOutOfGas(int)", &signal_0, QMetaData::Private },
	{ "AgentRefueled(int)", &signal_1, QMetaData::Private },
	{ "ConflictStarted(int)", &signal_2, QMetaData::Private },
	{ "ConflictEnded(int)", &signal_3, QMetaData::Private },
	{ "ReportCreated(int)", &signal_4, QMetaData::Private },
	{ "ReadingReports(int)", &signal_5, QMetaData::Private },
	{ "ElementSelected(MOS_SelectedElement&)", &signal_6, QMetaData::Private },
	{ "CenterOnPoint(const MT_Vector2D&)", &signal_7, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_EventToolbar", parentObject,
	slot_tbl, 12,
	signal_tbl, 8,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_EventToolbar.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_EventToolbar::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_EventToolbar" ) )
	return this;
    return QToolBar::qt_cast( clname );
}

// SIGNAL AgentOutOfGas
void MOS_EventToolbar::AgentOutOfGas( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

// SIGNAL AgentRefueled
void MOS_EventToolbar::AgentRefueled( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 1, t0 );
}

// SIGNAL ConflictStarted
void MOS_EventToolbar::ConflictStarted( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 2, t0 );
}

// SIGNAL ConflictEnded
void MOS_EventToolbar::ConflictEnded( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 3, t0 );
}

// SIGNAL ReportCreated
void MOS_EventToolbar::ReportCreated( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 4, t0 );
}

// SIGNAL ReadingReports
void MOS_EventToolbar::ReadingReports( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 5, t0 );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL ElementSelected
void MOS_EventToolbar::ElementSelected( MOS_SelectedElement& t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 6 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL CenterOnPoint
void MOS_EventToolbar::CenterOnPoint( const MT_Vector2D& t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 7 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

bool MOS_EventToolbar::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: FillRemotePopup((QPopupMenu&)*((QPopupMenu*)static_QUType_ptr.get(_o+1)),(const MOS_ActionContext&)*((const MOS_ActionContext*)static_QUType_ptr.get(_o+2))); break;
    case 1: SubscribeToAgent(); break;
    case 2: UnsubscribeFromAgent(); break;
    case 3: OnTeamChanged(); break;
    case 4: ClearSubscriptions(); break;
    case 5: OnAgentOutOfGas((MOS_Agent&)*((MOS_Agent*)static_QUType_ptr.get(_o+1))); break;
    case 6: OnAgentRefueled((MOS_Agent&)*((MOS_Agent*)static_QUType_ptr.get(_o+1))); break;
    case 7: OnConflictStarted((MOS_Agent&)*((MOS_Agent*)static_QUType_ptr.get(_o+1))); break;
    case 8: OnConflictEnded((MOS_Agent&)*((MOS_Agent*)static_QUType_ptr.get(_o+1))); break;
    case 9: OnReportCreated((MOS_Agent&)*((MOS_Agent*)static_QUType_ptr.get(_o+1))); break;
    case 10: OnReadingReports((MOS_Agent&)*((MOS_Agent*)static_QUType_ptr.get(_o+1))); break;
    case 11: FocusOnAgent((int)static_QUType_int.get(_o+1),(bool)static_QUType_bool.get(_o+2)); break;
    default:
	return QToolBar::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_EventToolbar::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: AgentOutOfGas((int)static_QUType_int.get(_o+1)); break;
    case 1: AgentRefueled((int)static_QUType_int.get(_o+1)); break;
    case 2: ConflictStarted((int)static_QUType_int.get(_o+1)); break;
    case 3: ConflictEnded((int)static_QUType_int.get(_o+1)); break;
    case 4: ReportCreated((int)static_QUType_int.get(_o+1)); break;
    case 5: ReadingReports((int)static_QUType_int.get(_o+1)); break;
    case 6: ElementSelected((MOS_SelectedElement&)*((MOS_SelectedElement*)static_QUType_ptr.get(_o+1))); break;
    case 7: CenterOnPoint((const MT_Vector2D&)*((const MT_Vector2D*)static_QUType_ptr.get(_o+1))); break;
    default:
	return QToolBar::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool MOS_EventToolbar::qt_property( int id, int f, QVariant* v)
{
    return QToolBar::qt_property( id, f, v);
}

bool MOS_EventToolbar::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
