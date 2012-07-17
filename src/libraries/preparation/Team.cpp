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
#include "IdManager.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: Team constructor
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
Team::Team( kernel::Controllers& controllers, IdManager& idManager )
    : kernel::Team( controllers, idManager.GetNextId(), "" )
{
    name_ = tools::translate( "Preparation", "Army %L1" ).arg( id_ );
    RegisterSelf( *this );
    CreateDictionary( controllers.controller_ );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Team constructor
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
Team::Team( xml::xistream& xis, kernel::Controllers& controllers, IdManager& idManager )
    : kernel::Team( controllers, xis.attribute< unsigned long >( "id" ), xis.attribute< std::string >( "name" ).c_str() )
{
    RegisterSelf( *this );
    idManager.Lock( id_ );
    CreateDictionary( controllers.controller_ );
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

// -----------------------------------------------------------------------------
// Name: Team::Rename
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void Team::Rename( const QString& name )
{
    name_ = name;
    Touch();
}

// -----------------------------------------------------------------------------
// Name: Team::SerializeAttributes
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void Team::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", id_ )
        << xml::attribute( "name", name_.toUtf8().constData() );
}

