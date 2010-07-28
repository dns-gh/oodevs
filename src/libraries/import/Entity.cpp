// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "Entity.h"

std::map< std::string, Entity* > Entity::map_;

// -----------------------------------------------------------------------------
// Name: Entity constructor
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
Entity::Entity( xml::xistream& xis, Mapping& mapping, unsigned int knowledgeGroup )
    : mapping_         ( mapping )
    , knowledgeGroupId_( knowledgeGroup )
{
    std::string id;
    xis >> xml::attribute( "id", id );
    map_[ id ] = this;
    id_ = mapping.AddId( id );
    xis >> xml::content( "name", name_ )
        >> xml::start( "meta-entity-ref" )
            >> xml::content( "id", type_ )
        >> xml::end
        >> xml::optional >> xml::start( "members" )
            >> xml::list( "content", *this, &Entity::ReadChild, knowledgeGroup );
}

// -----------------------------------------------------------------------------
// Name: Entity destructor
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
Entity::~Entity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Entity::ReadChild
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
void Entity::ReadChild( xml::xistream& xis, unsigned int knowledgeGroup )
{
    boost::shared_ptr< Entity > child( new Entity( xis, mapping_, knowledgeGroup ) );
    children_.push_back( child );
}

// -----------------------------------------------------------------------------
// Name: operator<<
// Created: MCO 2010-07-13
// -----------------------------------------------------------------------------
xml::xostream& operator<<( xml::xostream& xos, const Entity& entity )
{
    entity.Write( xos );
    return xos;
}

// -----------------------------------------------------------------------------
// Name: Entity::Write
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
void Entity::Write( xml::xostream& xos ) const
{
    if( IsUnit() )
        WriteUnit( xos );
    else if( IsAutomat() )
        WriteAutomat( xos, mapping_[ type_ ] );
    else if( const Entity* unit = HasUnitChild() )
        WrapAutomat( xos, *unit );
    else
        WriteFormation( xos );
}

// -----------------------------------------------------------------------------
// Name: Entity::WrapAutomat
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
void Entity::WrapAutomat( xml::xostream& xos, const Entity& unit ) const
{
    xos << xml::start( "formation" )
            << xml::attribute( "id", mapping_.AddId() )
            << xml::attribute( "level", "i" )
            << xml::attribute( "name", name_ );
    WriteAutomat( xos, mapping_.GetSuperiorId( unit.type_ ) );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Entity::WriteAutomat
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
void Entity::WriteAutomat( xml::xostream& xos, const std::string& type ) const
{
    const Entity* unit = 0;
    T_Entities::const_iterator it = children_.begin();
    for( ; it != children_.end(); ++it )
        if( (*it)->IsUnit() )
        {
            unit = it->get();
            break;
        }
    if( unit )
    {
        xos << xml::start( "automat" )
                << xml::attribute( "id", id_ )
                << xml::attribute( "engaged", false )
                << xml::attribute( "knowledge-group", knowledgeGroupId_ )
                << xml::attribute( "type", type )
                << xml::attribute( "name", name_ );
        unit->WriteUnit( xos );
        for( ++it; it != children_.end(); ++it )
            if( (*it)->IsUnit() )
                (*it)->WriteUnit( xos );
        xos << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: Entity::WriteFormation
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
void Entity::WriteFormation( xml::xostream& xos ) const
{
    xos << xml::start( "formation" )
            << xml::attribute( "id", id_ )
            << xml::attribute( "level", "i" )
            << xml::attribute( "name", name_ );
    for( T_Entities::const_iterator it = children_.begin(); it != children_.end(); ++it )
        (*it)->Write( xos );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Entity::WriteUnit
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
void Entity::WriteUnit( xml::xostream& xos ) const
{
    xos << xml::start( "unit" )
            << xml::attribute( "id", id_ )
            << xml::attribute( "command-post", mapping_.IsCommandPost( type_ ) )
            << xml::attribute( "position", position_ )
            << xml::attribute( "type", mapping_[ type_ ] )
            << xml::attribute( "name", name_ );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Entity::IsAutomat
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
bool Entity::IsAutomat() const
{
    return mapping_.IsAutomat( type_ );
}

// -----------------------------------------------------------------------------
// Name: Entity::HasUnitChild
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
const Entity* Entity::HasUnitChild() const
{
    for( T_Entities::const_iterator it = children_.begin(); it != children_.end(); ++it )
        if( (*it)->IsUnit() )
            return it->get();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: Entity::IsUnit
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
bool Entity::IsUnit() const
{
    return mapping_.IsUnit( type_ );
}

// -----------------------------------------------------------------------------
// Name: Entity::SetPosition
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
void Entity::SetPosition( const Position& position )
{
    position_ = position;
}

// -----------------------------------------------------------------------------
// Name: Entity::Find
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
Entity* Entity::Find( const std::string& id )
{
    std::map< std::string, Entity* >::iterator it = map_.find( id );
    if( it != map_.end() )
        return it->second;
    return 0;
}
