    // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MissionPanel.h"
#include "moc_MissionPanel.cpp"

#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/RichCheckBox.h"
#include "clients_gui/RichDateTimeEdit.h"
#include "clients_gui/RichLabel.h"
#include "clients_gui/RichPushButton.h"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Mission.h"
#include "clients_kernel/FragOrder.h"
#include "clients_kernel/FragOrderType.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Time_ABC.h"
#include "clients_kernel/tools.h"
#include "clients_kernel/MissionType.h"

#include "gaming/CommandPublisher.h"
#include "gaming/AutomatDecisions.h"
#include "gaming/Decisions.h"
#include "gaming/Event.h"
#include "gaming/PopulationDecisions.h"
#include "gaming/StaticModel.h"
#include "gaming/TimelinePublisher.h"

#include "protocol/ServerPublisher_ABC.h"

#include "icons.h"

#include "actions/Action_ABC.h"
#include "actions/ActionTasker.h"
#include "actions/EngageMagicAction.h"
#include "actions/ActionTiming.h"

#include "actions_gui/InterfaceBuilder_ABC.h"
#include "actions_gui/MissionInterface.h"
#include "actions_gui/resources.h"

#include "ENT/ENT_Tr.h"
#include "timeline/api.h"
#include "tools/ExerciseConfig.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MissionPanel constructor
// Created: APE 2004-03-19
// -----------------------------------------------------------------------------
MissionPanel::MissionPanel( QWidget* pParent, Controllers& controllers, const ::StaticModel& model, Publisher_ABC& publisher,
                            gui::GlTools_ABC& tools, const kernel::Profile_ABC& profile, actions::ActionsModel& actionsModel,
                            const kernel::Time_ABC& simulation, actions::gui::InterfaceBuilder_ABC& interfaceBuilder,
                            const tools::ExerciseConfig& config, TimelinePublisher& timelinePublisher )
    : gui::RichDockWidget      ( controllers, pParent, "mission" )
    , static_                  ( model )
    , actionsModel_            ( actionsModel )
    , tools_                   ( tools )
    , profile_                 ( profile )
    , commandPublisher_        ( new CommandPublisher( controllers_, publisher, profile_ ) )
    , pMissionInterface_       ( 0 )
    , selectedEntity_          ( controllers )
    , simulation_              ( simulation )
    , interfaceBuilder_        ( interfaceBuilder )
    , config_                  ( config )
    , timelinePublisher_       ( timelinePublisher )
{
    setWindowTitle( tr( "Mission" ) );
    setFloating( true );
    hide();

    // Top Layout
    titleLabel_ = new gui::RichLabel( "mission-title" );
    QFont font = titleLabel_->font();
    font.setBold( true );
    font.setPixelSize( 16 );
    titleLabel_->setFont( font );
    pixmapLabel_ = new gui::RichLabel( "mission-pixmap" );
    pixmapLabel_->setPixmap( MAKE_PIXMAP( mission_title ) );

    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->setContentsMargins( 5, 0, 0, 0 );
    topLayout->addWidget( titleLabel_ );
    topLayout->addStretch( 1 );
    topLayout->addWidget( pixmapLabel_ );

    // Bottom Layout
    planningCheckBox_ = new gui::RichCheckBox( "planning-checkbox" );
    planningCheckBox_->setText( tr( "Plan mission" ) );
    planningDateTimeEdit_ = new ::gui::RichDateTimeEdit( "planning-datetimeedit" );
    planningDateTimeEdit_->setCalendarPopup( true );
    planningDateTimeEdit_->setTimeSpec( Qt::UTC );
    planningDateTimeEdit_->setDateTime( simulation_.GetDateTime() );
    connect( planningCheckBox_, SIGNAL( stateChanged( int ) ), this, SLOT( OnPlanningChecked( int ) ) );
    okButton_ = new gui::RichPushButton( "ok", tr( "Ok" ) );
    okButton_->setDefault( true );
    connect( okButton_, SIGNAL( clicked() ), SLOT( Validate() ) );
    cancelButton_ = new gui::RichPushButton( "cancel", tr( "Cancel" ) );
    connect( cancelButton_, SIGNAL( clicked() ), SLOT( Close() ) );

    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->setMargin( 5 );
    bottomLayout->setSpacing( 5 );
    bottomLayout->addWidget( planningCheckBox_ );
    bottomLayout->addWidget( planningDateTimeEdit_ );
    bottomLayout->addStretch( 1 );
    bottomLayout->addWidget( okButton_ );
    bottomLayout->addWidget( cancelButton_ );

    // Main Layout
    stack_ = new QStackedWidget();
    CreateMissionInterface< kernel::MissionType >( eMissionType_Pawn, "unit-mission-interface", "units-mission-sheets-directory" );
    CreateMissionInterface< kernel::MissionType >( eMissionType_Automat, "automat-mission-interface", "automata-mission-sheets-directory" );
    CreateMissionInterface< kernel::MissionType >( eMissionType_Population, "crowd-mission-interface", "crowds-mission-sheets-directory" );
    CreateMissionInterface< kernel::MissionType >( eMissionType_Pawn, "fragorder-interface", "fragorders-mission-sheets-directory" );
    stack_->insertWidget( eNbrMissionTypes, new QWidget() );
    stack_->setCurrentIndex( eNbrMissionTypes );

    QWidget* mainWidget = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout( mainWidget );
    mainLayout->setMargin( 0 );
    mainLayout->setSpacing( 0 );
    mainLayout->addLayout( topLayout );
    mainLayout->addWidget( stack_, 1 );
    mainLayout->addLayout( bottomLayout );
    setWidget( mainWidget );
    SetVisible( false );

    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel destructor
// Created: APE 2004-03-19
// -----------------------------------------------------------------------------
MissionPanel::~MissionPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::SetVisible
// Created: ABR 2013-06-04
// -----------------------------------------------------------------------------
void MissionPanel::SetVisible( bool visible )
{
    titleLabel_->setVisible( visible );
    pixmapLabel_->setVisible( visible );
    planningCheckBox_->setVisible( visible ? config_.HasTimeline() : false );
    planningDateTimeEdit_->setVisible( false );
    okButton_->setVisible( visible );
    cancelButton_->setVisible( visible );
    setVisible( visible );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::CreateMissionInterface
// Created: ABR 2013-06-04
// -----------------------------------------------------------------------------
template< typename T >
void MissionPanel::CreateMissionInterface( E_MissionType type, const QString& objectName, const std::string& missionSheetPhysicalTag )
{
    QWidget* missionInterface = new actions::gui::MissionInterface< T >( 0, objectName, controllers_, config_, missionSheetPhysicalTag, actionsModel_ );
    stack_->insertWidget( type, missionInterface );
    assert( planningCheckBox_ != 0 );
    connect( missionInterface, SIGNAL( PlannedMission( const actions::Action_ABC& ) ), this, SLOT( OnPlannedMission( const actions::Action_ABC& ) ) );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::NotifyContextMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void MissionPanel::NotifyContextMenu( const Agent_ABC& agent, ContextMenu& menu )
{
    if( profile_.CanBeOrdered( agent ) )
    {
        selectedEntity_ = &agent;
        if( const Decisions* decisions = agent.Retrieve< Decisions >() )
            AddMissions( *decisions, menu, tr( "Agent missions" ), SLOT( ActivateAgentMission( int ) ) );
        if( const Automat_ABC* automat = static_cast< const Automat_ABC* >( agent.Get< kernel::TacticalHierarchies >().GetSuperior() ) )
            AddMissions( static_cast< const AutomatDecisions& >( automat->Get< kernel::AutomatDecisions_ABC >() ), menu, tr( "Automat missions" ), SLOT( ActivateAutomatMission( int ) ), MAKE_PIXMAP( lock ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::NotifyContextMenu
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void MissionPanel::NotifyContextMenu( const kernel::Automat_ABC& agent, kernel::ContextMenu& menu )
{
    if( profile_.CanBeOrdered( agent ) )
    {
        selectedEntity_ = &agent;
        const AutomatDecisions& decisions = static_cast< const AutomatDecisions& >( agent.Get< kernel::AutomatDecisions_ABC >() );
        AddMissions( decisions, menu, tr( "Automat missions" ), SLOT( ActivateAutomatMission( int ) ), MAKE_PIXMAP( lock ) );
        if( ! decisions.IsEmbraye() )
            menu.InsertItem( "Command", tr( "Engage" ), this, SLOT( Engage() ) );
        else if( decisions.CanBeOrdered() )
            menu.InsertItem( "Command", tr( "Disengage" ), this, SLOT( Disengage() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::NotifyContextMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void MissionPanel::NotifyContextMenu( const Population_ABC& entity, ContextMenu& menu )
{
    if( profile_.CanBeOrdered( entity ) )
    {
        selectedEntity_ = &entity;
        const PopulationDecisions& decisions = entity.Get< PopulationDecisions >();
        AddMissions( decisions, menu, tr( "Crowd missions" ), SLOT( ActivatePopulationMission( int ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::NotifyDeleted
// Created: SBO 2008-08-12
// -----------------------------------------------------------------------------
void MissionPanel::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    if( &entity == selectedEntity_ || selectedEntity_ == 0 )
        SetVisible( false );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::NotifyModeChanged
// Created: ABR 2013-06-04
// -----------------------------------------------------------------------------
void MissionPanel::NotifyModeChanged( E_Modes newMode )
{
    ModesObserver_ABC::NotifyModeChanged( newMode );
    if( okButton_ )
        okButton_->setText( newMode == eModes_Planning ? tr( "Add to planning" ) : tr( "Ok" ) );
}

namespace
{
    QString GetPrefix( const QString& name )
    {
        {
            QRegExp regexp( "(^[A-Z ]*[A-Z]{3,})\\s" );
            if( regexp.search( name ) > -1 )
                return regexp.cap( 1 );
        }
        {
            QRegExp regexp( "^(\\w{3,})\\s-\\s" );
            if( regexp.search( name ) > -1 )
                return regexp.cap( 1 );
        }
        return "";
    }

    QString FormatName( const Mission& mission, const QString& prefix )
    {
        QString result( QString( mission.GetName().c_str() ).mid( prefix.length() ).stripWhiteSpace() );
        if( result.startsWith( "-" ) )
            return result.mid( 1 ).stripWhiteSpace();
        return result;
    }

    QString FormatName( const FragOrder& order, const QString& /*prefix*/ )
    {
        return order.GetName().c_str();
    }

    struct MissionComparator
    {
        bool operator()( const Mission* lhs, const Mission* rhs )
        {
            return lhs->GetName() < rhs->GetName();
        }
        bool operator()( const FragOrder* lhs, const FragOrder* rhs )
        {
            return lhs->GetName() < rhs->GetName();
        }
        bool operator()( const QString& lhs, const QString& rhs ) const
        {
            return ( lhs.isEmpty() && ! rhs.isEmpty() ) ? false : ( ( rhs.isEmpty() && !lhs.isEmpty() ) ? true : lhs < rhs );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::AddMissionGroup
// Created: SBO 2008-10-20
// -----------------------------------------------------------------------------
template< typename E, typename T >
void MissionPanel::AddMissionGroup( kernel::ContextMenu& menu, const QString& prefix, const T& list, const char* slot, int current )
{
    if( list.empty() )
        return;
     if( prefix.isEmpty() )
     {
        if( menu.idAt( 0 ) != -1 )
           menu.addSeparator()->setText( tools::translate( "MissionPanel", "" ) );
     }
    else
    {
        if( !menu.count() ) // $$$$ FPT 2011-08-12 : Can't have a separator without an item before
        {
            menu.insertItem( "", 0 );
            menu.setItemFont( 0, QFont ( "Arial", 1 ) );
            menu.setItemEnabled( 0, false );
        }
        menu.addSeparator()->setText( prefix );
     }
    for( T::const_iterator it = list.begin(); it != list.end(); ++it )
    {
        const E& order = **it;
        const int id = menu.insertItem( FormatName( order, prefix ), this, slot );
        menu.setItemParameter( id, order.GetId() );
        menu.setItemChecked( id, current == int( order.GetId() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::AddMissions
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
QAction* MissionPanel::AddMissions( tools::Iterator< const Mission& > it, ContextMenu& menu, const QString& name, const char* slot, int current )
{
    kernel::ContextMenu& missions = *new kernel::ContextMenu( &menu );
    QString lastPrefix;
    typedef std::map< QString, std::set< const Mission*, MissionComparator >, MissionComparator > T_Missions;
    T_Missions list;
    while( it.HasMoreElements() )
    {
        const Mission& mission = it.NextElement();
        const QString prefix = GetPrefix( mission.GetName().c_str() );
        list[ prefix ].insert( &mission );
    }
    for( T_Missions::const_iterator itM = list.begin(); itM != list.end(); ++itM )
        AddMissionGroup< Mission >( missions, itM->first, itM->second, slot, current );
    return menu.InsertItem( "Order", name, &missions, 3 );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::AddFragOrders
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
QAction* MissionPanel::AddFragOrders( const Decisions_ABC& decisions, ContextMenu& menu, const QString& name, const char* slot )
{
    kernel::ContextMenu& orders = *new kernel::ContextMenu( &menu );
    typedef std::map< QString, std::set< const FragOrder*, MissionComparator >, MissionComparator > T_FragOrders;
    T_FragOrders list;
    if( const Mission* mission = decisions.GetCurrentMission() )
    {
        tools::Iterator< const FragOrder& > it = static_cast< const tools::Resolver< FragOrder >& >( *mission ).CreateIterator();
        while( it.HasMoreElements() )
        {
            const FragOrder& element = it.NextElement();
            list[ element.GetType().IsMissionRequired() ? mission->GetName().c_str() : "" ].insert( &element );
        }
        it = decisions.GetFragOrders();
        while( it.HasMoreElements() )
        {
            const FragOrder& element = it.NextElement();
            if( element.GetType().IsMissionRequired() && element.GetType().IsAvailableFor( *selectedEntity_ ) )
                list[ "" ].insert( &element );
        }
    }
    {
        tools::Iterator< const FragOrder& > it = decisions.GetFragOrders();
        while( it.HasMoreElements() )
        {
            const FragOrder& element = it.NextElement();
            if( !element.GetType().IsMissionRequired() && element.GetType().IsAvailableFor( *selectedEntity_ ) )
                list[ "" ].insert( &element );
        }
    }
    for( T_FragOrders::const_iterator itM = list.begin(); itM != list.end(); ++itM )
        AddMissionGroup< FragOrder >( orders, itM->first, itM->second, slot, -1 );
    return menu.InsertItem( "Order", name, &orders, 2 );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::AddMissions
// Created: AGE 2007-04-04
// -----------------------------------------------------------------------------
void MissionPanel::AddMissions( const Decisions_ABC& decisions, kernel::ContextMenu& menu, const QString& name, const char* slot, const QPixmap& pixmap /* = QPixmap()*/ )
{
    if( !decisions.CanBeOrdered() )
        return;

    const kernel::Mission* current = decisions.GetCurrentMission();
    QAction* action = AddMissions( decisions.GetMissions(), menu, name, slot, current ? current->GetId() : -1 );
    if( !pixmap.isNull() )
    {
        QIcon icon( pixmap );
        action->setIcon( icon );
    }

    AddFragOrders( decisions, menu, tr( "Fragmentary orders" ), SLOT( ActivateFragOrder( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::FillInterface
// Created: ABR 2013-06-04
// -----------------------------------------------------------------------------
template< typename T >
void MissionPanel::FillInterface( int id )
{
    if( !selectedEntity_ )
        return;
    pMissionInterface_ = static_cast< actions::gui::MissionInterface_ABC* >( stack_->currentWidget() );
    pMissionInterface_->Purge();
    NotifyMission();
    const T& order = static_cast< tools::Resolver_ABC< T >& >( static_.types_).Get( id );
    pMissionInterface_->Fill( interfaceBuilder_, *selectedEntity_, order );
    titleLabel_->setText( pMissionInterface_->Title() );
    planningCheckBox_->setCheckState( Qt::Unchecked );
    if( pMissionInterface_->IsEmpty() )
        Validate();
    else
        SetVisible( true );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::ActivateAgentMission
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void MissionPanel::ActivateAgentMission( int id )
{
    stack_->setCurrentIndex( eMissionType_Pawn );
    FillInterface< kernel::MissionType >( id );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::ActivateAutomatMission
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
void MissionPanel::ActivateAutomatMission( int id )
{
    stack_->setCurrentIndex( eMissionType_Automat );
    if( !selectedEntity_->Retrieve< kernel::AutomatDecisions_ABC >() )
        selectedEntity_ = selectedEntity_->Get< kernel::TacticalHierarchies >().GetSuperior();
    FillInterface< kernel::MissionType >( id );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::ActivatePopulationMission
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void MissionPanel::ActivatePopulationMission( int id )
{
    stack_->setCurrentIndex( eMissionType_Population );
    FillInterface< kernel::MissionType >( id );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::ActivateFragOrder
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void MissionPanel::ActivateFragOrder( int id )
{
    stack_->setCurrentIndex( eMissionType_FragOrder );
    if( !selectedEntity_->Retrieve< kernel::AutomatDecisions_ABC >() )
    {
        Entity_ABC* superior = const_cast< kernel::Entity_ABC* >( selectedEntity_->Get< kernel::TacticalHierarchies >().GetSuperior() );
        if( const kernel::AutomatDecisions_ABC* decisions = superior->Retrieve< kernel::AutomatDecisions_ABC >() )
            if( decisions->IsEmbraye() )
                selectedEntity_ = superior;
    }
    FillInterface< kernel::FragOrderType >( id );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void MissionPanel::Draw( gui::Viewport_ABC& viewport )
{
    if( pMissionInterface_ && isVisible() )
    {
        glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
            glLineWidth( 2.f );
            glColor4f( COLOR_PARAM );
            pMissionInterface_->Draw( tools_, viewport );
        glPopAttrib();
    }
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::Engage
// Created: SBO 2006-06-19
// -----------------------------------------------------------------------------
void MissionPanel::Engage()
{
    if( !selectedEntity_ )
        return;
    kernel::AutomatDecisions_ABC* decisions = selectedEntity_.ConstCast()->Retrieve< kernel::AutomatDecisions_ABC >();
    if( !decisions )
        return;

    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "change_mode" );
    actions::EngageMagicAction* action = new actions::EngageMagicAction( *selectedEntity_, actionType, controllers_.controller_, tr( "Engage" ), true, true );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new actions::ActionTasker( selectedEntity_, false ) );
    action->RegisterAndPublish( actionsModel_ );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::Disengage
// Created: SBO 2006-06-19
// -----------------------------------------------------------------------------
void MissionPanel::Disengage()
{
    if( !selectedEntity_ )
        return;
    kernel::AutomatDecisions_ABC* decisions = selectedEntity_.ConstCast()->Retrieve< kernel::AutomatDecisions_ABC >();
    if( !decisions )
        return;

    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( static_.types_ ).Get( "change_mode" );
    actions::EngageMagicAction* action = new actions::EngageMagicAction( *selectedEntity_, actionType, controllers_.controller_, tr( "Disengage" ), false, true );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new actions::ActionTasker( selectedEntity_, false ) );
    action->RegisterAndPublish( actionsModel_ );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::Close
// Created: LDC 2011-08-29
// -----------------------------------------------------------------------------
void MissionPanel::Close()
{
    if( pMissionInterface_ )
        pMissionInterface_->Purge();
    pMissionInterface_ = 0;
    stack_->setCurrentIndex( eNbrMissionTypes );
    SetVisible( false );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::closeEvent
// Created: ABR 2013-06-04
// -----------------------------------------------------------------------------
void MissionPanel::closeEvent( QCloseEvent* event )
{
    Close();
    gui::RichDockWidget::closeEvent( event );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::NotifyMission
// Created: AGE 2008-07-15
// -----------------------------------------------------------------------------
void MissionPanel::NotifyMission()
{
    if( pMissionInterface_ && selectedEntity_ )
    {
        std::stringstream message;
        message << "/mission "
                << selectedEntity_->GetTypeName() << " " << selectedEntity_->GetId() << " "
                << "\"" << pMissionInterface_->Title().toStdString() << "\"";
        commandPublisher_->Send( "", message.str() );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionInterface::Validate
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
void MissionPanel::Validate()
{
    if( static_cast< E_MissionType >( stack_->currentIndex() ) == eNbrMissionTypes || !pMissionInterface_ || !pMissionInterface_->CheckValidity() )
        return;
    pMissionInterface_->Publish();
    Close();
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::OnPlanningChecked
// Created: ABR 2013-06-04
// -----------------------------------------------------------------------------
void MissionPanel::OnPlanningChecked( int state )
{
    bool planned = state == Qt::Checked;

    planningDateTimeEdit_->setVisible( planned );
    planningDateTimeEdit_->setDateTime( simulation_.GetDateTime() );
    if( pMissionInterface_ )
        pMissionInterface_->SetPlanned( planned );
}

// -----------------------------------------------------------------------------
// Name: MissionPanel::OnPlannedMission
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
void MissionPanel::OnPlannedMission( const actions::Action_ABC& action )
{
    timeline::Event event;
    event.name = action.GetType().GetName();
    event.info = ENT_Tr::ConvertFromEventType( eEventTypes_Order, ENT_Tr_ABC::eToSim );
    event.begin = planningDateTimeEdit_->dateTime().toString( EVENT_DATE_FORMAT ).toStdString();
    event.action.target = "sword://sim";
    event.action.apply = true;
    action.Publish( timelinePublisher_, 0 );
    event.action.payload = timelinePublisher_.GetPayload();
    emit CreateEvent( event );
}
