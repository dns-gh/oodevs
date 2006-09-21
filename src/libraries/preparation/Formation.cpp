// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Formation.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Hierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

unsigned long Formation::idManager_ = 0;

// -----------------------------------------------------------------------------
// Name: Formation constructor
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
Formation::Formation( kernel::Controller& controller, const QString& level )
    : controller_( controller )
    , id_( idManager_++ )
    , name_( "" )
    , level_( level )
{
    RegisterSelf( *this );
}

// -----------------------------------------------------------------------------
// Name: Formation destructor
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
Formation::~Formation()
{
    controller_.Delete( *(Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Formation::DoUpdate
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
void Formation::DoUpdate( const kernel::InstanciationComplete& )
{
    controller_.Create( *(Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Formation::IsInTeam
// Created: SBO 2006-09-20
// -----------------------------------------------------------------------------
bool Formation::IsInTeam( const Team_ABC& team ) const
{
    const Hierarchies* hierarchy = Retrieve< Hierarchies >();
    return hierarchy || hierarchy->IsSubordinateOf( team );
}

// -----------------------------------------------------------------------------
// Name: Formation::GetName
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
QString Formation::GetName() const
{
    return level_;
}
    
// -----------------------------------------------------------------------------
// Name: Formation::GetId
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
unsigned long Formation::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Formation::GetLevel
// Created: SBO 2006-09-20
// -----------------------------------------------------------------------------
const QString& Formation::GetLevel() const
{
    return level_;
}

// -----------------------------------------------------------------------------
// Name: Formation::DoSerialize
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
void Formation::DoSerialize( xml::xostream& xos ) const
{
    xos << attribute( "id", long( id_ ) )
        << attribute( "name", std::string( name_.ascii() ) )
        << attribute( "level", std::string( level_.ascii() ) );
}
