// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_PolygonListParameter.h"
#include "MIL_MissionParameterFactory.h"
#include "simulation_orders/MIL_ParameterType_PolygonList.h"
#include "simulation_terrain/TER_Localisation.h"
#include "Network/NET_ASN_Tools.h"
#include "protocol/protocol.h"

// -----------------------------------------------------------------------------
// Name: MIL_PolygonListParameter constructor
// Created: LDC 2009-11-18
// -----------------------------------------------------------------------------
MIL_PolygonListParameter::MIL_PolygonListParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PolygonListParameter constructor
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
MIL_PolygonListParameter::MIL_PolygonListParameter( const Common::MsgPolygonList& asn )
{
    polygonList_.resize( asn.elem_size() );
    for( int i = 0; i < asn.elem_size(); ++i )
    {
        if( asn.elem(i).location().type() != Common::MsgLocation_Geometry_polygon || asn.elem(i).location().coordinates().elem_size() < 3 )
            throw std::runtime_error( "Bad polygon in polygon list" );
        polygonList_[i].reset( new TER_Localisation() );
        if( !NET_ASN_Tools::ReadLocation( asn.elem( i ).location(), *polygonList_[i] ) )
            throw std::runtime_error( "Unable to create polygon in polygon list" );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_PolygonListParameter constructor
// Created: LDC 2009-12-04
// -----------------------------------------------------------------------------
MIL_PolygonListParameter::MIL_PolygonListParameter( const std::vector< boost::shared_ptr< TER_Localisation > >& polygonList )
: polygonList_( polygonList )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PolygonListParameter destructor
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
MIL_PolygonListParameter::~MIL_PolygonListParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_PolygonListParameter::IsOfType
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_PolygonListParameter::IsOfType( const MIL_ParameterType_ABC& type ) const
{
    return dynamic_cast< const MIL_ParameterType_PolygonList* >( &type ) != 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_PolygonListParameter::Append
// Created: LDC 2009-11-18
// -----------------------------------------------------------------------------
void MIL_PolygonListParameter::Append( boost::shared_ptr< TER_Localisation > pLocation )
{
    polygonList_.push_back( pLocation );
}

// -----------------------------------------------------------------------------
// Name: MIL_PolygonListParameter::ToPolygonList
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_PolygonListParameter::ToPolygonList( Common::MsgPolygonList& asn ) const
{
    if( !polygonList_.empty() )
    {
        for( unsigned int i = 0; i < polygonList_.size(); ++i )
            asn.add_elem();

        for( unsigned int i = 0; i < polygonList_.size(); ++i )
            NET_ASN_Tools::WriteLocation( *polygonList_[i], *asn.mutable_elem( i )->mutable_location() );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PolygonListParameter::ToPolygonList
// Created: LDC 2009-05-22
// -----------------------------------------------------------------------------
bool MIL_PolygonListParameter::ToPolygonList( std::vector< boost::shared_ptr< TER_Localisation > >& value ) const
{
    value = polygonList_;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_LocationListParameter::ToList
// Created: MGD 2010-10-28
// -----------------------------------------------------------------------------
bool MIL_PolygonListParameter::ToList( std::vector< Common::MsgMissionParameter_Value >& result ) const
{
    for(std::vector< boost::shared_ptr< TER_Localisation > >::const_iterator it = polygonList_.begin(); it != polygonList_.end(); ++it )
    {
        Common::MsgMissionParameter_Value param;
        Common::MsgLocation* location = param.mutable_location();
        NET_ASN_Tools::WriteLocation( *(*it), *location );
        result.push_back( param );
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_PolygonListParameter::ToList
// Created: MGD 2010-10-28
// -----------------------------------------------------------------------------
bool MIL_PolygonListParameter::ToList( std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> >& result ) const
{
    for( std::vector< boost::shared_ptr< TER_Localisation > >::const_iterator it = polygonList_.begin(); it != polygonList_.end(); ++it )
    {
        boost::shared_ptr<MIL_MissionParameter_ABC> param = MIL_MissionParameterFactory::Create( *it );
        result.push_back( param );
    }
    return true;
}
