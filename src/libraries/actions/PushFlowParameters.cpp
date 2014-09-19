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
#include "Helpers.h"

#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Pathfind_ABC.h"
#include "clients_kernel/XmlAdapter.h"
#include "protocol/Protocol.h"

#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: PushFlowParameters constructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
PushFlowParameters::PushFlowParameters( const kernel::OrderParameter& parameter,
                                        const kernel::CoordinateConverter_ABC& converter,
                                        bool isSupply )
    : Parameter< QString >( parameter )
    , converter_( converter )
    , isSupply_( isSupply )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PushFlowParameters constructor
// Created: ABR 2014-01-15
// -----------------------------------------------------------------------------
PushFlowParameters::PushFlowParameters( const kernel::OrderParameter& parameter,
                                        const kernel::CoordinateConverter_ABC& converter,
                                        const kernel::EntityResolver_ABC& entityResolver,
                                        const tools::Resolver_ABC< kernel::DotationType >& dotationTypeResolver,
                                        const tools::Resolver_ABC< kernel::EquipmentType >& equipmentTypeResolver,
                                        const sword::PushFlowParameters& parameters )
    : Parameter< QString >( parameter )
    , converter_( converter )
    , isSupply_( parameters.has_supply() ? parameters.supply() : false )
{
    for( int i = 0; i < parameters.recipients_size(); ++i )
    {
        const sword::SupplyFlowRecipient& protoRecipient = parameters.recipients( i );
        const kernel::Automat_ABC* automat = entityResolver.FindAutomat( protoRecipient.receiver().id() );
        if( !automat )
            continue; // if somehow the automat has been deleted or not created yet
        Recipient& recipient = recipients_[ automat ];
        recipientsSequence_.push_back( automat );
        for( int j = 0; j < protoRecipient.resources_size(); ++j )
        {
            const sword::SupplyFlowResource& resource = protoRecipient.resources( j );
            const kernel::DotationType& dotationType = dotationTypeResolver.Get( resource.resourcetype().id() );
            recipient.resources_[ &dotationType ] += resource.quantity();
        }
        if( protoRecipient.has_pathfind() )
            recipient.path_ = protoRecipient.pathfind();
    }
    for( int i = 0; i < parameters.transporters_size(); ++i )
    {
        const sword::SupplyFlowTransporter& transporter = parameters.transporters( i );
        const kernel::EquipmentType& type = equipmentTypeResolver.Get( transporter.equipmenttype().id() );
        transporters_[ &type ] += transporter.quantity();
    }
    if( parameters.has_waybackpathfind() )
        wayBackPath_ = parameters.waybackpathfind();
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
// Name: PushFlowParameters::AddResource
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void PushFlowParameters::AddResource( const kernel::DotationType& type, unsigned long quantity, const kernel::Entity_ABC& recipient )
{
    recipients_[ &recipient ].resources_[ &type ] += quantity;
}

// -----------------------------------------------------------------------------
// Name: PushFlowParameters::AddTransporter
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void PushFlowParameters::AddTransporter( const kernel::EquipmentType& type, unsigned long quantity )
{
    transporters_[ &type ] += quantity;
}

// -----------------------------------------------------------------------------
// Name: PushFlowParameters::SetPath
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void PushFlowParameters::SetPath( const kernel::Pathfind_ABC* pathfind, const kernel::Entity_ABC& recipient )
{
    if( pathfind )
        recipients_[ &recipient ].path_ = pathfind->GetCreationMessage();
    recipientsSequence_.push_back( &recipient );
}

// -----------------------------------------------------------------------------
// Name: PushFlowParameters::SetWayBackPath
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void PushFlowParameters::SetWayBackPath( const kernel::Pathfind_ABC& pathfind )
{
    wayBackPath_ = pathfind.GetCreationMessage();
}

// -----------------------------------------------------------------------------
// Name: PushFlowParameters::CommitTo
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void PushFlowParameters::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( false );
    CommitTo( *message.add_value() );
}

// -----------------------------------------------------------------------------
// Name: PushFlowParameters::CommitTo
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void PushFlowParameters::CommitTo( sword::MissionParameter_Value& message ) const
{
    sword::PushFlowParameters* msgPushFlow = message.mutable_push_flow_parameters();
    for( auto it = recipientsSequence_.begin(); it != recipientsSequence_.end() ; ++it )
    {
        const kernel::Entity_ABC* pAutomat = *it;
        auto itRecipient = recipients_.find( pAutomat );
        if( !pAutomat || itRecipient == recipients_.end() )
            continue;
        const Recipient& recipient = itRecipient->second;
        sword::SupplyFlowRecipient* msgRecipient = msgPushFlow->add_recipients();
        msgRecipient->mutable_receiver()->set_id( pAutomat->GetId() );
        BOOST_FOREACH( const T_Resources::value_type& resource, recipient.resources_ )
        {
            sword::SupplyFlowResource* msgResource = msgRecipient->add_resources();
            msgResource->mutable_resourcetype()->set_id( resource.first->GetId() );
            msgResource->set_quantity( resource.second );
        }
        if( recipient.path_ )
             *msgRecipient->mutable_pathfind() = *recipient.path_;
    }
    BOOST_FOREACH( const T_Equipments::value_type& equipment, transporters_ )
    {
        sword::SupplyFlowTransporter* msg = msgPushFlow->add_transporters();
        msg->mutable_equipmenttype()->set_id( equipment.first->GetId() );
        msg->set_quantity( equipment.second );
    }

    if( wayBackPath_ )
        *msgPushFlow->mutable_waybackpathfind() = *wayBackPath_;

    if( isSupply_ )
        msgPushFlow->set_supply( true );
}

// -----------------------------------------------------------------------------
// Name: PushFlowParameters::Serialize
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void PushFlowParameters::Serialize( const T_PointVector& path, const std::string& tag, xml::xostream& xos ) const
{
    xos << xml::start( tag );
    BOOST_FOREACH( const geometry::Point2f& point, path )
        xos << xml::start( "point" ) << xml::attribute( "coordinates", converter_.ConvertToMgrs( point ) ) << xml::end;
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: PushFlowParameters::Serialize
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void PushFlowParameters::Serialize( xml::xostream& xos ) const
{
    Parameter< QString >::Serialize( xos );
    sword::MissionParameter message;
    CommitTo( message );
    const kernel::XmlWriterAdapter adapter( converter_ );
    protocol::Write( xos, adapter, message );
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
