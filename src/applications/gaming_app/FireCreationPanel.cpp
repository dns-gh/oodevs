// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
// LTO
// *****************************************************************************

#include "gaming_app_pch.h"
#include "FireCreationPanel.h"
#include "moc_FireCreationPanel.cpp"
#include "actions/ActionsModel.h"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/DotationType.h"
#include "actions/Identifier.h"
#include "actions/Numeric.h"
#include "actions/UnitMagicAction.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/LocationCreator.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentKnowledge_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/ObjectTypes.h"
#include "gaming/StaticModel.h"
#include "protocol/SimulationSenders.h"
#include <tools/Iterator.h>

using namespace actions;

// -----------------------------------------------------------------------------
// Name: FireCreationPanel constructor
// Created: MGD 2010-02-23
// -----------------------------------------------------------------------------
FireCreationPanel::FireCreationPanel( QWidget* parent,
                                      ::gui::PanelStack_ABC& panel,
                                      kernel::Controllers& controllers,
                                      actions::ActionsModel& actionsModel,
                                      const kernel::Time_ABC& simulation,
                                      const StaticModel& staticModel,
                                      const std::shared_ptr< ::gui::ParametersLayer >& paramLayer,
                                      const gui::GLView_ABC& tools )
    : ::gui::InfoPanel_ABC( parent, panel, tools::translate( "FireCreationPanel", "Strike" ), "FireCreationPanel" )
    , staticModel_( staticModel )
    , controllers_( controllers )
    , actionsModel_( actionsModel )
    , simulation_( simulation )
    , tools_ ( tools )
    , potentialTarget_( 0 )
    , selectedTarget_( 0 )
    , potentialReporter_( 0 )
    , selectedReporter_( 0 )
    , targetLabel_( 0 )
    , reporterLabel_( 0 )
    , ammunitionsBox_( 0 )
    , interventionType_( 0 )
    , location_( 0 )
    , isLocationRegistered_ ( false )
{
    Q3VBox* box = new Q3VBox( this );
    Q3GroupBox* group = new Q3GroupBox( 2, Qt::Horizontal, tools::translate( "FireCreationPanel", "Strike order description" ), box );
    {
        new QLabel( tools::translate( "FireCreationPanel", "Strike type:" ), group );
        strikeCombo_ = new ::gui::ValuedComboBox< E_StrikeType >( "strikeCombo", group );
        strikeCombo_->AddItem( tools::translate( "FireCreationPanel", "Strike on unit" ), eStrikeOnUnit);
        strikeCombo_->AddItem( tools::translate( "FireCreationPanel", "Strike on position" ), eStrikeOnPosition);
        connect( strikeCombo_, SIGNAL( activated( int ) ), this, SLOT( OnTypeChanged() ) );

        locationTitle_ = new QLabel( tools::translate( "FireCreationPanel", "Location target:" ), group );
        locationBox_ = new Q3HBox( group );
        locationBox_->setSpacing( 5 );
        locationLabel_ = new QLabel( tools::translate( "FireCreationPanel", "---" ), locationBox_ );
        locationLabel_->setMinimumWidth( 100 );
        locationLabel_->setAlignment( Qt::AlignCenter );
        locationLabel_->setFrameStyle( Q3Frame::Box | Q3Frame::Sunken );
        locationCreator_ = new ::gui::LocationCreator( group, tools::translate( "FireCreationPanel", "Strike order: location target" ), paramLayer, *this );
        locationCreator_->Allow( true, false, false, false, false );

        targetTitle_ = new QLabel( tools::translate( "FireCreationPanel", "Target:" ), group );
        targetBox_ = new Q3HBox( group );
        targetBox_->setSpacing( 5 );
        targetLabel_ = new QLabel( tools::translate( "FireCreationPanel", "---" ), targetBox_ );
        targetLabel_->setMinimumWidth( 100 );
        targetLabel_->setAlignment( Qt::AlignCenter );
        targetLabel_->setFrameStyle( Q3Frame::Box | Q3Frame::Sunken );

        reporterTitle_ = new QLabel( tools::translate( "FireCreationPanel", "Reporter:" ), group );
        reporterBox_ = new Q3HBox( group );
        reporterBox_->setSpacing( 5 );
        reporterLabel_ = new QLabel( tools::translate( "FireCreationPanel", "---" ), reporterBox_ );
        reporterLabel_->setMinimumWidth( 100 );
        reporterLabel_->setAlignment( Qt::AlignCenter );
        reporterLabel_->setFrameStyle( Q3Frame::Box | Q3Frame::Sunken );

        new QLabel( tools::translate( "FireCreationPanel", "Ammunition:" ), group );
        ammunitionsBox_ = new ::gui::ValuedComboBox< int >( "ammunitionsBox", group );
        ammunitionsBox_->SetSorting( true );

        new QLabel( tools::translate( "FireCreationPanel", "Iteration:" ), group );
        interventionType_ = new QLineEdit( "1", group );
        connect( interventionType_, SIGNAL( textChanged( const QString & ) ), this, SLOT( UpdateCommitButton() ) );
    }
    setWidget( box );
    ok_ = new QPushButton( tools::translate( "FireCreationPanel", "Launch strike" ), group );
    connect( ok_, SIGNAL( clicked() ), this, SLOT( Commit() ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: FireCreationPanel destructor
// Created: MGD 2010-02-23
// -----------------------------------------------------------------------------
FireCreationPanel::~FireCreationPanel()
{
    UnregisterIfNeeded();
    Reset();
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: FireCreationPanel::Register
// Created: ABR 2011-01-19
// -----------------------------------------------------------------------------
void FireCreationPanel::RegisterIfNeeded()
{
    if( !isLocationRegistered_ )
    {
        controllers_.Register( *locationCreator_ );
        isLocationRegistered_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: FireCreationPanel::Unregister
// Created: ABR 2011-01-19
// -----------------------------------------------------------------------------
void FireCreationPanel::UnregisterIfNeeded()
{
    if( locationCreator_ && isLocationRegistered_ )
    {
        controllers_.Unregister( *locationCreator_ );
        isLocationRegistered_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: FireCreationPanel::showEvent
// Created: ABR 2011-01-18
// -----------------------------------------------------------------------------
void FireCreationPanel::showEvent( QShowEvent* e )
{
    OnTypeChanged();
    QScrollArea::showEvent( e );
}

// -----------------------------------------------------------------------------
// Name: FireCreationPanel::hideEvent
// Created: ABR 2011-01-18
// -----------------------------------------------------------------------------
void FireCreationPanel::hideEvent( QHideEvent* )
{
    Reset();
    UnregisterIfNeeded();
}

// -----------------------------------------------------------------------------
// Name: FireCreationPanel::IsStrikeOnLocation
// Created: ABR 2011-01-19
// -----------------------------------------------------------------------------
bool FireCreationPanel::IsStrikeOnLocation() const
{
    return ( strikeCombo_->GetValue() == eStrikeOnPosition );
}

// -----------------------------------------------------------------------------
// Name: FireCreationPanel::CheckValidity
// Created: ABR 2011-01-18
// -----------------------------------------------------------------------------
bool FireCreationPanel::CheckValidity() const
{
    return ( ( ( selectedTarget_ && selectedReporter_ ) || ( location_ && location_->IsValid() ) )
            && interventionType_->text().toFloat() > 0 );
}

// -----------------------------------------------------------------------------
// Name: FireCreationPanel::UpdateCommitButton
// Created: LDC 2010-07-30
// -----------------------------------------------------------------------------
void FireCreationPanel::UpdateCommitButton()
{
    ok_->setEnabled( CheckValidity() );
}

// -----------------------------------------------------------------------------
// Name: FireCreationPanel::Commit
// Created: MGD 2010-02-23
// -----------------------------------------------------------------------------
void FireCreationPanel::Commit()
{
    if( CheckValidity() )
    {
        if( IsStrikeOnLocation() )
            actionsModel_.PublishFireOrderOnLocation( ammunitionsBox_->GetValue(), *location_, interventionType_->text().toFloat() );
        else
        {
            kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( staticModel_.types_ ).Get( "fire_order" );
            std::unique_ptr< Action_ABC > action( new UnitMagicAction( actionType, controllers_.controller_, false ) );
            tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();

            action->AddParameter( *new parameters::Identifier( it.NextElement(), selectedTarget_ ) );
            action->AddParameter( *new parameters::DotationType( it.NextElement(), ammunitionsBox_->GetValue(), staticModel_.objectTypes_ ) );
            action->AddParameter( *new parameters::Numeric( it.NextElement(), interventionType_->text().toFloat() ) );
            action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
            action->Attach( *new ActionTasker( controllers_.controller_, selectedReporter_, false ) );
            actionsModel_.Publish( *action );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: FireCreationPanel::NotifyUpdated
// Created: MGD 2010-02-23
// -----------------------------------------------------------------------------
void FireCreationPanel::NotifyUpdated( const kernel::ModelLoaded& )
{
    tools::Iterator< const kernel::DotationType& > it = staticModel_.objectTypes_.Resolver2< kernel::DotationType >::CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::DotationType& type = it.NextElement();
        if( type.IsIndirectFireAmmunition() )
            ammunitionsBox_->AddItem( type.GetName(), type.GetId() );
    }
    Show();
}

// -----------------------------------------------------------------------------
// Name: FireCreationPanel::NotifyUpdated
// Created: JSR 2011-02-02
// -----------------------------------------------------------------------------
void FireCreationPanel::NotifyUpdated( const kernel::ModelUnLoaded& )
{
    ammunitionsBox_->Clear();
}

// -----------------------------------------------------------------------------
// Name: FireCreationPanel::NotifyContextMenu
// Created: MGD 2010-02-23
// -----------------------------------------------------------------------------
void FireCreationPanel::NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu )
{
    if( isVisible() && !IsStrikeOnLocation() )
    {
        menu.InsertItem( "Parameter", tools::translate( "FireCreationPanel", "Strike order: reporter" ), this, SLOT( MenuItemReporterValidated() ) );
        potentialReporter_ = &entity;
    }
}

// -----------------------------------------------------------------------------
// Name: FireCreationPanel::NotifyContextMenu
// Created: MGD 2010-02-23
// -----------------------------------------------------------------------------
void FireCreationPanel::NotifyContextMenu( const kernel::AgentKnowledge_ABC& kn, kernel::ContextMenu& menu )
{
    if( isVisible() && !IsStrikeOnLocation() )
    {
        menu.InsertItem( "Parameter", tools::translate( "FireCreationPanel", "Strike order: unit target" ), this, SLOT( MenuItemTargetValidated() ) );
        potentialTarget_ = kn.GetId();
        potentialTargetName_ = kn.GetName();
    }
}

// -----------------------------------------------------------------------------
// Name: FireCreationPanel::MenuItemTargetValidated
// Created: MGD 2010-02-24
// -----------------------------------------------------------------------------
void FireCreationPanel::MenuItemTargetValidated()
{
    selectedTarget_ = potentialTarget_;
    targetLabel_->setText( potentialTargetName_ );
    UpdateCommitButton();
}

// -----------------------------------------------------------------------------
// Name: FireCreationPanel::MenuItemReporterValidated
// Created: MGD 2010-02-24
// -----------------------------------------------------------------------------
void FireCreationPanel::MenuItemReporterValidated()
{
    selectedReporter_ = potentialReporter_;
    reporterLabel_->setText( selectedReporter_->GetName() );
    UpdateCommitButton();
}

// -----------------------------------------------------------------------------
// Name: FireCreationPanel::Handle
// Created: ABR 2011-01-18
// -----------------------------------------------------------------------------
void FireCreationPanel::Handle( kernel::Location_ABC& location )
{
    Reset();
    location_ = &location;
    if( location.IsValid() )
        locationLabel_->setText( location.GetName() );
    UpdateCommitButton();
}

// -----------------------------------------------------------------------------
// Name: FireCreationPanel::Reset
// Created: ABR 2011-01-18
// -----------------------------------------------------------------------------
void FireCreationPanel::Reset()
{
    potentialTarget_ = 0;
    selectedTarget_ = 0;
    potentialReporter_ = 0;
    selectedReporter_ = 0;
    if( location_ )
    {
        delete location_;
        location_ = 0;
    }
    targetLabel_->setText( tools::translate( "FireCreationPanel", "---" ) );
    reporterLabel_->setText( tools::translate( "FireCreationPanel", "---" ) );
    locationLabel_->setText( tools::translate( "FireCreationPanel", "---" ) );
}

// -----------------------------------------------------------------------------
// Name: FireCreationPanel::Draw
// Created: ABR 2011-01-18
// -----------------------------------------------------------------------------
void FireCreationPanel::Draw( gui::Viewport_ABC& /*viewport*/ )
{
    if( isVisible() && location_ && location_->IsValid() )
    {
        ::gui::ShapeHandler_ABC::Draw( *location_, geometry::Rectangle2f(), tools_ );
    }
}

// -----------------------------------------------------------------------------
// Name: FireCreationPanel::OnTypeChanged
// Created: ABR 2011-01-18
// -----------------------------------------------------------------------------
void FireCreationPanel::OnTypeChanged()
{
    Reset();
    UpdateCommitButton();

    if( IsStrikeOnLocation() )
    {
        locationTitle_->show();
        locationBox_->show();
        targetTitle_->hide();
        targetBox_->hide();
        reporterTitle_->hide();
        reporterBox_->hide();
        RegisterIfNeeded();
    }
    else
    {
        locationTitle_->hide();
        locationBox_->hide();
        targetTitle_->show();
        targetBox_->show();
        reporterTitle_->show();
        reporterBox_->show();
        UnregisterIfNeeded();
    }
}
