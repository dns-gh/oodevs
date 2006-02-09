// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_DisplayGroup.h"
#include "MOS_DisplayItem.h"
#include <stdexcept>

// -----------------------------------------------------------------------------
// Name: MOS_DisplayGroup constructor
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
MOS_DisplayGroup::MOS_DisplayGroup( QWidget* parent, const char* name  )
    : QGroupBox( 2, Qt::Horizontal, tr( name ), parent )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: MOS_DisplayGroup destructor
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
MOS_DisplayGroup::~MOS_DisplayGroup()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_DisplayGroup::AddItem
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
MOS_DisplayGroup& MOS_DisplayGroup::AddItem( const char* name, bool bold )
{
    MOS_DisplayItem*& item = items_[ std::string( name ) ];
    if( item )
        throw std::runtime_error( "Item '" + std::string( name ) + "' already added" );
    item = new MOS_DisplayItem( this, name, bold );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: MOS_DisplayGroup::GetItem
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
MOS_DisplayItem& MOS_DisplayGroup::GetItem( const char* name )
{
    MOS_DisplayItem* item = items_[ std::string( name ) ];
    if( ! item )
        throw std::runtime_error( "Item '" + std::string( name ) + "' does not exist" );
    return *item;
}

// -----------------------------------------------------------------------------
// Name: MOS_DisplayGroup::Clear
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
void MOS_DisplayGroup::Clear()
{
    for( IT_Items it = items_.begin(); it != items_.end(); ++it )
        it->second->Display( "" );
}
