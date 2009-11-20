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
#include "simulation_orders/MIL_ParameterType_PolygonList.h"
#include "Network/NET_ASN_Tools.h"

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
MIL_PolygonListParameter::MIL_PolygonListParameter( const ASN1T_PolygonList& asn )
{   
    for( unsigned int i = 0; i < asn.n; ++i )
    {
        if( asn.elem[i].type != EnumLocationType::polygon || asn.elem[i].coordinates.n < 3 )
            throw std::runtime_error( "Bad polygon in polygon list" );
    }

    polygonList_.resize( asn.n ); 
    for( unsigned int i = 0; i < asn.n; ++i )
    {
        polygonList_[i].reset( new TER_Localisation() );
        if( !NET_ASN_Tools::ReadLocation( asn.elem[i], *polygonList_[i] ) )
            throw std::runtime_error( "Unable to create polygon in polygon list" );
    }
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
    return( dynamic_cast<const MIL_ParameterType_PolygonList*>( &type ) != 0 );
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
bool MIL_PolygonListParameter::ToPolygonList( ASN1T_PolygonList& asn ) const
{
    asn.n = polygonList_.size();
    if( !polygonList_.empty() )
    {
        ASN1T_Location* pCoord = new ASN1T_Location[ polygonList_.size() ]; //$$$ RAM
        asn.elem = pCoord;

        for( unsigned int i = 0; i < polygonList_.size(); ++i )
            NET_ASN_Tools::WriteLocation( *polygonList_[i], pCoord[i] );
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
