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
#include "clients_kernel/ObjectKnowledgeConverter_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;
using namespace actions;
using namespace parameters;

namespace
{
    unsigned long ReadId( xml::xistream& xis )
    {
        unsigned long id;
        xis >> xml::attribute( "value", id );
        return id;
    }

    std::string ReadName( xml::xistream& xis )
    {
        std::string name;
        xis >> xml::attribute( "name", name );
        return name;
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ObjectKnowledge::ObjectKnowledge( const OrderParameter& parameter )
    : Entity< ObjectKnowledge_ABC >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ObjectKnowledge::ObjectKnowledge( const OrderParameter& parameter, unsigned long id, kernel::ObjectKnowledgeConverter_ABC& converter, const Entity_ABC& owner )
    : Entity< ObjectKnowledge_ABC >( parameter )
{
    const Team_ABC& team = static_cast< const Team_ABC& >( owner.Get< CommunicationHierarchies >().GetTop() );
    SetValue( converter.Find( id, team ) );
//    if( ! GetValue() )
//        throw std::runtime_error( tools::translate( "Parameter", "Object knowledge not found." ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ObjectKnowledge::ObjectKnowledge( xml::xistream& xis, const Resolver_ABC< Object_ABC >& resolver, ObjectKnowledgeConverter_ABC& converter, const Entity_ABC& owner )
    : Entity< ObjectKnowledge_ABC >( OrderParameter( ReadName( xis ), "objectknowledge", false ) )
{
    const Team_ABC& team = static_cast< const Team_ABC& >( owner.Get< CommunicationHierarchies >().GetTop() );
    SetValue( converter.Find( resolver.Get( ReadId( xis ) ), team ) );
    if( ! GetValue() )
        throw std::runtime_error( tools::translate( "Parameter", "Object knowledge not found." ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ObjectKnowledge::ObjectKnowledge( const OrderParameter& parameter, xml::xistream& xis, const Resolver_ABC< Object_ABC >& resolver, ObjectKnowledgeConverter_ABC& converter, const Entity_ABC& owner )
    : Entity< ObjectKnowledge_ABC >( parameter )
{
    const Team_ABC& team = static_cast< const Team_ABC& >( owner.Get< CommunicationHierarchies >().GetTop() );
    SetValue( converter.Find( resolver.Get( ReadId( xis ) ), team ) );
    if( ! GetValue() )
        throw std::runtime_error( tools::translate( "Parameter", "Object knowledge not found." ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge destructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ObjectKnowledge::~ObjectKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ObjectKnowledge::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = !IsSet();
    asn.value.t = T_MissionParameter_value_objectKnowledge;
    if( IsSet() )
        Entity< ObjectKnowledge_ABC >::CommitTo( (ASN1T_OID&)asn.value.u.objectKnowledge );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::Accept
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ObjectKnowledge::Accept( ParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ObjectKnowledge::CommitTo( ASN1T_ObjectKnowledge& asn ) const
{
    Entity< ObjectKnowledge_ABC >::CommitTo( (ASN1T_OID&)asn );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::Serialize
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ObjectKnowledge::Serialize( xml::xostream& xos ) const
{
    if( ! GetValue() )
        throw std::runtime_error( tools::translate( "Parameter", "Invalid object knowledge." ).ascii() );
    Parameter< const ObjectKnowledge_ABC* >::Serialize( xos );
    xos << xml::attribute( "value", GetValue()->GetEntity()->GetId() ); // $$$$ SBO 2007-05-24: 
}
