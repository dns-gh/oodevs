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

#include "actions/ActionsModel.h"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/UnitMagicAction.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/tools.h"
#include "gaming/StaticModel.h"

// -----------------------------------------------------------------------------
// Name: InhabitantChangeHealthStateDialog constructor
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
InhabitantChangeHealthStateDialog::InhabitantChangeHealthStateDialog( QWidget* pParent, kernel::Controllers& controllers, const StaticModel& staticModel, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile )
    : QDialog( pParent, tools::translate( "InhabitantChangeHealthStateDialog", "Change health state" ) )
    , controllers_  ( controllers )
    , static_       ( staticModel )
    , actionsModel_ ( actionsModel )
    , simulation_   ( simulation )
    , profile_      ( profile )
    , selected_     ( controllers )
{
    // Init dialog
    setCaption( tools::translate( "InhabitantChangeHealthStateDialog", "Change health state" ) );
    resize( 250, 150 );
    // Main layout
    QGridLayout* grid = new QGridLayout( this, 7, 2, 5, 5 );
    // SpinBoxs
    grid->addWidget( new QLabel( tools::translate( "InhabitantChangeHealthStateDialog", "Alive:" ), this ), 0, 0 );
    healthySpinBox_ = new QSpinBox ( this );
    grid->addWidget( healthySpinBox_, 0, 1 );
    grid->addWidget( new QLabel( tools::translate( "InhabitantChangeHealthStateDialog", "Wounded:" ), this ), 1, 0 );
    woundedSpinBox_ = new QSpinBox ( this );
    grid->addWidget( woundedSpinBox_, 1, 1 );
    grid->addWidget( new QLabel( tools::translate( "InhabitantChangeHealthStateDialog", "Dead:" ), this ), 2, 0 );
    deadSpinBox_ = new QSpinBox ( this );
    grid->addWidget( deadSpinBox_, 2, 1 );
    // Separator
    QFrame* hline = new QFrame( this );
    hline->setFrameStyle( QFrame::HLine | QFrame::Sunken );
    grid->addMultiCellWidget( hline, 3, 3, 0, 1 );
    // Labels
    grid->addWidget( new QLabel( tools::translate( "InhabitantChangeHealthStateDialog", "New total:" ), this ), 4, 0 );
    newTotalLabel_ = new QLabel( "0", this );
    grid->addWidget( newTotalLabel_, 4, 1 );
    grid->addWidget( new QLabel( tools::translate( "InhabitantChangeHealthStateDialog", "Original total:" ), this ), 5, 0 );
    originalTotalLabel_ = new QLabel( "0", this );
    grid->addWidget( originalTotalLabel_, 5, 1 );
    //Configurations
    healthySpinBox_->setMinValue( 0 );
    healthySpinBox_->setMaxValue( std::numeric_limits< int >::max() );
    healthySpinBox_->setLineStep( 10 );
    woundedSpinBox_->setMinValue( 0 );
    woundedSpinBox_->setMaxValue( std::numeric_limits< int >::max() );
    woundedSpinBox_->setLineStep( 10 );
    deadSpinBox_->setMinValue( 0 );
    deadSpinBox_->setMaxValue( std::numeric_limits< int >::max() );
    deadSpinBox_->setLineStep( 10 );
    newTotalLabel_->setAlignment( Qt::AlignCenter );
    newTotalLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );
    originalTotalLabel_->setAlignment( Qt::AlignCenter );
    originalTotalLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );
    // Buttons
    QHBox* buttonLayout = new QHBox( this );
    QPushButton* okButton     = new QPushButton( tr( "Ok" )    , buttonLayout );
    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ), buttonLayout );
    grid->addMultiCellWidget( buttonLayout, 6, 6, 0, 1 );
    okButton->setDefault( TRUE );
    // Signals
    connect( okButton        , SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( cancelButton    , SIGNAL( clicked() ), SLOT( Reject() ) );
    connect( healthySpinBox_ , SIGNAL( valueChanged( int ) ), SLOT( OnValuesChanged( int ) ) );
    connect( woundedSpinBox_ , SIGNAL( valueChanged( int ) ), SLOT( OnValuesChanged( int ) ) );
    connect( deadSpinBox_    , SIGNAL( valueChanged( int ) ), SLOT( OnValuesChanged( int ) ) );

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
    healthySpinBox_->setValue( selected_->GetHealthy() );
    woundedSpinBox_->setValue( selected_->GetWounded() );
    deadSpinBox_->setValue( selected_->GetDead() );
    originalTotalLabel_->setText( QString::number( healthySpinBox_->value() + woundedSpinBox_->value() + deadSpinBox_->value() ) );
    OnValuesChanged();
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
    actions::Action_ABC* action = actionsModel_.CreateInhabitantChangeHealthStateAction( healthySpinBox_->value(), woundedSpinBox_->value(), deadSpinBox_->value(), *selected_, controllers_.controller_, static_.types_ );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new actions::ActionTasker( selected_, false ) );
    action->Polish();
    actionsModel_.Publish( *action );
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
// Name: InhabitantChangeHealthStateDialog::OnValuesChanged
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantChangeHealthStateDialog::OnValuesChanged( int /* newValue */ )
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
        subMenu->insertItem( tools::translate( "InhabitantChangeHealthStateDialog", "Change health state" ), this, SLOT( Show() ) );
    }
}
