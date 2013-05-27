// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "PropertiesDictionary.h"
#include "clients_kernel/Displayer_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: PropertiesDictionary constructor
// Created: SBO 2006-10-17
// -----------------------------------------------------------------------------
PropertiesDictionary::PropertiesDictionary( Controller& controller )
    : controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertiesDictionary destructor
// Created: SBO 2006-10-17
// -----------------------------------------------------------------------------
PropertiesDictionary::~PropertiesDictionary()
{
    for( auto it = properties_.begin(); it != properties_.end(); ++it )
        delete it->second;
}

// -----------------------------------------------------------------------------
// Name: PropertiesDictionary::Remove
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
void PropertiesDictionary::Remove( const QString& name )
{
    for( auto it = properties_.begin(); it != properties_.end(); )
    {
        auto curIt = it++;
        if( curIt->first.startsWith( name, Qt::CaseInsensitive ) )
        {
            controller_.Delete( DictionaryUpdated( curIt->second->GetOwner(), curIt->first ) );
            properties_.erase( curIt );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PropertiesDictionary::HasKey
// Created: ABR 2011-01-27
// -----------------------------------------------------------------------------
bool PropertiesDictionary::HasKey( const QString& name ) const
{
    return properties_.find( name ) != properties_.end();
}

// -----------------------------------------------------------------------------
// Name: PropertiesDictionary::Display
// Created: SBO 2006-10-17
// -----------------------------------------------------------------------------
void PropertiesDictionary::Display( Displayer_ABC& displayer )
{
    for( auto it = properties_.begin(); it != properties_.end(); ++it )
        displayer.Display( it->second );
}

// -----------------------------------------------------------------------------
// Name: PropertiesDictionary::Display
// Created: LGY 2012-09-05
// -----------------------------------------------------------------------------
void PropertiesDictionary::Display( const QString& name, Displayer_ABC& displayer )
{
    auto it = properties_.find( name );
    if( it != properties_.end() )
        displayer.Display( it->second );
}

// -----------------------------------------------------------------------------
// Name: PropertiesDictionary::GetProperties
// Created: LGY 2013-01-17
// -----------------------------------------------------------------------------
const PropertiesDictionary::T_Properties& PropertiesDictionary::GetProperties() const
{
    return properties_;
}
