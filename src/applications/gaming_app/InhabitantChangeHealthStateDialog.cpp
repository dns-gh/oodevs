// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InhabitantChangeHealthStateDialog.h"
#include "moc_InhabitantChangeHealthStateDialog.cpp"

#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/Quantity.h"
#include "actions/UnitMagicAction.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/tools.h"
#include "gaming/StaticModel.h"

// -----------------------------------------------------------------------------
// Name: InhabitantChangeHealthStateDialog constructor
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
InhabitantChangeHealthStateDialog::InhabitantChangeHealthStateDialog( QWidget* pParent, kernel::Controllers& controllers, const StaticModel& staticModel, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile )
    : QDialog( pParent, tools::translate( "InhabitantChangeHealthDialog", "Change health state" ) )
    , controllers_( controllers )
    , static_( staticModel )
    , actionsModel_( actionsModel )
    , simulation_( simulation )
    , profile_( profile )
    , selected_( controllers )
{
    // Init dialog
    setCaption( tools::translate( "InhabitantChangeHealthDialog", "Change health state" ) );
    resize( 250, 150 );

    // Health informations
    QGridLayout* grid = new QGridLayout( this, 6, 2, 5, 5 );

    grid->addWidget( new QLabel( tools::translate( "InhabitantChangeHealthDialog", "Alive:" ), this ), 0, 0 );
    healthySpinBox_ = new QSpinBox ( this );
    grid->addWidget( healthySpinBox_, 0, 1 );
    grid->addWidget( new QLabel( tools::translate( "InhabitantChangeHealthDialog", "Wounded:" ), this ), 1, 0 );
    woundedSpinBox_ = new QSpinBox ( this );
    grid->addWidget( woundedSpinBox_, 1, 1 );
    grid->addWidget( new QLabel( tools::translate( "InhabitantChangeHealthDialog", "Dead:" ), this ), 2, 0 );
    deadSpinBox_ = new QSpinBox ( this );
    grid->addWidget( deadSpinBox_, 2, 1 );

    grid->addWidget( new QLabel( tools::translate( "InhabitantChangeHealthDialog", "New total:" ), this ), 3, 0 );
    newTotalLabel_ = new QLabel( tools::translate( "InhabitantChangeHealthDialog", "0" ), this );
    grid->addWidget( newTotalLabel_, 3, 1 );
    grid->addWidget( new QLabel( tools::translate( "InhabitantChangeHealthDialog", "Original total:" ), this ), 4, 0 );
    originalTotalLabel_ = new QLabel( tools::translate( "InhabitantChangeHealthDialog", "0" ), this );
    grid->addWidget( originalTotalLabel_, 4, 1 );

    healthySpinBox_->setMinValue( 0 );
    healthySpinBox_->setMaxValue( std::numeric_limits< int >::max() );
    woundedSpinBox_->setMinValue( 0 );
    woundedSpinBox_->setMaxValue( std::numeric_limits< int >::max() );
    deadSpinBox_->setMinValue( 0 );
    deadSpinBox_->setMaxValue( std::numeric_limits< int >::max() );

    newTotalLabel_->setAlignment( Qt::AlignCenter );
    newTotalLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );
    originalTotalLabel_->setAlignment( Qt::AlignCenter );
    originalTotalLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );

    // ok / cancel butons
    QHBox* buttonLayout = new QHBox( this );
    QPushButton* okButton     = new QPushButton( tools::translate( "InhabitantChangeHealthDialog","Ok ")    , buttonLayout );
    QPushButton* cancelButton = new QPushButton( tools::translate( "InhabitantChangeHealthDialog","Cancel" ), buttonLayout );
    grid->addMultiCellWidget( buttonLayout, 5, 5, 0, 1 );
    okButton->setDefault( TRUE );

    connect( okButton        , SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( cancelButton    , SIGNAL( clicked() ), SLOT( Reject() ) );

    connect( healthySpinBox_ , SIGNAL( valueChanged( int ) ), SLOT( UpdateNewTotal( int ) ) );
    connect( woundedSpinBox_ , SIGNAL( valueChanged( int ) ), SLOT( UpdateNewTotal( int ) ) );
    connect( deadSpinBox_    , SIGNAL( valueChanged( int ) ), SLOT( UpdateNewTotal( int ) ) );

    // Base
    selected_ = 0;
    controllers_.Register( *this );
    hide();
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeHealthStateDialog destructor
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
InhabitantChangeHealthStateDialog::~InhabitantChangeHealthStateDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeHealthStateDialog::Show
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantChangeHealthStateDialog::Show()
{
    if( !selected_ )
        return;

    healthySpinBox_->setValue( selected_->GetHealty() );
    woundedSpinBox_->setValue( selected_->GetWounded() );
    deadSpinBox_->setValue( selected_->GetDead() );

    originalTotalLabel_->setText( QString::number( healthySpinBox_->value() + woundedSpinBox_->value() + deadSpinBox_->value() ) );
    UpdateNewTotal();

    show();
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeHealthStateDialog::Validate
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantChangeHealthStateDialog::Validate()
{
    if( ! selected_ )
        return;

    accept();

    kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( static_.types_ ).Get( "inhabitant_change_health_state" );

    actions::UnitMagicAction* action = new actions::UnitMagicAction( *selected_, actionType, controllers_.controller_, tools::translate( "FireCreationPanel", "Strike order on location" ), true );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();

    action->AddParameter( *new actions::parameters::Quantity( it.NextElement(), healthySpinBox_->value() ) );
    action->AddParameter( *new actions::parameters::Quantity( it.NextElement(), woundedSpinBox_->value() ) );
    action->AddParameter( *new actions::parameters::Quantity( it.NextElement(), deadSpinBox_->value() ) );

    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new actions::ActionTasker( selected_, false ) );
    action->RegisterAndPublish( actionsModel_ );

    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeHealthStateDialog::Reject
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantChangeHealthStateDialog::Reject()
{
    reject();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeHealthStateDialog::closeEvent
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantChangeHealthStateDialog::closeEvent( QCloseEvent * /* e */ )
{
    Reject();
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeHealthStateDialog::UpdateNewTotal
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantChangeHealthStateDialog::UpdateNewTotal( int /* newValue */ )
{
    newTotalLabel_->setText( QString::number( healthySpinBox_->value() + woundedSpinBox_->value() + deadSpinBox_->value() ) );
}

// -----------------------------------------------------------------------------
// Name: InhabitantChangeHealthStateDialog::NotifyContextMenu
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantChangeHealthStateDialog::NotifyContextMenu( const kernel::Inhabitant_ABC& entity, kernel::ContextMenu& menu )
{
    if( profile_.CanDoMagic( entity ) )
    {
        selected_ = &entity;
        QPopupMenu* subMenu = menu.SubMenu( "Order", tr( "Magic orders" ) );
        subMenu->insertItem( tools::translate( "InhabitantChangeHealthDialog", "Change health state" ), this, SLOT( Show() ) );
    }
}
