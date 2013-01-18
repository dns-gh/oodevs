// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Team.h"
#include "tools/IdManager.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: Team constructor
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
Team::Team( kernel::Controllers& controllers, tools::IdManager& idManager )
    : kernel::Team( controllers, idManager.GetNextId(), "" )
{
    name_ = tools::translate( "Preparation", "Army %L1" ).arg( id_ );
    AddExtension( *this );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Team constructor
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
Team::Team( xml::xistream& xis, kernel::Controllers& controllers, tools::IdManager& idManager )
    : kernel::Team( controllers, xis.attribute< unsigned long >( "id" ), xis.attribute< std::string >( "name" ).c_str() )
{
    AddExtension( *this );
    idManager.Lock( id_ );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Team destructor
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
Team::~Team()
{
    Destroy();
    controllers_.Unregister( *this );
}
