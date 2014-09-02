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
#include "Tools.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"

using namespace gui;

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
        pNColor_  = tools::AddColorButton( mainLayout, controllers_.options_, "neutralized_color_button",
                                           tr( "Neutralized:" ), "Color/Neutralized", QColor( 235, 230, 101 ) );
        pADColor_ = tools::AddColorButton( mainLayout, controllers_.options_, "tactically_destroyed_color_button",
                                           tr( "Tactically destroyed:" ), "Color/TacticallyDestroyed", QColor( 235, 185, 101 ) );
        pODColor_ = tools::AddColorButton( mainLayout, controllers_.options_, "totally_destroyed_color_button",
                                           tr( "Totally destroyed:" ), "Color/TotallyDestroyed", QColor( 235, 106, 101 ) );
        container->addWidget( box );
        container->addStretch();
    }
    {
        QGroupBox* box = new QGroupBox( tr( "Crowds health state color" ), this );
        QVBoxLayout* mainLayout = new QVBoxLayout( box );
        pHealthyColor_       = tools::AddColorButton( mainLayout, controllers_.options_, "healthy_color_button",
                                                      tr( "Healthy:" ), "Color/Healthy", QColor::fromRgbF( COLOR_LIGHT_BLUE ) );
        pContaminatedColor_  = tools::AddColorButton( mainLayout, controllers_.options_, "contaminated_color_button",
                                                      tr( "Contaminated:" ), "Color/Contaminated", QColor( Qt::green ) );
        pWoundedColor_       = tools::AddColorButton( mainLayout, controllers_.options_, "wounded_color_button",
                                                      tr( "Wounded:" ), "Color/Wounded", QColor( Qt::red ) );
        pDeadColor_          = tools::AddColorButton( mainLayout, controllers_.options_, "dead_color_button",
                                                      tr( "Dead:" ), "Color/Dead", QColor( Qt::black ) );
        pMostlyHealthyColor_ = tools::AddColorButton( mainLayout, controllers_.options_, "mostly_healthy_color_button",
                                                      tr( "Mostly healthy:" ), "Color/MostlyHealthy", QColor( Qt::yellow ) );
        container->addWidget( box );
        container->addStretch( 1 );
    }
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
        pNColor_->SetColor( QColor( value.To< QString >() ) );
    else if( option[1] == "TacticallyDestroyed" )
        pADColor_->SetColor( QColor( value.To< QString >() ) );
    else if( option[1] == "TotallyDestroyed" )
        pODColor_->SetColor( QColor( value.To< QString >() ) );
    else if( option[ 1 ] == "Healthy" )
        pHealthyColor_->SetColor( QColor( value.To< QString >() ) );
    else if( option[ 1 ] == "Contaminated" )
        pContaminatedColor_->SetColor( QColor( value.To< QString >() ) );
    else if( option[ 1 ] == "Wounded" )
        pWoundedColor_->SetColor( QColor( value.To< QString >() ) );
    else if( option[ 1 ] == "Dead" )
        pDeadColor_->SetColor( QColor( value.To< QString >() ) );
    else if( option[ 1 ] == "MostlyHealthy" )
        pMostlyHealthyColor_->SetColor( QColor( value.To< QString >() ) );
}
