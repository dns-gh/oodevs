// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Usages.h"
#include "Tools.h"
#include "clients_kernel/PropertiesDictionary.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: Usages constructor
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
Usages::Usages( kernel::PropertiesDictionary& dictionary )
    : dictionary_( dictionary )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Usages destructor
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
Usages::~Usages()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Usages::Add
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
void Usages::Add( const std::string& usage, unsigned int proportion )
{
    usages_[ usage ] = proportion;
    CIT_Usages it = usages_.find( usage );
    dictionary_.Register( *this, tools::translate( "Block", "PhysicalFeatures/Motivations/" ) + QString( it->first.c_str() ), it->second );
}

// -----------------------------------------------------------------------------
// Name: Usages::Find
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
unsigned int Usages::Find( const std::string& usage ) const
{
    CIT_Usages it = usages_.find( usage );
    if( it != usages_.end() )
        return it->second;
    return 0u;
}
