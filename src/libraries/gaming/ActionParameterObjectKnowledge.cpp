// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterObjectKnowledge.h"
#include "ObjectKnowledgeConverter_ABC.h"
#include "ActionParameterVisitor_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

namespace
{
    unsigned long ReadId( xml::xistream& xis )
    {
        unsigned long id;
        xis >> xml::attribute( "value", id );
        return id;
    }

    QString ReadName( xml::xistream& xis )
    {
        std::string name;
        xis >> xml::attribute( "name", name );
        return name.c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjectKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ActionParameterObjectKnowledge::ActionParameterObjectKnowledge( const OrderParameter& parameter )
    : ActionParameterEntity< ObjectKnowledge_ABC >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjectKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ActionParameterObjectKnowledge::ActionParameterObjectKnowledge( const OrderParameter& parameter, unsigned long id, ObjectKnowledgeConverter_ABC& converter, const Entity_ABC& owner )
    : ActionParameterEntity< ObjectKnowledge_ABC >( parameter )
{
    const Team_ABC& team = static_cast< const Team_ABC& >( owner.Get< CommunicationHierarchies >().GetTop() );
    SetValue( converter.Find( id, team ) );
    if( ! GetValue() )
        throw std::runtime_error( tools::translate( "ActionParameter", "Object knowledge not found." ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjectKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ActionParameterObjectKnowledge::ActionParameterObjectKnowledge( xml::xistream& xis, const Resolver_ABC< Object_ABC >& resolver, ObjectKnowledgeConverter_ABC& converter, const Entity_ABC& owner )
    : ActionParameterEntity< ObjectKnowledge_ABC >( OrderParameter( ReadName( xis ), "objectknowledge", false ) )
{
    const Team_ABC& team = static_cast< const Team_ABC& >( owner.Get< CommunicationHierarchies >().GetTop() );
    SetValue( converter.Find( resolver.Get( ReadId( xis ) ), team ) );
    if( ! GetValue() )
        throw std::runtime_error( tools::translate( "ActionParameter", "Object knowledge not found." ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjectKnowledge constructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ActionParameterObjectKnowledge::ActionParameterObjectKnowledge( const OrderParameter& parameter, xml::xistream& xis, const Resolver_ABC< Object_ABC >& resolver, ObjectKnowledgeConverter_ABC& converter, const Entity_ABC& owner )
    : ActionParameterEntity< ObjectKnowledge_ABC >( parameter )
{
    const Team_ABC& team = static_cast< const Team_ABC& >( owner.Get< CommunicationHierarchies >().GetTop() );
    SetValue( converter.Find( resolver.Get( ReadId( xis ) ), team ) );
    if( ! GetValue() )
        throw std::runtime_error( tools::translate( "ActionParameter", "Object knowledge not found." ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjectKnowledge destructor
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
ActionParameterObjectKnowledge::~ActionParameterObjectKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjectKnowledge::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ActionParameterObjectKnowledge::CommitTo( ASN1T_MissionParameter& asn ) const
{
    asn.value.t = T_MissionParameter_value_objectKnowledge;
    ActionParameterEntity< ObjectKnowledge_ABC >::CommitTo( (ASN1T_OID&)asn.value.u.objectKnowledge );
    asn.null_value = asn.value.u.objectKnowledge ? 0 : 1;
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjectKnowledge::Accept
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ActionParameterObjectKnowledge::Accept( ActionParameterVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjectKnowledge::CommitTo
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ActionParameterObjectKnowledge::CommitTo( ASN1T_ObjectKnowledge& asn ) const
{
    ActionParameterEntity< ObjectKnowledge_ABC >::CommitTo( (ASN1T_OID&)asn );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterObjectKnowledge::Serialize
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
void ActionParameterObjectKnowledge::Serialize( xml::xostream& xos ) const
{
    if( ! GetValue() )
        throw std::runtime_error( tools::translate( "ActionParameter", "Invalid object knowledge." ).ascii() );
    ActionParameter< const ObjectKnowledge_ABC* >::Serialize( xos );
    xos << xml::attribute( "value", GetValue()->GetEntity()->GetId() ); // $$$$ SBO 2007-05-24: 
}
