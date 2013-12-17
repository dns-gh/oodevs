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
#include "actions/ActionError.h"
#include "actions/ActionWithTarget_ABC.h"
#include "actions_gui/MissionInterface.h"
#include "clients_gui/GLToolColors.h"
#include "clients_gui/ImageWrapper.h"
#include "clients_gui/RichGroupBox.h"
#include "clients_gui/EntitySymbols.h"
#include "clients_gui/EventPresenter.h"
#include "clients_gui/EventOrderPresenter.h"
#include "clients_gui/EventOrderViewState.h"
#include "clients_gui/EventViewState.h"
#include "clients_gui/RichLabel.h"
#include "clients_gui/RichPushButton.h"
#include "clients_gui/RichWarnWidget.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatDecisions_ABC.h"
#include "clients_kernel/EventAction.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Time_ABC.h"
#include "clients_kernel/TimelinePublisher.h"
#include "ENT/ENT_Tr.h"
#include "gaming/AgentsModel.h"
#include "gaming/AutomatDecisions.h"
#include "gaming/Decisions.h"
#include "gaming/Model.h"
#include "gaming/PopulationDecisions.h"
#include "gaming/StaticModel.h"
#include "gaming/MissionParameters.h"
#include "tools/GeneralConfig.h"
#include <timeline/api.h>
#include <boost/assign/list_of.hpp>
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
    , entitySymbols_( entitySymbols )
    , target_( controllers )
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

    // Top
    missionTypeCombo_ = new gui::RichWarnWidget< QComboBox >( "event-order-mission-type-combobox" );
    missionComboLayout_ = new QVBoxLayout();
    missionComboLayout_->setMargin( 0 );
    missionComboLayout_->setSpacing( 0 );
    targetLabel_ = new gui::RichLabel( "event-order-target-label", "---" );
    symbolLabel_ = new gui::RichLabel( "event-order-target-symbol-label" );
    activateTargetButton_ = new gui::RichPushButton( "activateTargetButton", gui::Icon( tools::GeneralConfig::BuildResourceChildFile( "images/gaming/center_time.png" ) ), "" );
    activateTargetButton_->setToolTip( tr( "Select" ) );
    connect( activateTargetButton_, SIGNAL( clicked() ), this, SLOT( OnTargetActivated() ) );

    removeTargetButton_ = new gui::RichPushButton( "removeTargetButton", qApp->style()->standardIcon( QStyle::SP_DialogCloseButton ), "" );
    removeTargetButton_->setToolTip( tr( "Remove" ) );
    connect( removeTargetButton_, SIGNAL( clicked() ), this, SLOT( OnTargetRemoved() ) );

    missionCombo_ = new gui::RichWarnWidget< QComboBox >( "event-order-mission-combobox" );
    QSortFilterProxyModel* proxy = new QSortFilterProxyModel( missionCombo_ );
    proxy->setSourceModel( missionCombo_->model() );
    missionCombo_->model()->setParent( proxy );
    missionCombo_->setModel( proxy );
    missionComboLayout_->addWidget( missionCombo_ );

    targetGroupBox_ = new gui::RichGroupBox( "event-order-target-groupbox", tr( "Recipient" ) );
    QHBoxLayout* internalTargetLayout = new QHBoxLayout( targetGroupBox_ );
    internalTargetLayout->setContentsMargins( 5, 0, 5, 5 );
    QWidget* symbolWidget = new QWidget();
    QHBoxLayout* symbolLayout = new QHBoxLayout( symbolWidget );
    symbolLayout->setMargin( 0 );
    symbolLayout->addWidget( symbolLabel_ );
    symbolLayout->addWidget( targetLabel_ );
    internalTargetLayout->addWidget( symbolWidget, 10, Qt::AlignCenter );
    internalTargetLayout->addWidget( activateTargetButton_, 1, Qt::AlignRight );
    internalTargetLayout->addWidget( removeTargetButton_, 1, Qt::AlignRight );

    QHBoxLayout* targetLayout = new QHBoxLayout();
    targetLayout->addWidget( targetGroupBox_ );

    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->setSpacing( 5 );
    topLayout->addWidget( missionTypeCombo_ );
    topLayout->addLayout( missionComboLayout_ );

    // Layout
    mainLayout_->setSpacing( 5 );
    mainLayout_->addLayout( targetLayout );
    mainLayout_->addLayout( topLayout );
    mainLayout_->addWidget( static_cast< actions::gui::MissionInterface* >( missionInterface_.get() ), 1 );

    // Connections
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
    SetTarget( 0 );
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
    const kernel::Decisions_ABC* GetDecisions( const kernel::Entity_ABC* entity )
    {
        const kernel::Decisions_ABC* decisions = 0;
        if( entity )
        {
            if( entity->GetTypeName() == kernel::Agent_ABC::typeName_ )
                decisions = entity->Retrieve< Decisions >();
            if( entity->GetTypeName() == kernel::Automat_ABC::typeName_ )
                decisions = static_cast< const AutomatDecisions* >( entity->Retrieve< kernel::AutomatDecisions_ABC >() );
            if( entity->GetTypeName() == kernel::Population_ABC::typeName_ )
                decisions = entity->Retrieve< PopulationDecisions >();
        }
        return decisions;
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
    const kernel::EventAction& eventAction = static_cast< const kernel::EventAction& >( *state.event_ );
    if( const actions::Action_ABC* action = eventAction.GetAction() )
    {
        const actions::ActionWithTarget_ABC* mission = static_cast< const actions::ActionWithTarget_ABC* >( action );
        SetTarget( mission->GetEntityId() );
        orderPresenter_->FillFromAction( *action, eventAction.GetMissionType(), target_, GetDecisions( target_ ) );
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::Build
// Created: LGY 2013-08-29
// -----------------------------------------------------------------------------
void EventOrderWidget::Build( const gui::EventOrderViewState& state )
{
    // Target
    SetTarget( state.target_ );
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
        targetGroupBox_->EnableStaticWarning( state.invalid_ );
        targetLabel_->EnableStaticWarning( state.invalid_ );
    }
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::AddReplaceTargetToMenu
// Created: ABR 2013-12-11
// -----------------------------------------------------------------------------
void EventOrderWidget::AddReplaceTargetToMenu( kernel::ContextMenu& menu )
{
    if( isVisible() &&
        ( !target_ ||
          target_->GetTypeName() == selectedEntity_->GetTypeName() &&
          target_->GetId() != selectedEntity_->GetId() ) )
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
        if( const kernel::AutomatDecisions_ABC* decisions = automat->Retrieve< kernel::AutomatDecisions_ABC >() )
        {
            if( decisions->IsEmbraye() )
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
        const kernel::AutomatDecisions_ABC* decisions = automat.Retrieve< kernel::AutomatDecisions_ABC >();
        QAction* action = menu.InsertItem( "Order", tr( "New order" ), this, SLOT( OnOrderClicked() ), false, 2 );
        if( decisions && decisions->IsEmbraye() )
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
    if( target_ == &entity ||
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
void EventOrderWidget::NotifyUpdated( const kernel::Decisions_ABC& decisions )
{
    if( target_ == &decisions.GetAgent() )
        OnTargetChanged( target_ );
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
    if( target_ && target_->GetId() == extension.GetEntityId() )
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
// Name: EventOrderWidget::SetTarget
// Created: ABR 2013-06-06
// -----------------------------------------------------------------------------
void EventOrderWidget::SetTarget( unsigned long id )
{
    target_ = model_.agents_.FindAllAgent( id );
    bool hasTarget = target_ != 0;
    targetLabel_->setText( hasTarget ? target_->GetName() : "---" );
    activateTargetButton_->setEnabled( hasTarget );
    removeTargetButton_->setEnabled( hasTarget );
    QPixmap pixmap;
    if( hasTarget )
        if( auto symbol = target_->Retrieve< kernel::TacticalHierarchies >() )
        {
            pixmap = entitySymbols_.GetSymbol( *target_, symbol->GetSymbol(), symbol->GetLevel(),
                                                QSize( 64, 64 ), gui::EntitySymbols::eColorWithModifier );
            pixmap = pixmap.scaled( QSize( 48, 48 ), Qt::KeepAspectRatio, Qt::SmoothTransformation );
        }
    symbolLabel_->setPixmap( pixmap );
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
// Name: EventOrderWidget::OnTargetRemoved
// Created: NPT 2013-07-30
// -----------------------------------------------------------------------------
void EventOrderWidget::OnTargetRemoved()
{
    if( !target_ )
        throw MASA_EXCEPTION( "Can't remove an unset target" );
    OnTargetChanged( 0 );
}

// -----------------------------------------------------------------------------
// Name: EventOrderWidget::OnTargetActivated
// Created: ABR 2013-10-31
// -----------------------------------------------------------------------------
void EventOrderWidget::OnTargetActivated() const
{
    if( !target_ )
        throw MASA_EXCEPTION( "Can't activate an unset target" );
    target_->Select( controllers_.actions_ );
    target_->MultipleSelect( controllers_.actions_, boost::assign::list_of( target_ ) );
    target_->Activate( controllers_.actions_ );
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
    activateTargetButton_->blockSignals( blocked );
    removeTargetButton_->blockSignals( blocked );
}
