// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "PropertiesDictionary.h"
#include "Displayer_ABC.h"

using namespace kernel;

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
    for( IT_Properties it = properties_.begin(); it != properties_.end(); ++it )
        delete it->second;
}

// -----------------------------------------------------------------------------
// Name: PropertiesDictionary::Remove
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
void PropertiesDictionary::Remove( const QString& name )
{
    IT_Properties it = properties_.find( name );
    if( it != properties_.end() )
        properties_.erase( it );
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
    for( CIT_Properties it = properties_.begin(); it != properties_.end(); ++it )
	{
		if ( it->second->IsVisible() )
			displayer.Display( it->first, it->second );
	}
}

// -----------------------------------------------------------------------------
// Name: PropertiesDictionary::Display
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
void PropertiesDictionary::Display( const QString& name, Displayer_ABC& displayer )
{
    CIT_Properties it = properties_.find( name );
    if( it != properties_.end() && it->second->IsVisible() )
        it->second->Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: PropertiesDictionary::DisplaySubPath
// Created: SBO 2006-11-13
// -----------------------------------------------------------------------------
void PropertiesDictionary::DisplaySubPath( const QString& path, Displayer_ABC& displayer )
{
    QString search = path.endsWith( "/" ) ? path : path + "/";
    for( CIT_Properties it = properties_.begin(); it != properties_.end(); ++it )
    {
        if( it->first.find( search, 0, false ) != -1 && it->second->IsVisible() )
            displayer.Display( it->first, it->second );
    }
}

// -----------------------------------------------------------------------------
// Name: PropertiesDictionary::SetPropertyVisibility
// Created: RPD 2011-12-26
// -----------------------------------------------------------------------------
void PropertiesDictionary::SetPropertyVisibility( const QString& name, bool visible )
{
    CIT_Properties it = properties_.find( name );
    if( it != properties_.end() )
        it->second->SetVisibility( visible );
}