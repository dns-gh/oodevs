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
#include "KnowledgeGroupFactory_ABC.h"
#include "KnowledgeGroup.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_gui/Tools.h"
#include "xeumeuleu/xml.h"
#include "IdManager.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: Team constructor
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
Team::Team( Controller& controller, KnowledgeGroupFactory_ABC& factory, IdManager& idManager )
    : controller_( controller )
    , factory_( factory )
    , id_( idManager.GetNextId() )
{
    RegisterSelf( *this );
    name_ = tools::translate( "Preparation", "Armée %1" ).arg( id_ );
}

// -----------------------------------------------------------------------------
// Name: Team constructor
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
Team::Team( xml::xistream& xis, kernel::Controller& controller, KnowledgeGroupFactory_ABC& factory, IdManager& idManager )
    : controller_( controller )
    , factory_( factory )
{
    std::string name;
    xis >> attribute( "id", (int&)id_ )
        >> attribute( "name", name );
    name_ = name.c_str();
    idManager.Lock( id_ );
    RegisterSelf( *this );
}

// -----------------------------------------------------------------------------
// Name: Team destructor
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
Team::~Team()
{
    DestroyExtensions();
    controller_.Delete( *(Team_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Team::DoUpdate
// Created: SBO 2006-09-20
// -----------------------------------------------------------------------------
void Team::DoUpdate( const kernel::InstanciationComplete& )
{
    controller_.Create( *(Team_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Team::GetName
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
QString Team::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Team::GetId
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
unsigned long Team::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Team::CreateKnowledgeGroup
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void Team::CreateKnowledgeGroup()
{
    factory_.CreateKnowledgeGroup( *this );
}

// -----------------------------------------------------------------------------
// Name: Team::CreateKnowledgeGroup
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
void Team::CreateKnowledgeGroup( xml::xistream& xis )
{
    factory_.CreateKnowledgeGroup( xis, *this );
}

// -----------------------------------------------------------------------------
// Name: Team::Rename
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void Team::Rename( const QString& name )
{
    name_ = name;
    controller_.Update( *(Team_ABC*)this );
    controller_.Update( *(Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Team::DoSerialize
// Created: SBO 2006-09-06
// -----------------------------------------------------------------------------
void Team::DoSerialize( xml::xostream& xos ) const
{
    xos << attribute( "id", long( id_ ) )
        << attribute( "name", name_.ascii() );
}
