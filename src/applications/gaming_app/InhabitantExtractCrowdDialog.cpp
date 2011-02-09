// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InhabitantExtractCrowdDialog.h"
#include "moc_InhabitantExtractCrowdDialog.cpp"

#include "actions/ActionsModel.h"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/UnitMagicAction.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/InhabitantType.h"
#include "clients_kernel/PopulationType.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/tools.h"
#include "gaming/Inhabitant.h"
#include "gaming/StaticModel.h"

// -----------------------------------------------------------------------------
// Name: InhabitantExtractCrowdDialog constructor
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
InhabitantExtractCrowdDialog::InhabitantExtractCrowdDialog( QWidget* pParent, kernel::Controllers& controllers, const StaticModel& staticModel, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile )
    : QDialog( pParent, tools::translate( "InhabitantExtractCrowdDialog", "Extract crowd" ) )
    , controllers_            ( controllers )
    , static_                 ( staticModel )
    , actionsModel_           ( actionsModel )
    , simulation_             ( simulation )
    , profile_                ( profile )
    , selected_               ( controllers )
    , originalInhabitantSize_ ( 0 )
{
    // Init dialog
    setCaption( tools::translate( "InhabitantExtractCrowdDialog", "Extract crowd" ) );
    resize( 250, 150 );
    // Main layout
    QGridLayout* grid = new QGridLayout( this, 8, 2, 5, 5 );
    // Type & SpinBoxs
    grid->addWidget( new QLabel( tools::translate( "InhabitantExtractCrowdDialog", "Crowd type:" ), this ), 0, 0 );
    crowdTypeLabel_ = new QLabel( "0", this );
    grid->addWidget( crowdTypeLabel_, 0, 1 );
    grid->addWidget( new QLabel( tools::translate( "InhabitantExtractCrowdDialog", "Alive:" ), this ), 1, 0 );
    healthySpinBox_ = new QSpinBox ( this );
    grid->addWidget( healthySpinBox_, 1, 1 );
    grid->addWidget( new QLabel( tools::translate( "InhabitantExtractCrowdDialog", "Wounded:" ), this ), 2, 0 );
    woundedSpinBox_ = new QSpinBox ( this );
    grid->addWidget( woundedSpinBox_, 2, 1 );
    grid->addWidget( new QLabel( tools::translate( "InhabitantExtractCrowdDialog", "Dead:" ), this ), 3, 0 );
    deadSizeLabel_ = new QLabel ( "0", this );
    grid->addWidget( deadSizeLabel_, 3, 1 );
    // Separator
    QFrame* hline = new QFrame( this );
    hline->setFrameStyle( QFrame::HLine | QFrame::Sunken );
    grid->addMultiCellWidget( hline, 4, 4, 0, 1 );
    // Labels
    grid->addWidget( new QLabel( tools::translate( "InhabitantExtractCrowdDialog", "Crowd size:" ), this ), 5, 0 );
    crowdSizeLabel_ = new QLabel( "0", this );
    grid->addWidget( crowdSizeLabel_, 5, 1 );
    grid->addWidget( new QLabel( tools::translate( "InhabitantExtractCrowdDialog", "Remaining population:" ), this ), 6, 0 );
    remainingInhabitantLabel_ = new QLabel( "0", this );
    grid->addWidget( remainingInhabitantLabel_, 6, 1 );
    // Configurations
    healthySpinBox_->setMinValue( 0 );
    healthySpinBox_->setMaxValue( std::numeric_limits< int >::max() );
    healthySpinBox_->setLineStep( 10 );
    woundedSpinBox_->setMinValue( 0 );
    woundedSpinBox_->setMaxValue( std::numeric_limits< int >::max() );
    woundedSpinBox_->setLineStep( 10 );
    crowdTypeLabel_->setAlignment( Qt::AlignCenter );
    crowdTypeLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );
    crowdSizeLabel_->setAlignment( Qt::AlignCenter );
    crowdSizeLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );
    remainingInhabitantLabel_->setAlignment( Qt::AlignCenter );
    remainingInhabitantLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );
    // Buttons
    QHBox* buttonLayout = new QHBox( this );
    QPushButton* okButton     = new QPushButton( tr( "Ok" )    , buttonLayout );
    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ), buttonLayout );
    grid->addMultiCellWidget( buttonLayout, 7, 7, 0, 1 );
    okButton->setDefault( TRUE );
    // Signals
    connect( okButton        , SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( cancelButton    , SIGNAL( clicked() ), SLOT( Reject() ) );
    connect( healthySpinBox_ , SIGNAL( valueChanged( int ) ), SLOT( OnValuesChanged( int ) ) );
    connect( woundedSpinBox_ , SIGNAL( valueChanged( int ) ), SLOT( OnValuesChanged( int ) ) );

    selected_ = 0;
    controllers_.Register( *this );
    hide();
}

// -----------------------------------------------------------------------------
// Name: InhabitantExtractCrowdDialog destructor
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
InhabitantExtractCrowdDialog::~InhabitantExtractCrowdDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InhabitantExtractCrowdDialog::Show
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantExtractCrowdDialog::Show()
{
    if( !selected_ )
        return;
    healthySpinBox_->setValue( 0 );
    healthySpinBox_->setMaxValue( selected_->GetHealthy() );
    healthySpinBox_->setEnabled( selected_->GetHealthy() != 0 );
    woundedSpinBox_->setValue( 0 );
    woundedSpinBox_->setMaxValue( selected_->GetWounded() );
    woundedSpinBox_->setEnabled( selected_->GetWounded() != 0 );
    QToolTip::add( healthySpinBox_, tools::translate( "InhabitantExtractCrowdDialog", QString( "Maximum %1" ).arg( selected_->GetHealthy() ) ) );
    QToolTip::add( woundedSpinBox_, tools::translate( "InhabitantExtractCrowdDialog", QString( "Maximum %1" ).arg( selected_->GetWounded() ) ) );
    deadSizeLabel_->setText( QString::number( selected_->GetDead() ) );
    crowdTypeLabel_->setText( static_cast< const Inhabitant& > ( *( selected_.ConstCast() ) ).GetType().GetCrowdType().GetName().c_str() );
    originalInhabitantSize_ = selected_->GetHealthy() + selected_->GetWounded() + selected_->GetDead();
    OnValuesChanged();
    show();
}

// -----------------------------------------------------------------------------
// Name: InhabitantExtractCrowdDialog::Validate
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantExtractCrowdDialog::Validate()
{
    if( ! selected_ )
        return;
    accept();
    // Throw Inhabitant_Change_Health_State Magic Action
    {
        actions::Action_ABC* action = actionsModel_.CreateInhabitantChangeHealthStateAction( selected_->GetHealthy() - healthySpinBox_->value(), selected_->GetWounded() - woundedSpinBox_->value(), selected_->GetDead(), 
                                                                                             *selected_, controllers_.controller_, static_.types_ );
        action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
        action->Attach( *new actions::ActionTasker( selected_, false ) );
        action->Polish();
        actionsModel_.Publish( *action );
    }
    // Throw Crowd_Creation Magic Action
    {
        const Inhabitant& entity = static_cast< const Inhabitant& > ( *( selected_.ConstCast() ) );
        const kernel::Entity_ABC& top = entity.Get< kernel::TacticalHierarchies >().GetTop();
        actions::Action_ABC* action = actionsModel_.CreateCrowdCreationAction( entity.GetType().GetCrowdType(), healthySpinBox_->value() + woundedSpinBox_->value(), entity.GetPosition(), top, 
                                                                               controllers_.controller_, static_.types_, static_.coordinateConverter_ );
        action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
        action->Attach( *new actions::ActionTasker( &top, false ) );
        action->Polish();
        actionsModel_.Publish( *action );
    }
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: InhabitantExtractCrowdDialog::Reject
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantExtractCrowdDialog::Reject()
{
    reject();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: InhabitantExtractCrowdDialog::closeEvent
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantExtractCrowdDialog::closeEvent( QCloseEvent * /* e */ )
{
    Reject();
}

// -----------------------------------------------------------------------------
// Name: InhabitantExtractCrowdDialog::OnValuesChanged
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantExtractCrowdDialog::OnValuesChanged( int /* newValue */ )
{
    crowdSizeLabel_->setText( QString::number( healthySpinBox_->value() + woundedSpinBox_->value() ) );
    remainingInhabitantLabel_->setText( QString::number( originalInhabitantSize_ - ( healthySpinBox_->value() + woundedSpinBox_->value() ) ) );
}

// -----------------------------------------------------------------------------
// Name: InhabitantExtractCrowdDialog::NotifyContextMenu
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
void InhabitantExtractCrowdDialog::NotifyContextMenu( const kernel::Inhabitant_ABC& entity, kernel::ContextMenu& menu )
{
    if( profile_.CanDoMagic( entity ) )
    {
        selected_ = &entity;
        QPopupMenu* subMenu = menu.SubMenu( "Order", tr( "Magic orders" ) );
        subMenu->insertItem( tools::translate( "InhabitantExtractCrowdDialog", "Extract crowd" ), this, SLOT( Show() ) );
    }
}
