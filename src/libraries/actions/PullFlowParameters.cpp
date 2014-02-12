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
// Name: PullFlowParameters constructor
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
PullFlowParameters::PullFlowParameters( const kernel::OrderParameter& parameter,
                                        const kernel::CoordinateConverter_ABC& converter,
                                        const kernel::EntityResolver_ABC& entityResolver,
                                        const tools::Resolver_ABC< kernel::DotationType >& dotationTypeResolver,
                                        const tools::Resolver_ABC< kernel::EquipmentType >& equipmentTypeResolver,
                                        xml::xistream& xis )
    : Parameter< QString >( parameter )
    , supplierFormation_  ( 0 )
    , supplierAutomat_    ( 0 )
    , converter_          ( converter )
{
    unsigned int idTmp;
    xis >> xml::start( "supplier" )
        >> xml::attribute< unsigned int >( "id", idTmp )
        >> xml::end;

    supplierAutomat_ = entityResolver.FindAutomat( idTmp );
    if( !supplierAutomat_ )
        supplierFormation_ = &entityResolver.GetFormation( idTmp) ;

    const T_XmlFunctor outpath = boost::bind( &PullFlowParameters::ReadPoint, this, _1, boost::ref( wayOutPath_ ) );
    const T_XmlFunctor backpath = boost::bind( &PullFlowParameters::ReadPoint, this, _1, boost::ref( wayBackPath_ ) );
    xis >> xml::list( "resource", *this, &PullFlowParameters::ReadResource, dotationTypeResolver )
        >> xml::list( "transporter", *this, &PullFlowParameters::ReadTransporter, equipmentTypeResolver )
        >> xml::list( boost::bind( &WalkPointList, "wayoutpath", outpath, _1, _2, _3 ) )
        >> xml::list( boost::bind( &WalkPointList, "waybackpath", backpath, _1, _2, _3 ) );
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
// Name: PullFlowParameters::ReadPoint
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
void PullFlowParameters::ReadPoint( xml::xistream& xis, T_PointVector& points )
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
