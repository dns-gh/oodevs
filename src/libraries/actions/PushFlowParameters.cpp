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
#include "LocationBase.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: PushFlowParameters constructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
PushFlowParameters::PushFlowParameters( const OrderParameter& parameter, const CoordinateConverter_ABC& converter )
    : Parameter< QString >( parameter )
    , converter_          ( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PushFlowParameters constructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
PushFlowParameters::PushFlowParameters( const kernel::OrderParameter& parameter, const CoordinateConverter_ABC& converter, const kernel::EntityResolver_ABC& entityResolver, const tools::Resolver_ABC< kernel::DotationType >& dotationTypeResolver, const tools::Resolver_ABC< kernel::EquipmentType >& equipmentTypeResolver, xml::xistream& xis )
    : Parameter< QString >( parameter )
    , converter_          ( converter )
{
    xis >> xml::list( "recipient", *this, &PushFlowParameters::ReadRecipient, entityResolver, dotationTypeResolver )
        >> xml::list( "transporter", *this, &PushFlowParameters::ReadTransporter, equipmentTypeResolver )
        >> xml::start( "wayBackPath" )
            >> xml::list( "point", *this, &PushFlowParameters::ReadPoint, wayBackPath_ )
        >> xml::end;
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
    Recipient& recipient = recipients_[ &automat ];
    xis >> xml::list( "resource", *this, &PushFlowParameters::ReadResource, dotationTypeResolver, recipient.resources_ )
        >> xml::start( "path" )
            >> xml::list( "point", *this, &PushFlowParameters::ReadPoint, recipient.path_ )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PushFlowParameters::ReadResource
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void PushFlowParameters::ReadResource( xml::xistream& xis, const tools::Resolver_ABC< kernel::DotationType >& dotationTypeResolver, T_Resources& resources )
{
    const unsigned int idTmp = xis.attribute< unsigned int >( "id" );
    const DotationType& dotationType = dotationTypeResolver.Get( idTmp );
    const unsigned int quantity = xis.attribute< unsigned int >( "quantity" );
    resources[ &dotationType ] += quantity;
}

// -----------------------------------------------------------------------------
// Name: PushFlowParameters::ReadTransporter
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void PushFlowParameters::ReadTransporter( xml::xistream& xis, const tools::Resolver_ABC< kernel::EquipmentType >& equipmentTypeResolver )
{
    const unsigned int idTmp = xis.attribute< unsigned int >( "id" );
    const EquipmentType& type = equipmentTypeResolver.Get( idTmp );
    const unsigned int quantity = xis.attribute< unsigned int >( "quantity" );
    transporters_[ &type ] += quantity;
}

// -----------------------------------------------------------------------------
// Name: PushFlowParameters::ReadPoint
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
void PushFlowParameters::ReadPoint( xml::xistream& xis, T_PointVector& points )
{
    std::string mgrs;
    xis >> xml::attribute( "coordinates", mgrs );

    std::vector< std::string > result;
    boost::algorithm::split( result, mgrs, boost::is_any_of(" ") );
    geometry::Point2f point;
    if( result.size() == 2 ) //Location in WGS84
    {
        point = converter_.ConvertFromGeo( geometry::Point2d( boost::lexical_cast< double >( result[ 0 ] ),
                                                      boost::lexical_cast< double >( result[ 1 ] ) ) );
    }
    else
        point = converter_.ConvertToXY( mgrs );
    points.push_back( point );
}

// -----------------------------------------------------------------------------
// Name: PushFlowParameters::AddResource
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void PushFlowParameters::AddResource( const kernel::DotationType& type, unsigned long quantity, const kernel::Automat_ABC& recipient )
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
void PushFlowParameters::SetPath( const T_PointVector& path, const kernel::Automat_ABC& recipient )
{
    recipients_[ &recipient ].path_ = path;
}

// -----------------------------------------------------------------------------
// Name: PushFlowParameters::SetWayBackPath
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void PushFlowParameters::SetWayBackPath( const T_PointVector& path )
{
    wayBackPath_ = path;
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
    BOOST_FOREACH( const T_Recipients::value_type& recipientData, recipients_ )
    {
        sword::SupplyFlowRecipient* msgRecipient = msgPushFlow->add_recipients();
        msgRecipient->mutable_receiver()->set_id( recipientData.first->GetId() );
        BOOST_FOREACH( const T_Resources::value_type& resource, recipientData.second.resources_ )
        {
            sword::SupplyFlowResource* msgResource = msgRecipient->add_resources();
            msgResource->mutable_resourcetype()->set_id( resource.first->GetId() );
            msgResource->set_quantity( resource.second );
        }

        //$$$ Un rien trop compliqué ...
        const T_PointVector& path = recipientData.second.path_;
        if( !path.empty() )
            CommitTo( path, *msgRecipient->mutable_path() );
    }
    BOOST_FOREACH( const T_Equipments::value_type& equipment, transporters_ )
    {
        sword::SupplyFlowTransporter* msg = msgPushFlow->add_transporters();
        msg->mutable_equipmenttype()->set_id( equipment.first->GetId() );
        msg->set_quantity( equipment.second );
    }

    if( !wayBackPath_.empty() )
        CommitTo( wayBackPath_, *msgPushFlow->mutable_waybackpath() );
}

// -----------------------------------------------------------------------------
// Name: PushFlowParameters::CommitTo
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void PushFlowParameters::CommitTo( const T_PointVector& path, sword::PointList& msgPath ) const
{
    BOOST_FOREACH( const geometry::Point2f& point, path )
    {
        sword::Location& msgPoint = *msgPath.add_elem()->mutable_location();
        msgPoint.set_type( sword::Location_Geometry_point );
        converter_.ConvertToGeo( point, *msgPoint.mutable_coordinates()->add_elem() );
    }
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
    BOOST_FOREACH( const T_Recipients::value_type& recipientData, recipients_ )
    {
        xos << xml::start( "recipient" )
                << xml::attribute( "id", recipientData.first->GetId() );
        BOOST_FOREACH( const T_Resources::value_type& resource, recipientData.second.resources_ )
        {
            xos << xml::start( "resource" )
                    << xml::attribute( "id", resource.first->GetId() )
                    << xml::attribute( "quantity", resource.second )
                << xml::end;
        }
        Serialize( recipientData.second.path_, "path", xos );
        xos << xml::end;
    }
    BOOST_FOREACH( const T_Equipments::value_type& equipment, transporters_ )
    {
        xos << xml::start( "transporter" )
                << xml::attribute( "id", equipment.first->GetId() )
                << xml::attribute( "quantity", equipment.second )
            << xml::end;
    }
    Serialize( wayBackPath_, "wayBackPath", xos );
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
