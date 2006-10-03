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
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "xeumeuleu/xml.h"
#include "IdManager.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: Formation constructor
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
Formation::Formation( kernel::Controller& controller, const HierarchyLevel_ABC& level, IdManager& idManager )
    : controller_( controller )
    , id_( idManager.GetNextId() )
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
    if( const Hierarchies* hierarchies = Retrieve< Hierarchies >() )
    {
        Iterator< const Entity_ABC& > it = hierarchies->CreateSubordinateIterator();
        while( it.HasMoreElements() )
        {
            const Entity_ABC& entity = it.NextElement();
            delete &entity;
        }
    }
    DestroyExtensions();
    controller_.Delete( *(Formation_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Formation::DoUpdate
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
void Formation::DoUpdate( const kernel::InstanciationComplete& )
{
    controller_.Create( *(Formation_ABC*)this );
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
    return level_.GetName();
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
const HierarchyLevel_ABC& Formation::GetLevel() const
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
        << attribute( "name", name_.ascii() )
        << attribute( "level", level_.GetName().ascii() );
}
