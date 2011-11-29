// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::OrbatPanel */

#include "clients_gui_pch.h"
#include "OrbatPanel.h"
#include "moc_OrbatPanel.cpp"
#include "ColorButton.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: OrbatPanel constructor
// Created: LGY 2011-10-21
// -----------------------------------------------------------------------------
OrbatPanel::OrbatPanel( QWidget* parent, kernel::Controllers& controllers )
    : PreferencePanel_ABC( parent, "OrbatPanel" )
    , controllers_( controllers )
    , pNColor_    ( 0 )
    , pADColor_   ( 0 )
    , pODColor_   ( 0 )
    , pGhostColor_( 0 )
{
    QWidget* main = new QWidget( this );
    QBoxLayout* layout = new QBoxLayout( QBoxLayout::TopToBottom, main );
    QGroupBox* box = new QGroupBox( tr( "Operational state color" ), this );
    layout->setAlignment( Qt::AlignTop );
    layout->addWidget( box );
    QVBoxLayout* mainLayout = new QVBoxLayout( box );
    // Neutralized
    QHBoxLayout* neutralizedLayout = new QHBoxLayout();
    neutralizedLayout->addWidget( new QLabel( tr( "Neutralized:" ) ) );
    pNColor_ = new ColorButton( main, "", QColor( 235, 230, 101 ) );
    neutralizedLayout->addWidget( pNColor_ );
    mainLayout->addLayout( neutralizedLayout );
    // Tactically destroyed
    QHBoxLayout* tacticallyDestroyedLayout = new QHBoxLayout();
    tacticallyDestroyedLayout->addWidget( new QLabel( tr( "Tactically destroyed:" ) ) );
    pADColor_ = new ColorButton( main, "", QColor( 235, 185, 101 ) );
    tacticallyDestroyedLayout->addWidget( pADColor_ );
    mainLayout->addLayout( tacticallyDestroyedLayout );
    // Totally destroyed
    QHBoxLayout* totallyDestroyedLayout = new QHBoxLayout();
    totallyDestroyedLayout->addWidget( new QLabel( tr( "Totally destroyed:" ) ) );
    pODColor_ = new ColorButton( main, "", QColor( 235, 106, 101 ) );
    totallyDestroyedLayout->addWidget( pODColor_ );
    mainLayout->addLayout( totallyDestroyedLayout );
    // Phantom
    QHBoxLayout* ghostLayout = new QHBoxLayout();
    ghostLayout->addWidget( new QLabel( tr( "Phantom:" ) ) );
    pGhostColor_ = new ColorButton( main, "", QColor( 60, 180, 90 ) );
    ghostLayout->addWidget( pGhostColor_ );
    mainLayout->addLayout( ghostLayout );

    if( controllers_.options_.GetOption( "Color/Neutralized", QString( "" ) ).To< QString >() == "" )
        controllers_.options_.Change( "Color/Neutralized", pNColor_->GetColor().name() );
    if( controllers_.options_.GetOption( "Color/TacticallyDestroyed", QString( "" ) ).To< QString >() == "" )
        controllers_.options_.Change( "Color/TacticallyDestroyed", pADColor_->GetColor().name() );
    if( controllers_.options_.GetOption( "Color/TotallyDestroyed", QString( "" ) ).To< QString >() == "" )
        controllers_.options_.Change( "Color/TotallyDestroyed", pODColor_->GetColor().name() );
    if( controllers_.options_.GetOption( "Color/Phantom", QString( "" ) ).To< QString >() == "" )
        controllers_.options_.Change( "Color/Phantom", pGhostColor_->GetColor().name() );

    setWidget( main );
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
    pGhostColor_->Commit();
    controllers_.options_.Change( "Color/Neutralized", pNColor_->GetColor().name() );
    controllers_.options_.Change( "Color/TacticallyDestroyed", pADColor_->GetColor().name() );
    controllers_.options_.Change( "Color/TotallyDestroyed", pODColor_->GetColor().name() );
    controllers_.options_.Change( "Color/Phantom", pGhostColor_->GetColor().name() );
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
    pGhostColor_->Revert();
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
    else if( option[1] == "Phantom" )
    {
        pGhostColor_->SetColor( QColor( value.To< QString >() ) );
        pGhostColor_->Commit();
    }
}
