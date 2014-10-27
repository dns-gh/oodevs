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
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_gui/EntityType.h"
#include "clients_kernel/tools.h"
#include "clients_gui/RichSpinBox.h"
#include "gaming/Inhabitant.h"

// -----------------------------------------------------------------------------
// Name: InhabitantExtractCrowdDialog constructor
// Created: ABR 2011-01-25
// -----------------------------------------------------------------------------
InhabitantExtractCrowdDialog::InhabitantExtractCrowdDialog( QWidget* pParent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile )
    : QDialog( pParent, tools::translate( "InhabitantExtractCrowdDialog", "Extract crowd" ) )
    , controllers_            ( controllers )
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
    Q3GridLayout* grid = new Q3GridLayout( this, 8, 2, 5, 5 );
    // Type & SpinBoxs
    grid->addWidget( new QLabel( tools::translate( "InhabitantExtractCrowdDialog", "Crowd type:" ), this ), 0, 0 );
    crowdTypeLabel_ = new QLabel( "0", this );
    grid->addWidget( crowdTypeLabel_, 0, 1 );
    grid->addWidget( new QLabel( tools::translate( "InhabitantExtractCrowdDialog", "Alive:" ), this ), 1, 0 );
    healthySpinBox_ = new gui::RichSpinBox( "healthySpinBox", this, 0, std::numeric_limits< int >::max(), 10 );
    grid->addWidget( healthySpinBox_, 1, 1 );
    grid->addWidget( new QLabel( tools::translate( "InhabitantExtractCrowdDialog", "Wounded:" ), this ), 2, 0 );
    woundedSpinBox_ = new gui::RichSpinBox( "woundedSpinBox", this, 0, std::numeric_limits< int >::max(), 10 );
    grid->addWidget( woundedSpinBox_, 2, 1 );
    grid->addWidget( new QLabel( tools::translate( "InhabitantExtractCrowdDialog", "Dead:" ), this ), 3, 0 );
    deadSpinBox_ = new gui::RichSpinBox( "deadSpinBox", this, 0, std::numeric_limits< int >::max(), 10 );
    grid->addWidget( deadSpinBox_, 3, 1 );
    // Separator
    Q3Frame* hline = new Q3Frame( this );
    hline->setFrameStyle( Q3Frame::HLine | Q3Frame::Sunken );
    grid->addMultiCellWidget( hline, 4, 4, 0, 1 );
    // Labels
    grid->addWidget( new QLabel( tools::translate( "InhabitantExtractCrowdDialog", "Crowd size:" ), this ), 5, 0 );
    crowdSizeLabel_ = new QLabel( "0", this );
    grid->addWidget( crowdSizeLabel_, 5, 1 );
    grid->addWidget( new QLabel( tools::translate( "InhabitantExtractCrowdDialog", "Remaining population:" ), this ), 6, 0 );
    remainingInhabitantLabel_ = new QLabel( "0", this );
    grid->addWidget( remainingInhabitantLabel_, 6, 1 );
    // Configurations
    crowdTypeLabel_->setAlignment( Qt::AlignCenter );
    crowdTypeLabel_->setFrameStyle( Q3Frame::Box | Q3Frame::Sunken );
    crowdSizeLabel_->setAlignment( Qt::AlignCenter );
    crowdSizeLabel_->setFrameStyle( Q3Frame::Box | Q3Frame::Sunken );
    remainingInhabitantLabel_->setAlignment( Qt::AlignCenter );
    remainingInhabitantLabel_->setFrameStyle( Q3Frame::Box | Q3Frame::Sunken );
    // Buttons
    Q3HBox* buttonLayout = new Q3HBox( this );
    QPushButton* okButton     = new QPushButton( tr( "Ok" )    , buttonLayout );
    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ), buttonLayout );
    grid->addMultiCellWidget( buttonLayout, 7, 7, 0, 1 );
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
    deadSpinBox_->setValue( 0 );
    deadSpinBox_->setMaxValue( selected_->GetDead() );
    deadSpinBox_->setEnabled( selected_->GetDead() != 0 );
    QToolTip::add( healthySpinBox_, tools::translate( "InhabitantExtractCrowdDialog", QString( "Maximum %L1" ).arg( selected_->GetHealthy() ) ) );
    QToolTip::add( woundedSpinBox_, tools::translate( "InhabitantExtractCrowdDialog", QString( "Maximum %L1" ).arg( selected_->GetWounded() ) ) );
    QToolTip::add( deadSpinBox_,    tools::translate( "InhabitantExtractCrowdDialog", QString( "Maximum %L1" ).arg( selected_->GetDead() ) ) );
    crowdTypeLabel_->setText( selected_->Get< gui::EntityType< kernel::InhabitantType > >().GetType().GetCrowdType().GetName().c_str() );
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
    actionsModel_.PublishInhabitantChangeHealthStateAction( selected_->GetHealthy() - healthySpinBox_->value(), selected_->GetWounded() - woundedSpinBox_->value(), selected_->GetDead() - deadSpinBox_->value(), *selected_ );

    // Throw Crowd_Creation Magic Action
    {
        const Inhabitant& entity = static_cast< const Inhabitant& > ( *( selected_.ConstCast() ) );
        const kernel::Entity_ABC& top = entity.Get< kernel::TacticalHierarchies >().GetTop();
        actionsModel_.PublishCrowdCreationAction( entity.Get< gui::EntityType< kernel::InhabitantType > >().GetType().GetCrowdType(), healthySpinBox_->value(), woundedSpinBox_->value(), deadSpinBox_->value(), entity.Get< kernel::Positions >().GetPosition(), top );
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
    crowdSizeLabel_->setText( locale().toString( healthySpinBox_->value() + woundedSpinBox_->value() + deadSpinBox_->value() ) );
    remainingInhabitantLabel_->setText( locale().toString( originalInhabitantSize_ - ( healthySpinBox_->value() + woundedSpinBox_->value() + deadSpinBox_->value() ) ) );
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
        kernel::ContextMenu* subMenu = menu.SubMenu( "Order", tools::translate( "Magic orders", "Magic orders" ) );
        subMenu->insertItem( tools::translate( "InhabitantExtractCrowdDialog", "Extract crowd" ), this, SLOT( Show() ) );
    }
}
