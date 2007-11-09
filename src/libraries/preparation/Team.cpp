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
#include "ObjectFactory_ABC.h"
#include "KnowledgeGroup.h"
#include "Object.h"
#include "TeamKarmas.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/IntelligenceHierarchies.h"
#include "Tools.h"
#include "xeumeuleu/xml.h"
#include "IdManager.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: Team constructor
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
Team::Team( Controller& controller, KnowledgeGroupFactory_ABC& kgFactory, ObjectFactory_ABC& objectFactory, IdManager& idManager, TeamKarmas& karmas )
    : EntityImplementation< Team_ABC >( controller, idManager.GetNextId(), "" )
    , kgFactory_( kgFactory )
    , objectFactory_( objectFactory )
    , karma_( &karmas.GetDefault() )
{
    name_ = tools::translate( "Preparation", "Army %1" ).arg( id_ );
    RegisterSelf( *this );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Team constructor
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
Team::Team( xml::xistream& xis, kernel::Controller& controller, KnowledgeGroupFactory_ABC& kgFactory, ObjectFactory_ABC& objectFactory, IdManager& idManager, TeamKarmas& karmas )
    : EntityImplementation< Team_ABC >( controller, ReadId( xis ), ReadName( xis ) )
    , kgFactory_( kgFactory )
    , objectFactory_( objectFactory )
{
    std::string karma;
    xis >> attribute( "type", karma );
    karma_ = &karmas.Get( karma.c_str() );
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
// Name: Team::ReadId
// Created: AGE 2006-10-12
// -----------------------------------------------------------------------------
unsigned long Team::ReadId( xml::xistream& xis )
{
    int id;
    xis >> attribute( "id", id );
    return id;
}

// -----------------------------------------------------------------------------
// Name: Team::ReadName
// Created: AGE 2006-10-12
// -----------------------------------------------------------------------------
QString Team::ReadName( xml::xistream& xis )
{
    std::string name;
    xis >> attribute( "name", name );
    return name.c_str();
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
Object_ABC* Team::CreateObject( const kernel::ObjectType& type, const QString& name, const Enum_ObstacleType& obstacleType, bool reservedObstacleActivated, const kernel::Location_ABC& location )
{
    Object_ABC* object = objectFactory_.CreateObject( type, *this, name, obstacleType, reservedObstacleActivated, location );
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
    xos << attribute( "id", long( id_ ) )
        << attribute( "name", name_.ascii() )
        << attribute( "type", karma_->GetId() );

    xos << start( "objects" );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << start( "object" );
        it->second->Interface().Apply( & Serializable_ABC::SerializeAttributes, xos );
        xos << end();
    }
    xos << end();
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
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Team", "Info/Karma" ), karma_, *this, &Team::SetKarma );
}

// -----------------------------------------------------------------------------
// Name: Team::GetKarma
// Created: AGE 2006-11-24
// -----------------------------------------------------------------------------
const kernel::Karma& Team::GetKarma() const
{
    return *karma_;
}

// -----------------------------------------------------------------------------
// Name: Team::SetKarma
// Created: AGE 2006-11-24
// -----------------------------------------------------------------------------
void Team::SetKarma( const TeamKarma& karma )
{
    karma_ = karma;
    Touch();
    Get< kernel::TacticalHierarchies >().UpdateSymbol( false );
    Get< kernel::IntelligenceHierarchies >().UpdateSymbol( false );
}

// -----------------------------------------------------------------------------
// Name: Team::IsFriend
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
bool Team::IsFriend() const
{
    return *karma_ == Karma::friend_;
}

// -----------------------------------------------------------------------------
// Name: Team::IsEnemy
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
bool Team::IsEnemy() const
{
    return *karma_ == Karma::enemy_;
}

// -----------------------------------------------------------------------------
// Name: Team::IsNeutral
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
bool Team::IsNeutral() const
{
    return *karma_ == Karma::neutral_;
}
