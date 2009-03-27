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
#include "KnowledgeGroup.h"
#include "KnowledgeGroupFactory_ABC.h"
#include "IdManager.h"
#include "Object.h"
#include "ObjectFactory_ABC.h"
#include "Tools.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/PropertiesDictionary.h"
#include <xeumeuleu/xml.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Team constructor
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
Team::Team( Controller& controller, KnowledgeGroupFactory_ABC& kgFactory, ObjectFactory_ABC& objectFactory, IdManager& idManager )
    : EntityImplementation< Team_ABC >( controller, idManager.GetNextId(), "" )
    , kgFactory_( kgFactory )
    , objectFactory_( objectFactory )
{
    name_ = tools::translate( "Preparation", "Army %1" ).arg( id_ );
    RegisterSelf( *this );
    CreateDictionary( controller );
}

namespace
{
    QString ReadName( xml::xistream& xis )
    {
        std::string name;
        xis >> xml::attribute( "name", name );
        return name.c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: Team constructor
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
Team::Team( xml::xistream& xis, kernel::Controller& controller, KnowledgeGroupFactory_ABC& kgFactory, ObjectFactory_ABC& objectFactory, IdManager& idManager )
    : EntityImplementation< Team_ABC >( controller, xml::attribute< unsigned long >( xis, "id" ), ReadName( xis ) )
    , kgFactory_( kgFactory )
    , objectFactory_( objectFactory )
{
    RegisterSelf( *this );
    idManager.Lock( id_ );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Team destructor
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
Team::~Team()
{
    Resolver< Object_ABC >::DeleteAll();
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Team::CreateKnowledgeGroup
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void Team::CreateKnowledgeGroup()
{
    kgFactory_.CreateKnowledgeGroup( *this );
}

// -----------------------------------------------------------------------------
// Name: Team::CreateKnowledgeGroup
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
void Team::CreateKnowledgeGroup( xml::xistream& xis )
{
    kgFactory_.CreateKnowledgeGroup( xis, *this );
}

// -----------------------------------------------------------------------------
// Name: Team::CreateObject
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
Object_ABC* Team::CreateObject( const kernel::ObjectType& type, const QString& name, const kernel::Location_ABC& location )
{
    Object_ABC* object = objectFactory_.CreateObject( type, *this, name, location );
    Resolver< Object_ABC >::Register( object->GetId(), *object );
    return object;
}

// -----------------------------------------------------------------------------
// Name: Team::CreateObject
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
void Team::CreateObject( xml::xistream& xis )
{
    Object_ABC* object = objectFactory_.CreateObject( xis, *this );
    Resolver< Object_ABC >::Register( object->GetId(), *object );
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
    xos << xml::attribute( "id", long( id_ ) )
        << xml::attribute( "name", name_.ascii() );

    xos << xml::start( "objects" );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << xml::start( "object" );
        it->second->Interface().Apply( & Serializable_ABC::SerializeAttributes, xos );
        xos << xml::end();
    }
    xos << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Team::CreateDictionary
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
void Team::CreateDictionary( kernel::Controller& controller )
{
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    Attach( dictionary );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Team", "Info/Identifier" ), (const unsigned long)id_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Team", "Info/Name" ), name_ );
}
