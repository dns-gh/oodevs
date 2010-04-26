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

#include "actions/DotationType.h"
#include "actions/Identifier.h"
#include "actions/Numeric.h"
#include "actions/UnitMagicAction.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentKnowledge_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/ObjectTypes.h"
#include "gaming/ActionTiming.h"
#include "gaming/StaticModel.h"
#include "protocol/SimulationSenders.h"
#include "tools/iterator.h"

using namespace kernel;
using namespace actions;

// -----------------------------------------------------------------------------
// Name: FireCreationPanel constructor
// Created: MGD 2010-02-23
// -----------------------------------------------------------------------------
FireCreationPanel::FireCreationPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers
                                    , actions::ActionsModel& actionsModel, const Simulation& simulation, const StaticModel& staticModel )
: gui::InfoPanel_ABC( parent, panel, tr( "Fire" ), "FireCreationPanel" )
    , staticModel_( staticModel )
    , controllers_( controllers )
    , actionsModel_( actionsModel )
    , simulation_( simulation ) 
    , potentialTarget_( 0 )
    , selectedTarget_( 0 )
    , potentialReporter_( 0 )
    , selectedReporter_( 0 )
    , targetLabel_( 0 )
    , reporterLabel_( 0 )
    , ammunitionsBox_( 0 )
    , interventionType_( 0 )
{
    layout()->setAlignment( Qt::AlignTop );
    QGroupBox* group = new QGroupBox( 2, Qt::Horizontal, tr( "FireOrder description" ), this );
    {
        new QLabel( tr( "Target:" ), group );
        QHBox* box = new QHBox( group );
        box->setSpacing( 5 );
        targetLabel_ = new QLabel( "---", box );
        targetLabel_->setMinimumWidth( 100 );
        targetLabel_->setAlignment( Qt::AlignCenter );
        targetLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );

        new QLabel( tr( "Reporter:" ), group );
        QHBox* box2 = new QHBox( group );
        box2->setSpacing( 5 );
        reporterLabel_ = new QLabel( "---", box2 );
        reporterLabel_->setMinimumWidth( 100 );
        reporterLabel_->setAlignment( Qt::AlignCenter );
        reporterLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );

        new QLabel( tr( "Munitions:" ), group );
        ammunitionsBox_ = new gui::ValuedComboBox< int >( group );
        
        new QLabel( tr( "IT:" ), group );
        interventionType_ = new QLineEdit( "0", group );
    }

    QPushButton* ok = new QPushButton( tr( "ApplyFire" ), this );
    connect( ok, SIGNAL( clicked() ), this, SLOT( Commit() ) );
    controllers_.Register( *this );
}
    
// -----------------------------------------------------------------------------
// Name: FireCreationPanel destructor
// Created: MGD 2010-02-23
// -----------------------------------------------------------------------------
FireCreationPanel::~FireCreationPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: FireCreationPanel::Commit
// Created: MGD 2010-02-23
// -----------------------------------------------------------------------------
void FireCreationPanel::Commit()
{
    MagicActionType& actionType = static_cast< tools::Resolver< MagicActionType, std::string >& > ( staticModel_.types_ ).Get( "fire_order" );
    UnitMagicAction* action = new UnitMagicAction( *selectedReporter_, actionType, controllers_.controller_, true );
    tools::Iterator< const OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Identifier( it.NextElement(), selectedTarget_->GetId() ) );
    action->AddParameter( *new parameters::DotationType( it.NextElement(), ammunitionsBox_->GetValue(), staticModel_.objectTypes_ ) );
    action->AddParameter( *new parameters::Numeric( it.NextElement(), interventionType_->text().toFloat() ) );
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
    action->RegisterAndPublish( actionsModel_ );
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
        if( type.IsDType() )
        {
            Common::MsgDotationType dot;
            dot.set_oid( type.GetId() );
            ammunitionsBox_->AddItem( type.GetCategory(), dot.oid() );
        }
    }

    Show();
}

// -----------------------------------------------------------------------------
// Name: FireCreationPanel::NotifyContextMenu
// Created: MGD 2010-02-23
// -----------------------------------------------------------------------------
void FireCreationPanel::NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu )
{
    if( isVisible() )
    {
        menu.InsertItem( "Parameter", "FireOrder : Reporter", this, SLOT( MenuItemReporterValidated() ) );
        potentialReporter_ = &entity;
    }
}

// -----------------------------------------------------------------------------
// Name: FireCreationPanel::NotifyContextMenu
// Created: MGD 2010-02-23
// -----------------------------------------------------------------------------
void FireCreationPanel::NotifyContextMenu( const kernel::AgentKnowledge_ABC& kn, kernel::ContextMenu& menu )
{
    if( isVisible() )
    {
        menu.InsertItem( "Parameter", "FireOrder : Target", this, SLOT( MenuItemTargetValidated() ) );
        potentialTarget_ = &kn;
    }
}

// -----------------------------------------------------------------------------
// Name: FireCreationPanel::MenuItemTargetValidated
// Created: MGD 2010-02-24
// -----------------------------------------------------------------------------
void FireCreationPanel::MenuItemTargetValidated()
{
    selectedTarget_ = potentialTarget_;
    targetLabel_->setText( selectedTarget_->GetName() );
}

// -----------------------------------------------------------------------------
// Name: FireCreationPanel::MenuItemReporterValidated
// Created: MGD 2010-02-24
// -----------------------------------------------------------------------------
void FireCreationPanel::MenuItemReporterValidated()
{
    selectedReporter_ = potentialReporter_;
    reporterLabel_->setText( selectedReporter_->GetName() );
}
