// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_pch.h"
#include "PullFlowParameters.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>
#include <boost/foreach.hpp>

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: PullFlowParameters constructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
PullFlowParameters::PullFlowParameters( const OrderParameter& parameter )
    : Parameter< QString >( parameter )
    , supplierFormation_  ( 0 )
    , supplierAutomat_    ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PullFlowParameters constructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
PullFlowParameters::PullFlowParameters( const kernel::OrderParameter& parameter, const kernel::EntityResolver_ABC& entityResolver, const tools::Resolver_ABC< kernel::DotationType >& dotationTypeResolver, const tools::Resolver_ABC< kernel::EquipmentType >& equipmentTypeResolver, xml::xistream& xis )
    : Parameter< QString >( parameter )
    , supplierFormation_  ( 0 )
    , supplierAutomat_    ( 0 )
{
    unsigned int idTmp;
    xis >> xml::start( "supplier" )
        >> xml::attribute< unsigned int >( "id", idTmp )
        >> xml::end;

    supplierAutomat_ = entityResolver.FindAutomat( idTmp );
    if( !supplierAutomat_ )
        supplierFormation_ = &entityResolver.GetFormation( idTmp) ;

    xis >> xml::list( "resource", *this, &PullFlowParameters::ReadResource, dotationTypeResolver )
        >> xml::list( "transporter", *this, &PullFlowParameters::ReadTransporter, equipmentTypeResolver );
}

// -----------------------------------------------------------------------------
// Name: PullFlowParameters destructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
PullFlowParameters::~PullFlowParameters()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PullFlowParameters::ReadResource
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void PullFlowParameters::ReadResource( xml::xistream& xis, const tools::Resolver_ABC< kernel::DotationType >& dotationTypeResolver )
{
    const unsigned int idTmp = xis.attribute< unsigned int >("id" );
    const DotationType& dotationType = dotationTypeResolver.Get( idTmp );
    const unsigned int quantity = xis.attribute< unsigned int >("quantity" );
    resources_[ &dotationType ] += quantity;
}

// -----------------------------------------------------------------------------
// Name: PullFlowParameters::ReadTransporter
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void PullFlowParameters::ReadTransporter( xml::xistream& xis, const tools::Resolver_ABC< kernel::EquipmentType >& equipmentTypeResolver )
{
    const unsigned int idTmp = xis.attribute< unsigned int >( "id" );
    const EquipmentType& type = equipmentTypeResolver.Get( idTmp );
    const unsigned int quantity = xis.attribute< unsigned int >( "quantity" );
    transporters_[ &type ] += quantity;
}

// -----------------------------------------------------------------------------
// Name: PullFlowParameters::SetSupplier
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void PullFlowParameters::SetSupplier( const kernel::Automat_ABC& supplierAutomat )
{
    supplierFormation_ = 0;
    supplierAutomat_ = &supplierAutomat;
}

// -----------------------------------------------------------------------------
// Name: PullFlowParameters::SetSupplier
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void PullFlowParameters::SetSupplier( const kernel::Formation_ABC& supplierFormation )
{
    supplierFormation_ = &supplierFormation;
    supplierAutomat_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PullFlowParameters::AddResource
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void PullFlowParameters::AddResource( const kernel::DotationType& type, unsigned long quantity )
{
    resources_[ &type ] += quantity;
}

// -----------------------------------------------------------------------------
// Name: PullFlowParameters::CommitTo
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void PullFlowParameters::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( false );
    CommitTo( *message.mutable_value()->Add() );
}

// -----------------------------------------------------------------------------
// Name: PullFlowParameters::CommitTo
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void PullFlowParameters::CommitTo( sword::MissionParameter_Value& message ) const
{
    sword::PullFlowParameters* msgPullFlow = message.mutable_pull_flow_parameters();
    if( supplierAutomat_ )
        msgPullFlow->mutable_supplier()->mutable_automat()->set_id( supplierAutomat_->GetId() );
    else if( supplierFormation_ )
        msgPullFlow->mutable_supplier()->mutable_formation()->set_id( supplierFormation_->GetId() );
    BOOST_FOREACH( const T_Resources::value_type& resource, resources_ )
    {
        sword::SupplyFlowResource* msgResource = msgPullFlow->add_resources();
        msgResource->mutable_resourcetype()->set_id( resource.first->GetId() );
        msgResource->set_quantity( resource.second );
    }
    BOOST_FOREACH( const T_Equipments::value_type& equipment, transporters_ )
    {
        sword::SupplyFlowTransporter* msg = msgPullFlow->add_transporters();
        msg->mutable_equipmenttype()->set_id( equipment.first->GetId() );
        msg->set_quantity( equipment.second );
    }
}

// -----------------------------------------------------------------------------
// Name: PullFlowParameters::Serialize
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void PullFlowParameters::Serialize( xml::xostream& xos ) const
{
    assert( supplierAutomat_ || supplierFormation_ );
    Parameter< QString >::Serialize( xos );
    xos << xml::start( "supplier" );
        xos << xml::attribute( "id", supplierAutomat_ ? supplierAutomat_->GetId() : supplierFormation_->GetId() );
    xos << xml::end;

    BOOST_FOREACH( const T_Resources::value_type& resource, resources_ )
    {
        xos << xml::start( "resource" );
        xos << xml::attribute( "id", resource.first->GetId() );
        xos << xml::attribute( "quantity", resource.second );
        xos << xml::end;
    }
    BOOST_FOREACH( const T_Equipments::value_type& equipment, transporters_ )
    {
        xos << xml::start( "transporter" )
                << xml::attribute( "id", equipment.first->GetId() )
                << xml::attribute( "quantity", equipment.second )
            << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: PullFlowParameters::IsSet
// Created: SBO 2008-03-19
// -----------------------------------------------------------------------------
bool PullFlowParameters::IsSet() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: PullFlowParameters::SerializeType
// Created: LDC 2011-05-13
// -----------------------------------------------------------------------------
std::string PullFlowParameters::SerializeType() const
{
    return "pullFlowParameters";
}
