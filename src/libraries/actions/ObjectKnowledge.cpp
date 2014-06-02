// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "ObjectKnowledge.h"

#include "ParameterVisitor_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/ObjectKnowledgeConverter_ABC.h"
#include "protocol/Protocol.h"

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ObjectKnowledge::ObjectKnowledge( const OrderParameter& parameter, Controller& controller )
    : T_Entity( parameter, controller )
    , id_   ( 0 )
    , valid_( false )
{
    // NOTHING
}

namespace
{
    const ObjectKnowledge_ABC* GetKnowledge( const EntityResolver_ABC& entities,
                                             const ObjectKnowledgeConverter_ABC& converter,
                                             const Entity_ABC* owner,
                                             uint32_t id )
    {
        if( owner && entities.FindObject( id ) )
            return converter.Find( entities.GetObject( id ), *owner );
        return nullptr;
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ObjectKnowledge::ObjectKnowledge( const OrderParameter& parameter,
                                  uint32_t id,
                                  const ObjectKnowledgeConverter_ABC& converter,
                                  const Entity_ABC* owner,
                                  Controller& controller,
                                  const EntityResolver_ABC& entities )
    : T_Entity( parameter, GetKnowledge( entities, converter, owner, id ), controller )
    , id_( id )
{
    Attach();
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ObjectKnowledge::ObjectKnowledge( const OrderParameter& parameter,
                                  const ObjectKnowledge_ABC* knowledge,
                                  Controller& controller )
    : T_Entity( parameter, knowledge, controller )
    , id_( knowledge ? knowledge->GetEntityId() : 0 )
{
    Attach();
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge destructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ObjectKnowledge::~ObjectKnowledge()
{
    if( auto entity = const_cast< ObjectKnowledge_ABC* >( T_Entity::GetValue() ) )
        entity->RemoveListener( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::Accept
// Created: JSR 2010-05-20
// -----------------------------------------------------------------------------
void ObjectKnowledge::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ObjectKnowledge::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( !id_ );
    CommitTo( *message.add_value() );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ObjectKnowledge::CommitTo( sword::MissionParameter_Value& message ) const
{
    CommitTo( *message.mutable_objectknowledge() );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ObjectKnowledge::CommitTo( sword::Id& message ) const
{
    message.set_id( id_ );
}

// -----------------------------------------------------------------------------
// Name: Knowledge_ABC::GetDisplayName
// Created: JSR 2012-10-17
// -----------------------------------------------------------------------------
QString ObjectKnowledge::GetDisplayName( kernel::DisplayExtractor_ABC& extractor ) const
{
    if( valid_ )
        return T_Entity::GetDisplayName( extractor );
    return extractor.GetDisplayName( id_ );
}

// -----------------------------------------------------------------------------
// Name: Knowledge_ABC::Display
// Created: JSR 2010-05-20
// -----------------------------------------------------------------------------
void ObjectKnowledge::Display( kernel::Displayer_ABC& displayer ) const
{
    if( valid_ )
        T_Entity::Display( displayer );
    else
        displayer.Item( tools::translate( "Parameter", "Parameter" ) ).Display( GetName() )
                 .Item( tools::translate( "Parameter", "Value" ) ).Display( id_ );
}

// -----------------------------------------------------------------------------
// Name: Knowledge_ABC::Serialize
// Created: JSR 2010-05-20
// -----------------------------------------------------------------------------
void ObjectKnowledge::Serialize( xml::xostream& xos ) const
{
    Parameter< const ObjectKnowledge_ABC* >::Serialize( xos );
    xos << xml::attribute( "value", id_ );
}

// -----------------------------------------------------------------------------
// Name: Knowledge_ABC::NotifyDestruction
// Created: JSR 2010-05-20
// -----------------------------------------------------------------------------
void ObjectKnowledge::NotifyDestruction()
{
    valid_ = false;
}

bool ObjectKnowledge::IsSet() const
{
    return id_ != 0;
}

void ObjectKnowledge::SetValue( const T_Concrete& value )
{
    T_Entity::SetValue( value );
    id_ = value ? value->GetEntityId() : 0;
    Attach();
}

void ObjectKnowledge::Attach()
{
    auto entity = const_cast< ObjectKnowledge_ABC* >( T_Entity::GetValue() );
    valid_ = entity != nullptr;
    if( entity )
        entity->AddListener( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string ObjectKnowledge::SerializeType() const
{
    return "objectknowledge";
}
