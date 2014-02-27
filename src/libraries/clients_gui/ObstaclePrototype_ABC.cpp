// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ObstaclePrototype_ABC.h"
#include "moc_ObstaclePrototype_ABC.cpp"
#include "Tools.h"
#include "LoadableCheckBox.h"
#include "LoadableDateTimeEdit.h"
#include "LoadableTimeEdit.h"
#include "SubObjectName.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype_ABC constructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
ObstaclePrototype_ABC::ObstaclePrototype_ABC( QWidget* parent )
    : ObjectAttributePrototype_ABC( parent, "ObstaclePrototype_ABC", tools::translate( "gui::ObstaclePrototype_ABC", "Obstacle" ) )
{
    SubObjectName subObject( "ObstaclePrototypeABC" );

    QVBoxLayout* vLayout = new QVBoxLayout( this );
    QGroupBox* activationGroup = new QGroupBox( tools::translate( "gui::ObstaclePrototype_ABC", "Activation" ) );
    vLayout->addWidget( activationGroup );
    activationCombo_ = new QComboBox;
    activationCombo_->addItem( tools::translate( "gui::ObstaclePrototype_ABC", "Immediately" ) );
    activationCombo_->addItem( tools::translate( "gui::ObstaclePrototype_ABC", "After a delay" ) );
    activationCombo_->addItem( tools::translate( "gui::ObstaclePrototype_ABC", "At a specific date" ) );
    activationCombo_->addItem( tools::translate( "gui::ObstaclePrototype_ABC", "Inactive" ) );
    activationDelay_ = new LoadableTimeEdit( "activation_delay", 0 );
    activationDate_ = new LoadableDateTimeEdit( "activation_date", 0 );
    QHBoxLayout* activationLayout = new QHBoxLayout;
    activationLayout->addWidget( activationCombo_ );
    activationLayout->addWidget( activationDelay_ );
    activationLayout->addWidget( activationDate_ );
    activationDelay_->setEnabled( false );
    activationDate_->setVisible( false );
    activationGroup->setLayout( activationLayout );

    deactivationGroup_ = new RichGroupBox( "deactivation_group", tools::translate( "gui::ObstaclePrototype_ABC", "Deactivation" ) );
    vLayout->addWidget( deactivationGroup_ );
    deactivationCombo_ = new QComboBox;
    deactivationCombo_->addItem( tools::translate( "gui::ObstaclePrototype_ABC", "Never" ) );
    deactivationCombo_->addItem( tools::translate( "gui::ObstaclePrototype_ABC", "After a delay" ) );
    deactivationCombo_->addItem( tools::translate( "gui::ObstaclePrototype_ABC", "At a specific date" ) );
    deactivationDelay_ = new LoadableTimeEdit( "deactivation_delay", 0 );
    deactivationDate_ = new LoadableDateTimeEdit( "deactivation_date", 0 );
    QHBoxLayout* deactivationLayout = new QHBoxLayout;
    deactivationLayout->addWidget( deactivationCombo_ );
    deactivationLayout->addWidget( deactivationDelay_ );
    deactivationLayout->addWidget( deactivationDate_ );
    deactivationDelay_->setEnabled( false );
    deactivationDate_->setVisible( false );
    deactivationGroup_->setLayout( deactivationLayout );

    connect( activationCombo_, SIGNAL( activated( int ) ), this, SLOT( OnActivationTypeChanged( int ) ) );
    connect( deactivationCombo_, SIGNAL( activated( int ) ), this, SLOT( OnDeactivationTypeChanged( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype_ABC destructor
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
ObstaclePrototype_ABC::~ObstaclePrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype_ABC::CheckValidity
// Created: SBO 2006-04-20
// -----------------------------------------------------------------------------
bool ObstaclePrototype_ABC::CheckValidity( const kernel::Team_ABC& ) const
{
    if( activationCombo_->currentIndex() != eActivationInactive && deactivationCombo_->currentIndex() == eDeactivationDate )
    {
        QDateTime activationDate;
        if( activationCombo_->currentIndex() == eActivationDate )
            activationDate = activationDate_->dateTime();
        else
            activationDate = GetCreationDate().addSecs( GetActivationTime() );
        if( activationDate >= deactivationDate_->dateTime() )
        {
            deactivationGroup_->Warn();
            return false;
        }
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype_ABC::OnActivationTypeChanged
// Created: JSR 2013-09-12
// -----------------------------------------------------------------------------
void ObstaclePrototype_ABC::OnActivationTypeChanged( int index )
{
    switch( index )
    {
    case eActivationImmediately:
        deactivationGroup_->setEnabled( true );
        activationDelay_->setEnabled( false );
        activationDate_->setEnabled( false );
        break;
    case eActivationDelay:
        deactivationGroup_->setEnabled( true );
        activationDelay_->setEnabled( true );
        activationDelay_->setVisible( true );
        activationDate_->setVisible( false );
        break;
    case eActivationDate:
        deactivationGroup_->setEnabled( true );
        activationDate_->setEnabled( true );
        activationDelay_->setVisible( false );
        activationDate_->setVisible( true );
        break;
    case eActivationInactive:
        deactivationGroup_->setEnabled( false );
        activationDelay_->setEnabled( false );
        activationDate_->setEnabled( false );
        break;
    default:
        break;
    }
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype_ABC::OnDeactivationTypeChanged
// Created: JSR 2013-09-16
// -----------------------------------------------------------------------------
void ObstaclePrototype_ABC::OnDeactivationTypeChanged( int index )
{
    switch( index )
    {
    case eDeactivationNever:
        deactivationDelay_->setEnabled( false );
        deactivationDate_->setEnabled( false );
        break;
    case eDeactivationDelay:
        deactivationDelay_->setEnabled( true );
        deactivationDelay_->setVisible( true );
        deactivationDate_->setVisible( false );
        break;
    case eDeactivationDate:
        deactivationDate_->setEnabled( true );
        deactivationDelay_->setVisible( false );
        deactivationDate_->setVisible( true );
        break;
    default:
        break;
    }
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype_ABC::GetActivationTime
// Created: JSR 2010-07-07
// -----------------------------------------------------------------------------
int ObstaclePrototype_ABC::GetActivationTime() const
{
    switch( activationCombo_->currentIndex() )
    {
    case eActivationDelay:
        return activationDelay_->Seconds();
    case eActivationDate:
        return std::max( 0, GetCreationDate().secsTo( activationDate_->dateTime() ) );
    case eActivationImmediately:
    case eActivationInactive:
    default:
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype_ABC::IsActivated
// Created: JSR 2013-09-17
// -----------------------------------------------------------------------------
bool ObstaclePrototype_ABC::IsActivated() const
{
    int activation = activationCombo_->currentIndex();
    return ( activation == eActivationImmediately )
        || ( ( activation == eActivationDelay || activation == eActivationDate ) && GetActivationTime() == 0 );
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype_ABC::GetActivityTime
// Created: LGY 2011-08-31
// -----------------------------------------------------------------------------
int ObstaclePrototype_ABC::GetActivityTime() const
{
    if( activationCombo_->currentIndex() == eActivationInactive )
        return 0;
    switch( deactivationCombo_->currentIndex() )
    {
    case eDeactivationDelay:
        return deactivationDelay_->Seconds();
    case eDeactivationDate:
        return std::max( 0, GetCreationDate().addSecs( GetActivationTime() ).secsTo( deactivationDate_->dateTime() ) );
    case eDeactivationNever:
    default:
       return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype_ABC::SetLoader
// Created: BCI 2011-05-12
// -----------------------------------------------------------------------------
void ObstaclePrototype_ABC::SetLoader( ObjectPrototypeLoader_ABC* loader )
{
    activationDelay_->SetLoader( loader );
    activationDate_->SetLoader( loader );
    deactivationDelay_->SetLoader( loader );
    deactivationDate_->SetLoader( loader );
}

// -----------------------------------------------------------------------------
// Name: ObstaclePrototype_ABC::setVisible
// Created: JSR 2013-10-14
// -----------------------------------------------------------------------------
void ObstaclePrototype_ABC::setVisible( bool visible )
{
    ObjectAttributePrototype_ABC::setVisible( visible );
    activationDate_->GetDefaultValueWidget()->setDateTime( GetCreationDate() );
    deactivationDate_->GetDefaultValueWidget()->setDateTime( GetCreationDate() );
}
