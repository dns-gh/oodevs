// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EventOrderWidget.h"
#include "moc_EventOrderWidget.cpp"
#include "icons.h"
#include "TaskerWidget.h"

#include "actions/ActionError.h"
#include "actions/ActionWithTarget_ABC.h"

#include "actions_gui/MissionInterface.h"

#include "clients_gui/Decisions_ABC.h"
#include "clients_gui/GLToolColors.h"
#include "clients_gui/EntitySymbols.h"
#include "clients_gui/EventAction.h"
#include "clients_gui/EventPresenter.h"
#include "clients_gui/EventOrderPresenter.h"
#include "clients_gui/EventOrderViewState.h"
#include "clients_gui/EventViewState.h"
#include "clients_gui/TimelinePublisher.h"
#include "clients_gui/Tools.h"
#include "clients_gui/RichWarnWidget.h"

#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Time_ABC.h"

#include "ENT/ENT_Tr.h"

#include "gaming/AgentsModel.h"
#include "gaming/Model.h"
#include "gaming/StaticModel.h"
#include "gaming/MissionParameters.h"

#include <timeline/api.h>
#include <boost/make_shared.hpp>

namespace
{
    const QBrush disabledColor = Qt::darkGray;
    const QBrush invalidColor = Qt::red;
    const QBrush defaultColor = Qt::black;
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget constructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventOrderWidget::EventOrderWidget( gui::EventPresenter& presenter,
                                    kernel::Controllers& controllers,
                                    Model& model,
                                    const tools::ExerciseConfig& config,
                                    actions::gui::InterfaceBuilder_ABC& interfaceBuilder,
                                    const kernel::Profile_ABC& profile,
                                    gui::GlTools_ABC& tools,
                                    const kernel::Time_ABC& simulation,
                                    const gui::EntitySymbols& entitySymbols )
    : EventWidget_ABC< gui::EventOrderView_ABC >( presenter )
    , controllers_( controllers )
    , model_( model )
    , interfaceBuilder_( interfaceBuilder )
    , profile_( profile )
    , tools_( tools )
    , simulation_( simulation )
    , selectedEntity_( controllers )
    , selectedEngagedAutomat_( controllers )
    , missionCombo_( 0 )
    , missionTypeCombo_( 0 )
    , missionInterface_( new actions::gui::MissionInterface( 0, "event-mission-interface", controllers, config ) )
{
    // Presenter
    orderPresenter_ = boost::make_shared< gui::EventOrderPresenter >( *this,
                                                                      model.static_.types_,
                                                                      interfaceBuilder,
                                                                      *missionInterface_,
                                                                      model.actions_,
                                                                      model.actionFactory_,
                                                                      model.timelinePublisher_,
                                                                      controllers );
    presenter_.AddSubPresenter( eEventTypes_Order, orderPresenter_ );

    // Tasker
    taskerWidget_ = new TaskerWidget( controllers, entitySymbols );

    // Top
    missionTypeCombo_ = new gui::RichWarnWidget< QComboBox >( "event-order-mission-type-combobox" );
    missionComboLayout_ = new QVBoxLayout();
    missionComboLayout_->setMargin( 0 );
    missionComboLayout_->setSpacing( 0 );

    missionCombo_ = new gui::RichWarnWidget< QComboBox >( "event-order-mission-combobox" );
    QSortFilterProxyModel* proxy = new QSortFilterProxyModel( missionCombo_ );
    proxy->setSourceModel( missionCombo_->model() );
    missionCombo_->model()->setParent( proxy );
    missionCombo_->setModel( proxy );
    missionComboLayout_->addWidget( missionCombo_ );

    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->setSpacing( 5 );
    topLayout->addWidget( missionTypeCombo_ );
    topLayout->addLayout( missionComboLayout_ );

    // Layout
    mainLayout_->setSpacing( 5 );
    mainLayout_->addWidget( taskerWidget_ );
    mainLayout_->addLayout( topLayout );
    mainLayout_->addWidget( static_cast< actions::gui::MissionInterface* >( missionInterface_.get() ), 1 );

    // Connections
    connect( taskerWidget_, SIGNAL( ClearClicked() ), this, SLOT( OnClearTaskerClicked() ) );
    connect( missionTypeCombo_, SIGNAL( currentIndexChanged( const QString& ) ),
             this, SLOT( OnMissionTypeChanged( const QString& ) ) );
    connect( missionCombo_, SIGNAL( currentIndexChanged( const QString& ) ),
             orderPresenter_.get(), SLOT( OnMissionChanged( const QString& ) ) );
    // We want theses slots to be called after those from order presenter
    connect( missionTypeCombo_, SIGNAL( currentIndexChanged( const QString& ) ),
             &presenter_, SLOT( OnEventContentChanged() ) );
    connect( missionCombo_, SIGNAL( currentIndexChanged( const QString& ) ),
             &presenter_, SLOT( OnEventContentChanged() ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget destructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventOrderWidget::~EventOrderWidget()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::Purge
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::Purge()
{
    taskerWidget_->SetTasker( 0 );
    selectedEntity_ = 0;
    selectedEngagedAutomat_ = 0;
    missionTypeCombo_->clear();
    missionCombo_->clear();
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::Draw
// Created: ABR 2013-11-21
// -----------------------------------------------------------------------------
void EventOrderWidget::Draw( gui::Viewport_ABC& viewport )
{
    if( isVisible() )
    {
        glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
        glLineWidth( 2.f );
        glColor4f( COLOR_PARAM );
        missionInterface_->Draw( tools_, viewport );
        glPopAttrib();
    }
}

namespace
{
    const gui::Decisions_ABC* GetDecisions( const kernel::Entity_ABC* entity )
    {
        if( entity )
            return entity->Retrieve< gui::Decisions_ABC >();
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::Build
// Created: ABR 2013-11-21
// -----------------------------------------------------------------------------
void EventOrderWidget::Build( const gui::EventViewState& state )
{
    if( !state.event_ || !state.event_->GetType() == eEventTypes_Order )
        return;
    const gui::EventAction& eventAction = static_cast< const gui::EventAction& >( *state.event_ );
    if( const actions::Action_ABC* action = eventAction.GetAction() )
    {
        const actions::ActionWithTarget_ABC* mission = static_cast< const actions::ActionWithTarget_ABC* >( action );
        kernel::Entity_ABC* tasker = model_.agents_.FindAllAgent( mission->GetEntityId() );
        orderPresenter_->FillFromAction( *action, eventAction.GetMissionType(), tasker, GetDecisions( tasker ) );
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::Build
// Created: LGY 2013-08-29
// -----------------------------------------------------------------------------
void EventOrderWidget::Build( const gui::EventOrderViewState& state )
{
    // Target
    taskerWidget_->SetTasker( model_.agents_.FindAllAgent( state.target_ ) );
    // Mission type
    {
        // CLEAR
        missionTypeCombo_->clear();
        // FILL
        for( auto it = state.types_.begin(); it != state.types_.end(); ++it )
            missionTypeCombo_->addItem( QString::fromStdString( ENT_Tr::ConvertFromMissionType( *it ) ),
                                        QVariant( static_cast< unsigned int >( *it ) ) );
        // SELECT
        missionTypeCombo_->setCurrentIndex( missionTypeCombo_->findText(
            QString::fromStdString( ENT_Tr::ConvertFromMissionType( state.currentType_ ) ) ) );
    }
    // Mission
    {
        // CLEAR
        missionCombo_->clear();
        // FILL
        for( auto it = state.missions_.begin(); it != state.missions_.end(); ++it )
        {
            const std::string& name = *it;
            missionCombo_->addItem( QString::fromStdString( name ) );
            QBrush missionColor = defaultColor;
            if( name == state.currentMission_ )
            {
                if( state.invalid_ )
                    missionColor = invalidColor;
                else if( state.missionSelector_ )
                    missionColor = disabledColor;
            }
            else
                missionColor = std::find( state.disabledMissions_.begin(), state.disabledMissions_.end(), name ) == state.disabledMissions_.end()
                    ? defaultColor
                    : disabledColor;
            missionCombo_->setItemData( missionCombo_->count() - 1, missionColor, Qt::ForegroundRole );
        }
        // SELECT
        missionCombo_->setCurrentIndex( missionCombo_->findText( QString::fromStdString( state.currentMission_ ) ) );
        // DISABLE INVALID
        if( state.invalid_ || state.missionSelector_ )
            missionCombo_->setItemData( missionCombo_->currentIndex(), Qt::NoItemFlags, Qt::UserRole - 1 );
        missionCombo_->EnableStaticWarning( state.invalid_ );
        taskerWidget_->EnableStaticWarning( state.invalid_ );
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::AddReplaceTargetToMenu
// Created: ABR 2013-12-11
// -----------------------------------------------------------------------------
void EventOrderWidget::AddReplaceTargetToMenu( kernel::ContextMenu& menu )
{
    if( isVisible() &&
        ( !taskerWidget_->GetTasker() ||
          taskerWidget_->GetTasker()->GetTypeName() == selectedEntity_->GetTypeName() &&
          taskerWidget_->GetTasker()->GetId() != selectedEntity_->GetId() ) )
        menu.InsertItem( "Order", tr( "Replace order recipient" ), this, SLOT( OnReplaceTargetClicked() ), false, 4 );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::NotifyContextMenu
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu )
{
    selectedEntity_ = &agent;
    selectedEngagedAutomat_ = 0;
    if( const kernel::Automat_ABC* automat = static_cast< const kernel::Automat_ABC* >( agent.Get< kernel::TacticalHierarchies >().GetSuperior() ) )
    {
        if( tools::IsEngaged( *automat ) )
        {
            if( profile_.CanBeOrdered( *automat ) )
            {
                selectedEngagedAutomat_ = automat;
                QAction* action = menu.InsertItem( "Order", tr( "New order" ), this, SLOT( OnOrderAutomatClicked() ), false, 2 );
                action->setIcon( MAKE_PIXMAP( lock ) );
            }
            if( profile_.CanBeOrdered( agent ) )
            {
                menu.InsertItem( "Order", tr( "New order (unit)" ), this, SLOT( OnOrderClicked() ), false, 3 );
                AddReplaceTargetToMenu( menu );
            }
        }
        else if( profile_.CanBeOrdered( agent ) )
        {
            menu.InsertItem( "Order", tr( "New order" ), this, SLOT( OnOrderClicked() ), false, 2 );
            AddReplaceTargetToMenu( menu );
        }
        AddReplaceTargetToMenu( menu );
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::NotifyContextMenu
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu )
{
    selectedEntity_ = &automat;
    selectedEngagedAutomat_ = 0;
    if( profile_.CanBeOrdered( automat ) )
    {
        QAction* action = menu.InsertItem( "Order", tr( "New order" ), this, SLOT( OnOrderClicked() ), false, 2 );
        if( tools::IsEngaged( automat ) )
            action->setIcon( MAKE_PIXMAP( lock ) );
        AddReplaceTargetToMenu( menu );
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::NotifyContextMenu
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::NotifyContextMenu( const kernel::Population_ABC& population, kernel::ContextMenu& menu )
{
    selectedEntity_ = &population;
    selectedEngagedAutomat_ = 0;
    if( profile_.CanBeOrdered( population ) )
    {
        menu.InsertItem( "Mission", tr( "New order" ), this, SLOT( OnOrderClicked() ), false, 2 );
        AddReplaceTargetToMenu( menu );
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::NotifyDeleted
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    if( taskerWidget_->GetTasker() == &entity ||
        selectedEntity_ == &entity ||
        selectedEngagedAutomat_ == &entity )
    {
        OnTargetChanged( 0 );
        selectedEntity_ = 0;
        selectedEngagedAutomat_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::NotifyUpdated
// Created: LGY 2013-08-22
// -----------------------------------------------------------------------------
void EventOrderWidget::NotifyUpdated( const gui::Decisions_ABC& decisions )
{
    if( taskerWidget_->GetTasker() == &decisions.GetAgent() )
        OnTargetChanged( taskerWidget_->GetTasker() );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::NotifyUpdated
// Created: ABR 2013-11-13
// -----------------------------------------------------------------------------
void EventOrderWidget::NotifyUpdated( const actions::gui::Param_ABC& param )
{
    if( !missionInterface_->IsFilling() && missionInterface_->HasParameter( param ) )
        presenter_.OnEventContentChanged();
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::NotifyUpdated
// Created: LGY 2013-08-29
// -----------------------------------------------------------------------------
void EventOrderWidget::NotifyUpdated( const MissionParameters& extension )
{
    if( taskerWidget_->GetTasker() && taskerWidget_->GetTasker()->GetId() == extension.GetEntityId() )
    {
        const actions::Action_ABC* action = extension.GetLastMission();
        if( action && action->GetContext() == orderPresenter_->GetLastContext() )
        {
            bool valid = action->IsValid();
            std::string warningMsg = valid
                ? tr( "Order acknowledged" ).toStdString()
                : tr( "Error" ).toStdString();
            const QColor warningColor = valid ? Qt::darkGreen : Qt::red;
            if( !valid )
                if( const actions::ActionError* error = action->Retrieve< actions::ActionError >() )
                    if( !error->GetError().empty() )
                        warningMsg = error->GetError();
            presenter_.OnWarningChanged( warningMsg, warningColor );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::OnTargetChanged
// Created: ABR 2013-11-22
// -----------------------------------------------------------------------------
void EventOrderWidget::OnTargetChanged( const kernel::Entity_ABC* entity )
{
    orderPresenter_->OnTargetChanged( entity, GetDecisions( entity ) );
    presenter_.OnEventContentChanged();
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::OnClearTaskerClicked
// Created: ABR 2013-12-17
// -----------------------------------------------------------------------------
void EventOrderWidget::OnClearTaskerClicked()
{
    OnTargetChanged( 0 );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::OnOrderClicked
// Created: ABR 2013-11-21
// -----------------------------------------------------------------------------
void EventOrderWidget::OnOrderClicked( const kernel::Entity_ABC* entity )
{
    if( !entity )
        return;
    presenter_.StartCreation( eEventTypes_Order, simulation_.GetDateTime() );
    OnTargetChanged( entity );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::OnOrderClicked
// Created: ABR 2013-11-21
// -----------------------------------------------------------------------------
void EventOrderWidget::OnOrderClicked()
{
    OnOrderClicked( selectedEntity_ );
    selectedEntity_ = 0;
    selectedEngagedAutomat_ = 0;
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::OnOrderUnitClicked
// Created: ABR 2013-11-21
// -----------------------------------------------------------------------------
void EventOrderWidget::OnOrderAutomatClicked()
{
    OnOrderClicked( selectedEngagedAutomat_ );
    selectedEntity_ = 0;
    selectedEngagedAutomat_ = 0;
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::OnReplaceTargetClicked
// Created: ABR 2013-12-11
// -----------------------------------------------------------------------------
void EventOrderWidget::OnReplaceTargetClicked()
{
    OnTargetChanged( selectedEntity_ );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::OnMissionTypeChanged
// Created: ABR 2013-12-09
// -----------------------------------------------------------------------------
void EventOrderWidget::OnMissionTypeChanged( const QString& missionType )
{
    orderPresenter_->OnMissionTypeChanged( ENT_Tr::ConvertToMissionType( missionType.toStdString() ) );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::BlockSignals
// Created: ABR 2013-11-22
// -----------------------------------------------------------------------------
void EventOrderWidget::BlockSignals( bool blocked )
{
    missionCombo_->blockSignals( blocked );
    missionTypeCombo_->blockSignals( blocked );
    taskerWidget_->BlockSignals( blocked );
}
