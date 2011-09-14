// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DisplayBuilder.h"
#include "GroupDisplayer.h"

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DisplayBuilder constructor
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
DisplayBuilder::DisplayBuilder( QWidget* parent, ItemFactory_ABC& factory )
    : factory_( factory )
    , parent_( parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisplayBuilder destructor
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
DisplayBuilder::~DisplayBuilder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisplayBuilder::AddGroup
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
GroupDisplayer& DisplayBuilder::AddGroup( const QString& name )
{
    GroupDisplayer*& group = groups_[ name ];
    if( group )
        throw std::runtime_error( "Group '" + std::string( name.ascii() ) + "' already exists" );
    group = new GroupDisplayer( parent_, name, factory_ );
    return *group;
}

// -----------------------------------------------------------------------------
// Name: DisplayBuilder::SubItem
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
Displayer_ABC& DisplayBuilder::SubItem( const QString& name )
{
    GroupDisplayer* group = groups_[ name ];
    if( ! group )
        return AddGroup( name );
    return *group;
}

// -----------------------------------------------------------------------------
// Name: DisplayBuilder::Group
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
GroupDisplayer& DisplayBuilder::Group( const QString& name )
{
    return (GroupDisplayer&)( SubItem( name ) );
}

// -----------------------------------------------------------------------------
// Name: DisplayBuilder::StartDisplay
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void DisplayBuilder::StartDisplay()
{
    NotToBeCalled( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: DisplayBuilder::DisplayFormatted
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void DisplayBuilder::DisplayFormatted( const QString& )
{
    DisplayNotImplemented< QString >();
}

// -----------------------------------------------------------------------------
// Name: DisplayBuilder::EndDisplay
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void DisplayBuilder::EndDisplay()
{
    NotToBeCalled( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: DisplayBuilder::Hide
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void DisplayBuilder::Hide()
{
    for( IT_Groups it = groups_.begin(); it != groups_.end(); ++it )
        it->second->Hide();
}

// -----------------------------------------------------------------------------
// Name: DisplayBuilder::Clear
// Created: LDC 2009-03-18
// -----------------------------------------------------------------------------
void DisplayBuilder::Clear()
{
    for( IT_Groups it = groups_.begin(); it != groups_.end(); ++it )
        it->second->Clear();
}
