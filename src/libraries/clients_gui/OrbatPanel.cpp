// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "OrbatPanel.h"
#include "moc_OrbatPanel.cpp"
#include "ColorButton.h"
#include "GLToolColors.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"

// -----------------------------------------------------------------------------
// Name: OrbatPanel constructor
// Created: LGY 2011-10-21
// -----------------------------------------------------------------------------
OrbatPanel::OrbatPanel( QWidget* parent, kernel::Controllers& controllers )
    : gui::PreferencePanel_ABC( parent, "OrbatPanel" )
    , controllers_( controllers )
    , pNColor_    ( 0 )
    , pADColor_   ( 0 )
    , pODColor_   ( 0 )
{
    QVBoxLayout* container = new QVBoxLayout( this );
    container->setSizeConstraint( QLayout::SetMinimumSize );
    {
        QGroupBox* box = new QGroupBox( tr( "Operational state color" ), this );
        QVBoxLayout* mainLayout = new QVBoxLayout( box );
        // Neutralized
        QHBoxLayout* neutralizedLayout = new QHBoxLayout();
        neutralizedLayout->addWidget( new QLabel( tr( "Neutralized:" ) ) );
        pNColor_ = new gui::ColorButton( "pNColor", box, "", QColor( 235, 230, 101 ) );
        neutralizedLayout->addWidget( pNColor_ );
        mainLayout->addLayout( neutralizedLayout );
        // Tactically destroyed
        QHBoxLayout* tacticallyDestroyedLayout = new QHBoxLayout();
        tacticallyDestroyedLayout->addWidget( new QLabel( tr( "Tactically destroyed:" ) ) );
        pADColor_ = new gui::ColorButton( "pADColor", box, "", QColor( 235, 185, 101 ) );
        tacticallyDestroyedLayout->addWidget( pADColor_ );
        mainLayout->addLayout( tacticallyDestroyedLayout );
        // Totally destroyed
        QHBoxLayout* totallyDestroyedLayout = new QHBoxLayout();
        totallyDestroyedLayout->addWidget( new QLabel( tr( "Totally destroyed:" ) ) );
        pODColor_ = new gui::ColorButton( "pODColor", box, "", QColor( 235, 106, 101 ) );
        totallyDestroyedLayout->addWidget( pODColor_ );
        mainLayout->addLayout( totallyDestroyedLayout );

        container->addWidget( box );
        container->addStretch();
    }
    {
        QGroupBox* box = new QGroupBox( tr( "Crowds health state color" ), this );
        QVBoxLayout* mainLayout = new QVBoxLayout( box );
        // Healthy
        QHBoxLayout* healthyLayout = new QHBoxLayout();
        healthyLayout->addWidget( new QLabel( tr( "Healthy:" ) ) );
        pHealthyColor_ = new gui::ColorButton( "pHealthyColor", box, "", QColor::fromRgbF( COLOR_LIGHT_BLUE ) );
        healthyLayout->addWidget( pHealthyColor_ );
        mainLayout->addLayout( healthyLayout );
        // Contaminated
        QHBoxLayout* contaminatedLayout = new QHBoxLayout();
        contaminatedLayout->addWidget( new QLabel( tr( "Contaminated:" ) ) );
        pContaminatedColor_ = new gui::ColorButton( "pContaminatedColor", box, "", QColor( Qt::green ) );
        contaminatedLayout->addWidget( pContaminatedColor_ );
        mainLayout->addLayout( contaminatedLayout );
        // Wounded
        QHBoxLayout* woundedLayout = new QHBoxLayout();
        woundedLayout->addWidget( new QLabel( tr( "Wounded:" ) ) );
        pWoundedColor_ = new gui::ColorButton( "pWoundedColor", box, "", QColor( Qt::red ) );
        woundedLayout->addWidget( pWoundedColor_ );
        mainLayout->addLayout( woundedLayout );
        // Dead
        QHBoxLayout* deadLayout = new QHBoxLayout();
        deadLayout->addWidget( new QLabel( tr( "Dead:" ) ) );
        pDeadColor_ = new gui::ColorButton( "pDeadColor", box, "", QColor( Qt::black ) );
        deadLayout->addWidget( pDeadColor_ );
        mainLayout->addLayout( deadLayout );
        // Mostly healthy
        QHBoxLayout* mostlyHealthyLayout = new QHBoxLayout();
        mostlyHealthyLayout->addWidget( new QLabel( tr( "Mostly healthy:" ) ) );
        pMostlyHealthyColor_ = new gui::ColorButton( "pMostlyHealthyColor", box, "", QColor( Qt::yellow ) );
        mostlyHealthyLayout->addWidget( pMostlyHealthyColor_ );
        mainLayout->addLayout( mostlyHealthyLayout );

        container->addWidget( box );
        container->addStretch( 1 );
    }


    if( controllers_.options_.GetOption( "Color/Neutralized", QString( "" ) ).To< QString >() == "" )
        controllers_.options_.Change( "Color/Neutralized", pNColor_->GetColor().name() );
    if( controllers_.options_.GetOption( "Color/TacticallyDestroyed", QString( "" ) ).To< QString >() == "" )
        controllers_.options_.Change( "Color/TacticallyDestroyed", pADColor_->GetColor().name() );
    if( controllers_.options_.GetOption( "Color/TotallyDestroyed", QString( "" ) ).To< QString >() == "" )
        controllers_.options_.Change( "Color/TotallyDestroyed", pODColor_->GetColor().name() );
    if( controllers_.options_.GetOption( "Color/Healthy", QString( "" ) ).To< QString >() == "" )
        controllers_.options_.Change( "Color/Healthy", pHealthyColor_->GetColor().name() );
    if( controllers_.options_.GetOption( "Color/Contaminated", QString( "" ) ).To< QString >() == "" )
        controllers_.options_.Change( "Color/Contaminated", pContaminatedColor_->GetColor().name() );
    if( controllers_.options_.GetOption( "Color/Wounded", QString( "" ) ).To< QString >() == "" )
        controllers_.options_.Change( "Color/Wounded", pWoundedColor_->GetColor().name() );
    if( controllers_.options_.GetOption( "Color/Dead", QString( "" ) ).To< QString >() == "" )
        controllers_.options_.Change( "Color/Dead", pDeadColor_->GetColor().name() );
    if( controllers_.options_.GetOption( "Color/MostlyHealthy", QString( "" ) ).To< QString >() == "" )
        controllers_.options_.Change( "Color/MostlyHealthy", pMostlyHealthyColor_->GetColor().name() );

    setLayout( container );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: OrbatPanel destructor
// Created: LGY 2011-10-21
// -----------------------------------------------------------------------------
OrbatPanel::~OrbatPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: OrbatPanel::Commit
// Created: LGY 2011-10-21
// -----------------------------------------------------------------------------
void OrbatPanel::Commit()
{
    pNColor_->Commit();
    pADColor_->Commit();
    pODColor_->Commit();
    pHealthyColor_->Commit();
    pContaminatedColor_->Commit();
    pWoundedColor_->Commit();
    pDeadColor_->Commit();
    pMostlyHealthyColor_->Commit();
    controllers_.options_.Change( "Color/Neutralized", pNColor_->GetColor().name() );
    controllers_.options_.Change( "Color/TacticallyDestroyed", pADColor_->GetColor().name() );
    controllers_.options_.Change( "Color/TotallyDestroyed", pODColor_->GetColor().name() );
    controllers_.options_.Change( "Color/Healthy", pHealthyColor_->GetColor().name() );
    controllers_.options_.Change( "Color/Contaminated", pContaminatedColor_->GetColor().name() );
    controllers_.options_.Change( "Color/Wounded", pWoundedColor_->GetColor().name() );
    controllers_.options_.Change( "Color/Dead", pDeadColor_->GetColor().name() );
    controllers_.options_.Change( "Color/MostlyHealthy", pMostlyHealthyColor_->GetColor().name() );
}

// -----------------------------------------------------------------------------
// Name: OrbatPanel::Reset
// Created: LGY 2011-10-21
// -----------------------------------------------------------------------------
void OrbatPanel::Reset()
{
    pNColor_->Revert();
    pADColor_->Revert();
    pODColor_->Revert();
    pHealthyColor_->Revert();
    pContaminatedColor_->Revert();
    pWoundedColor_->Revert();
    pDeadColor_->Revert();
    pMostlyHealthyColor_->Revert();
}

// -----------------------------------------------------------------------------
// Name: OrbatPanel::OptionChanged
// Created: LGY 2011-10-21
// -----------------------------------------------------------------------------
void OrbatPanel::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    const QStringList option = QStringList::split( "/", name.c_str() );
    if( !( option[ 0 ] == "Color" ) )
        return;
    if( option[ 1 ] == "Neutralized" )
    {
        pNColor_->SetColor( QColor( value.To< QString >() ) );
        pNColor_->Commit();
    }
    else if( option[1] == "TacticallyDestroyed" )
    {
        pADColor_->SetColor( QColor( value.To< QString >() ) );
        pADColor_->Commit();
    }
    else if( option[1] == "TotallyDestroyed" )
    {
        pODColor_->SetColor( QColor( value.To< QString >() ) );
        pODColor_->Commit();
    }
    else if( option[ 1 ] == "Healthy" )
    {
        pHealthyColor_->SetColor( QColor( value.To< QString >() ) );
        pHealthyColor_->Commit();
    }
    else if( option[ 1 ] == "Contaminated" )
    {
        pContaminatedColor_->SetColor( QColor( value.To< QString >() ) );
        pContaminatedColor_->Commit();
    }
    else if( option[ 1 ] == "Wounded" )
    {
        pWoundedColor_->SetColor( QColor( value.To< QString >() ) );
        pWoundedColor_->Commit();
    }
    else if( option[ 1 ] == "Dead" )
    {
        pDeadColor_->SetColor( QColor( value.To< QString >() ) );
        pDeadColor_->Commit();
    }
    else if( option[ 1 ] == "MostlyHealthy" )
    {
        pMostlyHealthyColor_->SetColor( QColor( value.To< QString >() ) );
        pMostlyHealthyColor_->Commit();
    }
}
