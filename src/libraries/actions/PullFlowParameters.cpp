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
#include "Helpers.h"
#include "LocationBase.h"

#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Formation_ABC.h"
#include "protocol/Protocol.h"

#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <xeumeuleu/xml.hpp>

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: PullFlowParameters constructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
PullFlowParameters::PullFlowParameters( const OrderParameter& parameter,
                                        const kernel::CoordinateConverter_ABC& converter )
    : Parameter< QString >( parameter )
    , converter_          ( converter )
    , supplierFormation_  ( 0 )
    , supplierAutomat_    ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PullFlowParameters constructor
// Created: ABR 2014-01-15
// -----------------------------------------------------------------------------
PullFlowParameters::PullFlowParameters( const kernel::OrderParameter& parameter,
                                        const kernel::CoordinateConverter_ABC& converter,
                                        const kernel::EntityResolver_ABC& entityResolver,
                                        const tools::Resolver_ABC< kernel::DotationType >& dotationTypeResolver,
                                        const tools::Resolver_ABC< kernel::EquipmentType >& equipmentTypeResolver,
                                        const sword::PullFlowParameters& parameters )
    : Parameter< QString >( parameter )
    , converter_          ( converter )
    , supplierFormation_  ( parameters.supplier().has_formation() ? entityResolver.FindFormation( parameters.supplier().formation().id() ) : 0 )
    , supplierAutomat_    ( parameters.supplier().has_automat() ? entityResolver.FindAutomat( parameters.supplier().automat().id() ) : 0 )
{
    for( int i = 0; i < parameters.resources_size(); ++i )
    {
        const sword::SupplyFlowResource& resource = parameters.resources( i );
        const DotationType& dotationType = dotationTypeResolver.Get( resource.resourcetype().id() );
        resources_[ &dotationType ] += resource.quantity();
    }
    for( int i = 0; i < parameters.transporters_size(); ++i )
    {
        const sword::SupplyFlowTransporter& transporter = parameters.transporters( i );
        const EquipmentType& type = equipmentTypeResolver.Get( transporter.equipmenttype().id() );
        transporters_[ &type ] += transporter.quantity();
    }
    if( parameters.has_waybackpath() )
        FillFromPointList( wayBackPath_, parameters.waybackpath(), converter );
    if( parameters.has_wayoutpath() )
        FillFromPointList( wayOutPath_, parameters.wayoutpath(), converter );
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
// Name: PullFlowParameters::AddTransporter
// Created: MMC 2011-09-26
// -----------------------------------------------------------------------------
void PullFlowParameters::AddTransporter( const kernel::EquipmentType& type, unsigned long quantity )
{
    transporters_[ &type ] += quantity;
}

// -----------------------------------------------------------------------------
// Name: PullFlowParameters::SetWayOutPath
// Created: MMC 2011-09-26
// -----------------------------------------------------------------------------
void PullFlowParameters::SetWayOutPath( const T_PointVector& path )
{
    wayOutPath_ = path;
}

// -----------------------------------------------------------------------------
// Name: PullFlowParameters::SetWayBackPath
// Created: SBO 2011-09-26
// -----------------------------------------------------------------------------
void PullFlowParameters::SetWayBackPath( const T_PointVector& path )
{
    wayBackPath_ = path;
}

// -----------------------------------------------------------------------------
// Name: PullFlowParameters::CommitTo
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void PullFlowParameters::CommitTo( sword::MissionParameter& message ) const
{
    message.set_null_value( false );
    CommitTo( *message.add_value() );
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

    if( !wayOutPath_.empty() )
        CommitTo( wayOutPath_, *msgPullFlow->mutable_wayoutpath() );
    if( !wayBackPath_.empty() )
        CommitTo( wayBackPath_, *msgPullFlow->mutable_waybackpath() );
}

// -----------------------------------------------------------------------------
// Name: PullFlowParameters::CommitTo
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void PullFlowParameters::CommitTo( const T_PointVector& path, sword::PointList& msgPath ) const
{
    BOOST_FOREACH( const geometry::Point2f& point, path )
    {
        sword::Location& msgPoint = *msgPath.add_elem()->mutable_location();
        msgPoint.set_type( sword::Location_Geometry_point );
        converter_.ConvertToGeo( point, *msgPoint.mutable_coordinates()->add_elem() );
    }
}

// -----------------------------------------------------------------------------
// Name: PullFlowParameters::Serialize
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
void PullFlowParameters::Serialize( const T_PointVector& path, const std::string& tag, xml::xostream& xos ) const
{
    xos << xml::start( tag );
    BOOST_FOREACH( const geometry::Point2f& point, path )
        xos << xml::start( "point" ) << xml::attribute( "coordinates", converter_.ConvertToMgrs( point ) ) << xml::end;
    xos << xml::end;
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
    Serialize( wayOutPath_, "wayoutpath", xos );
    Serialize( wayBackPath_, "waybackpath", xos );
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
