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
Entity::Entity()
{
    // NOTHING
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
// Name: Entity::Read
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
std::string Entity::Read( xml::xistream& xis, Mapping& mapping, unsigned int knowledgeGroup )
{
    std::string editorId;
    xis >> xml::attribute( "id", editorId );
    map_[ editorId ] = this;
    id_ = mapping.AddId( editorId );
    knowledgeGroupId_ = knowledgeGroup;
    xis >> xml::content( "ns2:name", name_ )
        >> xml::start( "ns2:meta-entity-ref" )
            >> xml::content( "ns2:id", type_ )
        >> xml::end
        >> xml::optional >> xml::start( "ns5:members" )
            >> xml::list( "ns5:content", *this, &Entity::ReadChild, mapping, knowledgeGroup );
    return editorId;
}

// -----------------------------------------------------------------------------
// Name: Entity::ReadChild
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
void Entity::ReadChild( xml::xistream& xis, Mapping& mapping, unsigned int knowledgeGroup )
{
    boost::shared_ptr< Entity > child( new Entity() );
    child->Read( xis, mapping, knowledgeGroup );
    children_.push_back( child );
}

// -----------------------------------------------------------------------------
// Name: Entity::Write
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
void Entity::Write( xml::xostream& xos, Mapping& mapping ) const
{
    if( IsUnit( mapping ) )
        WriteUnit( xos, mapping );
    else if( IsAutomat( mapping ) )
        WriteAutomat( xos, mapping );
    else if( const Entity* unit = HasUnitChild( mapping ) )
        WrapAutomat( xos, mapping, *unit );
    else
        WriteFormation( xos, mapping );
}

// -----------------------------------------------------------------------------
// Name: Entity::WrapAutomat
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
void Entity::WrapAutomat( xml::xostream& xos, Mapping& mapping, const Entity& unit ) const
{
    xos << xml::start( "formation" )
            << xml::attribute( "id", mapping.AddId() )
            << xml::attribute( "level", "i" )
            << xml::attribute( "name", name_ );
    WriteAutomat( xos, mapping, mapping.GetSuperiorId( unit.type_ ) );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Entity::WriteAutomat
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
void Entity::WriteAutomat( xml::xostream& xos, Mapping& mapping ) const
{
    WriteAutomat( xos, mapping, mapping[ type_ ] );
}

// -----------------------------------------------------------------------------
// Name: Entity::WriteAutomat
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
void Entity::WriteAutomat( xml::xostream& xos, Mapping& mapping, const std::string& type ) const
{
    const Entity* unit = 0;
    T_Entities::const_iterator it = children_.begin();
    for( ; it != children_.end(); ++it )
        if( (*it)->IsUnit( mapping ) )
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
        unit->WriteUnit( xos, mapping );
        for( ++it; it != children_.end(); ++it )
            if( (*it)->IsUnit( mapping ) )
                (*it)->WriteUnit( xos, mapping );
        xos << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: Entity::WriteFormation
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
void Entity::WriteFormation( xml::xostream& xos, Mapping& mapping ) const
{
    xos << xml::start( "formation" )
            << xml::attribute( "id", id_ )
            << xml::attribute( "level", "i" )
            << xml::attribute( "name", name_ );
    for( T_Entities::const_iterator it = children_.begin(); it != children_.end(); ++it )
        (*it)->Write( xos, mapping );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Entity::WriteUnit
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
void Entity::WriteUnit( xml::xostream& xos, const Mapping& mapping ) const
{
    xos << xml::start( "unit" )
            << xml::attribute( "id", id_ )
            << xml::attribute( "command-post", mapping.IsCommandPost( type_ ) );
    position_.WriteAttribute( "position", xos );
    xos     << xml::attribute( "type", mapping[ type_ ] )
            << xml::attribute( "name", name_ );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Entity::IsAutomat
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
bool Entity::IsAutomat( const Mapping& mapping ) const
{
    return mapping.IsAutomat( type_ );
}

// -----------------------------------------------------------------------------
// Name: Entity::HasUnitChild
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
const Entity* Entity::HasUnitChild( const Mapping& mapping ) const
{
    for( T_Entities::const_iterator it = children_.begin(); it != children_.end(); ++it )
        if( (*it)->IsUnit( mapping ) )
            return it->get();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: Entity::IsUnit
// Created: LDC 2010-07-08
// -----------------------------------------------------------------------------
bool Entity::IsUnit( const Mapping& mapping ) const
{
    return mapping.IsUnit( type_ );
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