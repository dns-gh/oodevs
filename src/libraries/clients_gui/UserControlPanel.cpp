// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::UserControlPanel */

#include "clients_gui_pch.h"
#include "UserControlPanel.h"
#include "moc_UserControlPanel.cpp"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionVariant.h"
#include "ButtonGroup.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UserControlPanel constructor
// Created: RPD 2008-08-13
// -----------------------------------------------------------------------------
UserControlPanel::UserControlPanel( QWidget* pParent, kernel::Controllers& controllers )
    : PreferencePanel_ABC( pParent )
    , controllers_( controllers )
    , options_( controllers_.options_ )
{
    {
        mapDraggingType_ = new ButtonGroup( 2, Qt::Horizontal, tr( "Map dragging control" ), this );
        mapDraggingType_->insert( new QRadioButton( tr( "Use left click" ) , mapDraggingType_ ) );
        mapDraggingType_->insert( new QRadioButton( tr( "Use wheel click" ), mapDraggingType_ ) );
        mapDraggingType_->setButton( 0 );
    
        connect( mapDraggingType_, SIGNAL( clicked( int ) ), this, SLOT( OnMapDraggingType( int ) ) );
    }
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UserControlPanel destructor
// Created: RPD 2008-08-13
// -----------------------------------------------------------------------------
UserControlPanel::~UserControlPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UserControlPanel::Commit
// Created: RPD 2008-08-13
// -----------------------------------------------------------------------------
void UserControlPanel::Commit()
{
    mapDraggingType_->Commit();
}

// -----------------------------------------------------------------------------
// Name: UserControlPanel::Reset
// Created: RPD 2008-08-13
// -----------------------------------------------------------------------------
void UserControlPanel::Reset()
{
    mapDraggingType_->Revert();
}

// -----------------------------------------------------------------------------
// Name: UserControlPanel::OptionChanged
// Created: RPD 2008-08-13
// -----------------------------------------------------------------------------
void UserControlPanel::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "MapDraggingType" )
        mapDraggingType_-> setButton( value.To< int >());
}

// -----------------------------------------------------------------------------
// Name: UserControlPanel::OnMapDraggingType
// Created: RPD 2008-08-13
// -----------------------------------------------------------------------------
void UserControlPanel::OnMapDraggingType( int type )
{
    if( type == 0 )
    {
        // $$$$ SBO 2008-08-19: TODO
    }
    else if( type == 1 )
    {
        // $$$$ SBO 2008-08-19: TODO
    }
    options_.Change( "MapDraggingType", type );
}
