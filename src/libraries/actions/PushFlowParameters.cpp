// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "PushFlowParameters.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>
#include <boost/foreach.hpp>

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: PushFlowParameters constructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
PushFlowParameters::PushFlowParameters( const OrderParameter& parameter )
    : Parameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PushFlowParameters constructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
/*PushFlowParameters::PushFlowParameters( const OrderParameter& parameter, const sword::PushFlowParameters& message )
    : Parameter< QString >( parameter )
{
    //for( int i = 0; i < message.elem_size(); ++i )
    //    AddMedicalPriority( E_HumanWound( message.elem().Get(i) ) );
}*/

// -----------------------------------------------------------------------------
// Name: PushFlowParameters constructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
PushFlowParameters::PushFlowParameters( const kernel::OrderParameter& parameter, const kernel::EntityResolver_ABC& entityResolver, const tools::Resolver_ABC< kernel::DotationType >& dotationTypeResolver, xml::xistream& xis )
    : Parameter< QString >( parameter )
{
    xis >> xml::list( "recipient", *this, &PushFlowParameters::ReadRecipient, entityResolver, dotationTypeResolver );
}

// -----------------------------------------------------------------------------
// Name: PushFlowParameters destructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
PushFlowParameters::~PushFlowParameters()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PushFlowParameters::ReadRecipient
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void PushFlowParameters::ReadRecipient( xml::xistream& xis, const kernel::EntityResolver_ABC& entityResolver, const tools::Resolver_ABC< kernel::DotationType >& dotationTypeResolver )
{
    const unsigned int idTmp = xis.attribute< unsigned int >("id" );
    Automat_ABC& automat = entityResolver.GetAutomat( idTmp );
    T_Resources& resources = recipients_[ &automat ];
    xis >> xml::list( "resource", *this, &PushFlowParameters::ReadResource, dotationTypeResolver, resources );
}

// -----------------------------------------------------------------------------
// Name: PushFlowParameters::ReadResource
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void PushFlowParameters::ReadResource( xml::xistream& xis, const tools::Resolver_ABC< kernel::DotationType >& dotationTypeResolver, T_Resources& resources )
{
    const unsigned int idTmp = xis.attribute< unsigned int >("id" );
    const DotationType& dotationType = dotationTypeResolver.Get( idTmp );
    const unsigned int quantity = xis.attribute< unsigned int >("quantity" );
    resources[ &dotationType ] += quantity;
}

// -----------------------------------------------------------------------------
// Name: PushFlowParameters::AddResource
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void PushFlowParameters::AddResource( const kernel::DotationType& type, unsigned long quantity, const kernel::Automat_ABC& recipient )
{
    recipients_[ &recipient ][ &type ] += quantity;
}

// -----------------------------------------------------------------------------
// Name: PushFlowParameters::CommitTo
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void PushFlowParameters::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( false );
    CommitTo( *message.mutable_value()->Add() );
}

// -----------------------------------------------------------------------------
// Name: PushFlowParameters::CommitTo
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void PushFlowParameters::CommitTo( sword::MissionParameter_Value& message ) const
{
    sword::PushFlowParameters* msgPushFlow = message.mutable_push_flow_parameters();
    BOOST_FOREACH( const T_Recipients::value_type& recipient, recipients_ )
    {
        sword::SupplyFlowRecipient* msgRecipient = msgPushFlow->add_recipients();
        msgRecipient->mutable_receiver()->set_id( recipient.first->GetId() );
        BOOST_FOREACH( const T_Resources::value_type& resource, recipient.second )
        {
            sword::SupplyFlowResource* msgResource = msgRecipient->add_resources();
            msgResource->mutable_resourcetype()->set_id( resource.first->GetId() );
            msgResource->set_quantity( resource.second );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PushFlowParameters::Serialize
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void PushFlowParameters::Serialize( xml::xostream& xos ) const
{
    Parameter< QString >::Serialize( xos );
    BOOST_FOREACH( const T_Recipients::value_type& recipient, recipients_ )
    {
        xos << xml::start( "recipient" );
        xos << xml::attribute( "id", recipient.first->GetId() );
        BOOST_FOREACH( const T_Resources::value_type& resource, recipient.second )
        {
            xos << xml::start( "resource" );
            xos << xml::attribute( "id", resource.first->GetId() );
            xos << xml::attribute( "quantity", resource.second );
        }
        xos << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: PushFlowParameters::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool PushFlowParameters::IsSet() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: PushFlowParameters::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string PushFlowParameters::SerializeType() const
{
    return "pushflowparameters";
}
