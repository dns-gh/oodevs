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
// Name: PropertiesDictionary::FindKey
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
QStringList PropertiesDictionary::FindKey( QString name ) const
{
    QStringList result;
    for( CIT_Properties it = properties_.begin(); it != properties_.end(); ++it )
    {
        if( it->first.find( name, 0, false ) != -1 )
            result.append( it->first );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: PropertiesDictionary::Display
// Created: SBO 2006-10-17
// -----------------------------------------------------------------------------
void PropertiesDictionary::Display( Displayer_ABC& displayer )
{
    for( CIT_Properties it = properties_.begin(); it != properties_.end(); ++it )
        displayer.Display( it->first, it->second );
}

// -----------------------------------------------------------------------------
// Name: PropertiesDictionary::Display
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
void PropertiesDictionary::Display( const QString& name, Displayer_ABC& displayer )
{
    CIT_Properties it = properties_.find( name );
    if( it != properties_.end() )
        it->second->Display( displayer );
}
