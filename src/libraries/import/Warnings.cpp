// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// LTO
//
// *****************************************************************************

#include "Warnings.h"

// -----------------------------------------------------------------------------
// Name: Warnings constructor
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
Warnings::Warnings( const std::string& log )
    : log_( log.c_str() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Warnings destructor
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
Warnings::~Warnings()
{
    try
    {
        for( std::set< std::string >::const_iterator it = identifiers_.begin(); it != identifiers_.end(); ++it )
            log_ << "Identifiant non reconnu : " << *it << std::endl;
    }
    catch( ... )
    {
        // NOTHING
    }
}

// -----------------------------------------------------------------------------
// Name: Warnings::Add
// Created: MCO 2010-07-16
// -----------------------------------------------------------------------------
void Warnings::Add( const std::string& id )
{
    identifiers_.insert( id );
}
