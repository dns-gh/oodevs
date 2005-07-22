/****************************************************************************
** MOS_App meta object code from reading C++ file 'MOS_App.h'
**
** Created: ven. 22. juil. 13:35:03 2005
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

const char *MOS_App::className() const
{
    return "MOS_App";
}

QMetaObject *MOS_App::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MOS_App( "MOS_App", &MOS_App::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MOS_App::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_App", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MOS_App::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MOS_App", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MOS_App::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QApplication::staticMetaObject();
    static const QUMethod slot_0 = {"UpdateData", 0, 0 };
    static const QUMethod slot_1 = {"UpdateDisplay", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "UpdateData()", &slot_0, QMetaData::Private },
	{ "UpdateDisplay()", &slot_1, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ "bConnected", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"ConnexionStatusChanged", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ "bPaused", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod signal_1 = {"PauseStatusChanged", 1, param_signal_1 };
    static const QUParameter param_signal_2[] = {
	{ "nSpeed", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_2 = {"SpeedChanged", 1, param_signal_2 };
    static const QUParameter param_signal_3[] = {
	{ "nTimeInSeconds", &static_QUType_varptr, "\x11", QUParameter::In }
    };
    static const QUMethod signal_3 = {"TimeChanged", 1, param_signal_3 };
    static const QUParameter param_signal_4[] = {
	{ "bTickStart", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod signal_4 = {"TickStartEnd", 1, param_signal_4 };
    static const QUParameter param_signal_5[] = {
	{ "agent", &static_QUType_ptr, "MOS_Agent", QUParameter::InOut }
    };
    static const QUMethod signal_5 = {"AgentCreated", 1, param_signal_5 };
    static const QUParameter param_signal_6[] = {
	{ "agent", &static_QUType_ptr, "MOS_Agent", QUParameter::InOut }
    };
    static const QUMethod signal_6 = {"AgentUpdated", 1, param_signal_6 };
    static const QUParameter param_signal_7[] = {
	{ "agent", &static_QUType_ptr, "MOS_Agent", QUParameter::InOut }
    };
    static const QUMethod signal_7 = {"AgentReparented", 1, param_signal_7 };
    static const QUParameter param_signal_8[] = {
	{ "object", &static_QUType_ptr, "MOS_DynaObject", QUParameter::InOut }
    };
    static const QUMethod signal_8 = {"DynaObjectCreated", 1, param_signal_8 };
    static const QUParameter param_signal_9[] = {
	{ "object", &static_QUType_ptr, "MOS_DynaObject", QUParameter::InOut }
    };
    static const QUMethod signal_9 = {"DynaObjectUpdated", 1, param_signal_9 };
    static const QUParameter param_signal_10[] = {
	{ "object", &static_QUType_ptr, "MOS_DynaObject", QUParameter::InOut }
    };
    static const QUMethod signal_10 = {"DynaObjectDeleted", 1, param_signal_10 };
    static const QUParameter param_signal_11[] = {
	{ "gtia", &static_QUType_ptr, "MOS_Gtia", QUParameter::InOut }
    };
    static const QUMethod signal_11 = {"GtiaCreated", 1, param_signal_11 };
    static const QUParameter param_signal_12[] = {
	{ "gtia", &static_QUType_ptr, "MOS_Gtia", QUParameter::InOut }
    };
    static const QUMethod signal_12 = {"GtiaDeleted", 1, param_signal_12 };
    static const QUParameter param_signal_13[] = {
	{ "team", &static_QUType_ptr, "MOS_Team", QUParameter::InOut }
    };
    static const QUMethod signal_13 = {"TeamCreated", 1, param_signal_13 };
    static const QUParameter param_signal_14[] = {
	{ "agent", &static_QUType_ptr, "MOS_Agent", QUParameter::InOut },
	{ "report", &static_QUType_ptr, "MOS_Report_ABC", QUParameter::InOut }
    };
    static const QUMethod signal_14 = {"ReportCreated", 2, param_signal_14 };
    static const QUParameter param_signal_15[] = {
	{ "agent", &static_QUType_ptr, "MOS_Agent", QUParameter::InOut }
    };
    static const QUMethod signal_15 = {"AgentOutOfGas", 1, param_signal_15 };
    static const QUParameter param_signal_16[] = {
	{ "agent", &static_QUType_ptr, "MOS_Agent", QUParameter::InOut }
    };
    static const QUMethod signal_16 = {"AgentRefueled", 1, param_signal_16 };
    static const QUParameter param_signal_17[] = {
	{ "agent", &static_QUType_ptr, "MOS_Agent", QUParameter::InOut }
    };
    static const QUMethod signal_17 = {"AgentConflictStarted", 1, param_signal_17 };
    static const QUParameter param_signal_18[] = {
	{ "agent", &static_QUType_ptr, "MOS_Agent", QUParameter::InOut }
    };
    static const QUMethod signal_18 = {"AgentConflictEnded", 1, param_signal_18 };
    static const QUParameter param_signal_19[] = {
	{ "gtia", &static_QUType_ptr, "MOS_Gtia", QUParameter::InOut },
	{ "knowledge", &static_QUType_ptr, "MOS_AgentKnowledge", QUParameter::InOut }
    };
    static const QUMethod signal_19 = {"AgentKnowledgeCreated", 2, param_signal_19 };
    static const QUParameter param_signal_20[] = {
	{ "gtia", &static_QUType_ptr, "MOS_Gtia", QUParameter::InOut },
	{ "knowledge", &static_QUType_ptr, "MOS_AgentKnowledge", QUParameter::InOut }
    };
    static const QUMethod signal_20 = {"AgentKnowledgeUpdated", 2, param_signal_20 };
    static const QUParameter param_signal_21[] = {
	{ "gtia", &static_QUType_ptr, "MOS_Gtia", QUParameter::InOut },
	{ "knowledge", &static_QUType_ptr, "MOS_AgentKnowledge", QUParameter::InOut }
    };
    static const QUMethod signal_21 = {"AgentKnowledgeDeleted", 2, param_signal_21 };
    static const QUParameter param_signal_22[] = {
	{ "team", &static_QUType_ptr, "MOS_Team", QUParameter::InOut },
	{ "knowledge", &static_QUType_ptr, "MOS_DynaObjectKnowledge", QUParameter::InOut }
    };
    static const QUMethod signal_22 = {"ObjectKnowledgeCreated", 2, param_signal_22 };
    static const QUParameter param_signal_23[] = {
	{ "team", &static_QUType_ptr, "MOS_Team", QUParameter::InOut },
	{ "knowledge", &static_QUType_ptr, "MOS_DynaObjectKnowledge", QUParameter::InOut }
    };
    static const QUMethod signal_23 = {"ObjectKnowledgeUpdated", 2, param_signal_23 };
    static const QUParameter param_signal_24[] = {
	{ "team", &static_QUType_ptr, "MOS_Team", QUParameter::InOut },
	{ "knowledge", &static_QUType_ptr, "MOS_DynaObjectKnowledge", QUParameter::InOut }
    };
    static const QUMethod signal_24 = {"ObjectKnowledgeDeleted", 2, param_signal_24 };
    static const QUParameter param_signal_25[] = {
	{ "line", &static_QUType_ptr, "MOS_TacticalLine_ABC", QUParameter::InOut }
    };
    static const QUMethod signal_25 = {"TacticalLineDeleted", 1, param_signal_25 };
    static const QMetaData signal_tbl[] = {
	{ "ConnexionStatusChanged(bool)", &signal_0, QMetaData::Public },
	{ "PauseStatusChanged(bool)", &signal_1, QMetaData::Public },
	{ "SpeedChanged(int)", &signal_2, QMetaData::Public },
	{ "TimeChanged(uint)", &signal_3, QMetaData::Public },
	{ "TickStartEnd(bool)", &signal_4, QMetaData::Public },
	{ "AgentCreated(MOS_Agent&)", &signal_5, QMetaData::Public },
	{ "AgentUpdated(MOS_Agent&)", &signal_6, QMetaData::Public },
	{ "AgentReparented(MOS_Agent&)", &signal_7, QMetaData::Public },
	{ "DynaObjectCreated(MOS_DynaObject&)", &signal_8, QMetaData::Public },
	{ "DynaObjectUpdated(MOS_DynaObject&)", &signal_9, QMetaData::Public },
	{ "DynaObjectDeleted(MOS_DynaObject&)", &signal_10, QMetaData::Public },
	{ "GtiaCreated(MOS_Gtia&)", &signal_11, QMetaData::Public },
	{ "GtiaDeleted(MOS_Gtia&)", &signal_12, QMetaData::Public },
	{ "TeamCreated(MOS_Team&)", &signal_13, QMetaData::Public },
	{ "ReportCreated(MOS_Agent&,MOS_Report_ABC&)", &signal_14, QMetaData::Public },
	{ "AgentOutOfGas(MOS_Agent&)", &signal_15, QMetaData::Public },
	{ "AgentRefueled(MOS_Agent&)", &signal_16, QMetaData::Public },
	{ "AgentConflictStarted(MOS_Agent&)", &signal_17, QMetaData::Public },
	{ "AgentConflictEnded(MOS_Agent&)", &signal_18, QMetaData::Public },
	{ "AgentKnowledgeCreated(MOS_Gtia&,MOS_AgentKnowledge&)", &signal_19, QMetaData::Public },
	{ "AgentKnowledgeUpdated(MOS_Gtia&,MOS_AgentKnowledge&)", &signal_20, QMetaData::Public },
	{ "AgentKnowledgeDeleted(MOS_Gtia&,MOS_AgentKnowledge&)", &signal_21, QMetaData::Public },
	{ "ObjectKnowledgeCreated(MOS_Team&,MOS_DynaObjectKnowledge&)", &signal_22, QMetaData::Public },
	{ "ObjectKnowledgeUpdated(MOS_Team&,MOS_DynaObjectKnowledge&)", &signal_23, QMetaData::Public },
	{ "ObjectKnowledgeDeleted(MOS_Team&,MOS_DynaObjectKnowledge&)", &signal_24, QMetaData::Public },
	{ "TacticalLineDeleted(MOS_TacticalLine_ABC&)", &signal_25, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"MOS_App", parentObject,
	slot_tbl, 2,
	signal_tbl, 26,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MOS_App.setMetaObject( metaObj );
    return metaObj;
}

void* MOS_App::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MOS_App" ) )
	return this;
    return QApplication::qt_cast( clname );
}

// SIGNAL ConnexionStatusChanged
void MOS_App::ConnexionStatusChanged( bool t0 )
{
    activate_signal_bool( staticMetaObject()->signalOffset() + 0, t0 );
}

// SIGNAL PauseStatusChanged
void MOS_App::PauseStatusChanged( bool t0 )
{
    activate_signal_bool( staticMetaObject()->signalOffset() + 1, t0 );
}

// SIGNAL SpeedChanged
void MOS_App::SpeedChanged( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 2, t0 );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL TimeChanged
void MOS_App::TimeChanged( uint t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 3 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_varptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL TickStartEnd
void MOS_App::TickStartEnd( bool t0 )
{
    activate_signal_bool( staticMetaObject()->signalOffset() + 4, t0 );
}

// SIGNAL AgentCreated
void MOS_App::AgentCreated( MOS_Agent& t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 5 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL AgentUpdated
void MOS_App::AgentUpdated( MOS_Agent& t0 )
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

// SIGNAL AgentReparented
void MOS_App::AgentReparented( MOS_Agent& t0 )
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

// SIGNAL DynaObjectCreated
void MOS_App::DynaObjectCreated( MOS_DynaObject& t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 8 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL DynaObjectUpdated
void MOS_App::DynaObjectUpdated( MOS_DynaObject& t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 9 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL DynaObjectDeleted
void MOS_App::DynaObjectDeleted( MOS_DynaObject& t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 10 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL GtiaCreated
void MOS_App::GtiaCreated( MOS_Gtia& t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 11 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL GtiaDeleted
void MOS_App::GtiaDeleted( MOS_Gtia& t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 12 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL TeamCreated
void MOS_App::TeamCreated( MOS_Team& t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 13 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL ReportCreated
void MOS_App::ReportCreated( MOS_Agent& t0, MOS_Report_ABC& t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 14 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_ptr.set(o+1,&t0);
    static_QUType_ptr.set(o+2,&t1);
    activate_signal( clist, o );
}

// SIGNAL AgentOutOfGas
void MOS_App::AgentOutOfGas( MOS_Agent& t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 15 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL AgentRefueled
void MOS_App::AgentRefueled( MOS_Agent& t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 16 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL AgentConflictStarted
void MOS_App::AgentConflictStarted( MOS_Agent& t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 17 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL AgentConflictEnded
void MOS_App::AgentConflictEnded( MOS_Agent& t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 18 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

// SIGNAL AgentKnowledgeCreated
void MOS_App::AgentKnowledgeCreated( MOS_Gtia& t0, MOS_AgentKnowledge& t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 19 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_ptr.set(o+1,&t0);
    static_QUType_ptr.set(o+2,&t1);
    activate_signal( clist, o );
}

// SIGNAL AgentKnowledgeUpdated
void MOS_App::AgentKnowledgeUpdated( MOS_Gtia& t0, MOS_AgentKnowledge& t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 20 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_ptr.set(o+1,&t0);
    static_QUType_ptr.set(o+2,&t1);
    activate_signal( clist, o );
}

// SIGNAL AgentKnowledgeDeleted
void MOS_App::AgentKnowledgeDeleted( MOS_Gtia& t0, MOS_AgentKnowledge& t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 21 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_ptr.set(o+1,&t0);
    static_QUType_ptr.set(o+2,&t1);
    activate_signal( clist, o );
}

// SIGNAL ObjectKnowledgeCreated
void MOS_App::ObjectKnowledgeCreated( MOS_Team& t0, MOS_DynaObjectKnowledge& t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 22 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_ptr.set(o+1,&t0);
    static_QUType_ptr.set(o+2,&t1);
    activate_signal( clist, o );
}

// SIGNAL ObjectKnowledgeUpdated
void MOS_App::ObjectKnowledgeUpdated( MOS_Team& t0, MOS_DynaObjectKnowledge& t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 23 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_ptr.set(o+1,&t0);
    static_QUType_ptr.set(o+2,&t1);
    activate_signal( clist, o );
}

// SIGNAL ObjectKnowledgeDeleted
void MOS_App::ObjectKnowledgeDeleted( MOS_Team& t0, MOS_DynaObjectKnowledge& t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 24 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_ptr.set(o+1,&t0);
    static_QUType_ptr.set(o+2,&t1);
    activate_signal( clist, o );
}

// SIGNAL TacticalLineDeleted
void MOS_App::TacticalLineDeleted( MOS_TacticalLine_ABC& t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 25 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

bool MOS_App::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: UpdateData(); break;
    case 1: UpdateDisplay(); break;
    default:
	return QApplication::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MOS_App::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: ConnexionStatusChanged((bool)static_QUType_bool.get(_o+1)); break;
    case 1: PauseStatusChanged((bool)static_QUType_bool.get(_o+1)); break;
    case 2: SpeedChanged((int)static_QUType_int.get(_o+1)); break;
    case 3: TimeChanged((uint)(*((uint*)static_QUType_ptr.get(_o+1)))); break;
    case 4: TickStartEnd((bool)static_QUType_bool.get(_o+1)); break;
    case 5: AgentCreated((MOS_Agent&)*((MOS_Agent*)static_QUType_ptr.get(_o+1))); break;
    case 6: AgentUpdated((MOS_Agent&)*((MOS_Agent*)static_QUType_ptr.get(_o+1))); break;
    case 7: AgentReparented((MOS_Agent&)*((MOS_Agent*)static_QUType_ptr.get(_o+1))); break;
    case 8: DynaObjectCreated((MOS_DynaObject&)*((MOS_DynaObject*)static_QUType_ptr.get(_o+1))); break;
    case 9: DynaObjectUpdated((MOS_DynaObject&)*((MOS_DynaObject*)static_QUType_ptr.get(_o+1))); break;
    case 10: DynaObjectDeleted((MOS_DynaObject&)*((MOS_DynaObject*)static_QUType_ptr.get(_o+1))); break;
    case 11: GtiaCreated((MOS_Gtia&)*((MOS_Gtia*)static_QUType_ptr.get(_o+1))); break;
    case 12: GtiaDeleted((MOS_Gtia&)*((MOS_Gtia*)static_QUType_ptr.get(_o+1))); break;
    case 13: TeamCreated((MOS_Team&)*((MOS_Team*)static_QUType_ptr.get(_o+1))); break;
    case 14: ReportCreated((MOS_Agent&)*((MOS_Agent*)static_QUType_ptr.get(_o+1)),(MOS_Report_ABC&)*((MOS_Report_ABC*)static_QUType_ptr.get(_o+2))); break;
    case 15: AgentOutOfGas((MOS_Agent&)*((MOS_Agent*)static_QUType_ptr.get(_o+1))); break;
    case 16: AgentRefueled((MOS_Agent&)*((MOS_Agent*)static_QUType_ptr.get(_o+1))); break;
    case 17: AgentConflictStarted((MOS_Agent&)*((MOS_Agent*)static_QUType_ptr.get(_o+1))); break;
    case 18: AgentConflictEnded((MOS_Agent&)*((MOS_Agent*)static_QUType_ptr.get(_o+1))); break;
    case 19: AgentKnowledgeCreated((MOS_Gtia&)*((MOS_Gtia*)static_QUType_ptr.get(_o+1)),(MOS_AgentKnowledge&)*((MOS_AgentKnowledge*)static_QUType_ptr.get(_o+2))); break;
    case 20: AgentKnowledgeUpdated((MOS_Gtia&)*((MOS_Gtia*)static_QUType_ptr.get(_o+1)),(MOS_AgentKnowledge&)*((MOS_AgentKnowledge*)static_QUType_ptr.get(_o+2))); break;
    case 21: AgentKnowledgeDeleted((MOS_Gtia&)*((MOS_Gtia*)static_QUType_ptr.get(_o+1)),(MOS_AgentKnowledge&)*((MOS_AgentKnowledge*)static_QUType_ptr.get(_o+2))); break;
    case 22: ObjectKnowledgeCreated((MOS_Team&)*((MOS_Team*)static_QUType_ptr.get(_o+1)),(MOS_DynaObjectKnowledge&)*((MOS_DynaObjectKnowledge*)static_QUType_ptr.get(_o+2))); break;
    case 23: ObjectKnowledgeUpdated((MOS_Team&)*((MOS_Team*)static_QUType_ptr.get(_o+1)),(MOS_DynaObjectKnowledge&)*((MOS_DynaObjectKnowledge*)static_QUType_ptr.get(_o+2))); break;
    case 24: ObjectKnowledgeDeleted((MOS_Team&)*((MOS_Team*)static_QUType_ptr.get(_o+1)),(MOS_DynaObjectKnowledge&)*((MOS_DynaObjectKnowledge*)static_QUType_ptr.get(_o+2))); break;
    case 25: TacticalLineDeleted((MOS_TacticalLine_ABC&)*((MOS_TacticalLine_ABC*)static_QUType_ptr.get(_o+1))); break;
    default:
	return QApplication::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool MOS_App::qt_property( int id, int f, QVariant* v)
{
    return QApplication::qt_property( id, f, v);
}

bool MOS_App::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
