// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "OrbatPanel.h"
#include "moc_OrbatPanel.cpp"
#include "clients_gui/ColorButton.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"

// -----------------------------------------------------------------------------
// Name: OrbatPanel constructor
// Created: LGY 2011-10-21
// -----------------------------------------------------------------------------
OrbatPanel::OrbatPanel( QWidget* parent, kernel::Controllers& controllers )
    : gui::PreferencePanel_ABC( parent, "OrbatPanel" )
    , controllers_( controllers )
    , pGhostColor_( 0 )
    , pLinkColor_ ( 0 )
{
    QWidget* main = new QWidget( this );
    QBoxLayout* layout = new QBoxLayout( QBoxLayout::TopToBottom, main );
    QGroupBox* box = new QGroupBox( tr( "Edition" ), this );
    layout->setAlignment( Qt::AlignTop );
    layout->addWidget( box );
    QVBoxLayout* mainLayout = new QVBoxLayout( box );
    // Ghost
    QHBoxLayout* ghostLayout = new QHBoxLayout();
    ghostLayout->addWidget( new QLabel( tr( "Ghost:" ) ) );
    pGhostColor_ = new gui::ColorButton( main, "", QColor( 60, 180, 90 ) );
    ghostLayout->addWidget( pGhostColor_ );
    mainLayout->addLayout( ghostLayout );

    // Missing logistic links
    QHBoxLayout* linkLayout = new QHBoxLayout();
    linkLayout->addWidget( new QLabel( tr( "Missing logistic links:" ) ) );
    pLinkColor_ = new gui::ColorButton( main, "", QColor( 255, 255, 0 ) );
    linkLayout->addWidget( pLinkColor_ );
    mainLayout->addLayout( linkLayout );

    if( controllers_.options_.GetOption( "Color/Phantom", QString( "" ) ).To< QString >() == "" )
        controllers_.options_.Change( "Color/Phantom", pGhostColor_->GetColor().name() );

    if( controllers_.options_.GetOption( "Color/MissingLogisticLinks", QString( "" ) ).To< QString >() == "" )
        controllers_.options_.Change( "Color/MissingLogisticLinks", pLinkColor_->GetColor().name() );

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
    pGhostColor_->Commit();
    pLinkColor_->Commit();
    controllers_.options_.Change( "Color/Phantom", pGhostColor_->GetColor().name() );
    controllers_.options_.Change( "Color/MissingLogisticLinks", pLinkColor_->GetColor().name() );
}

// -----------------------------------------------------------------------------
// Name: OrbatPanel::Reset
// Created: LGY 2011-10-21
// -----------------------------------------------------------------------------
void OrbatPanel::Reset()
{
    pGhostColor_->Revert();
    pLinkColor_->Revert();
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
    else if( option[ 1 ] == "Phantom" )
    {
        pGhostColor_->SetColor( QColor( value.To< QString >() ) );
        pGhostColor_->Commit();
    }
    else if( option[ 1 ] == "MissingLogisticLinks" )
    {
        pLinkColor_->SetColor( QColor( value.To< QString >() ) );
        pLinkColor_->Commit();
    }
}
