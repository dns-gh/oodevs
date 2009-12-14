// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "UrbanModel.h"
#include <Urban/Model.h>
#include <Urban/BlockModel.h>
#include <Urban/TerrainObject_ABC.h>
#include <Urban/Block.h>
#include <Urban/PhysicalFeature_ABC.h>
#include <Urban/StaticModel.h>
#include <Urban/WorldParameters.h>
#include <Urban/Architecture.h>
#include <Urban/Soil.h>
#include <Urban/Vegetation.h>
#include "Tools/MIL_Config.h"
#include <xeumeuleu/xml.h>
#include <boost/filesystem/path.hpp>
#include "boost/filesystem/operations.hpp"
#include "Network/NET_ASN_Messages.h"
#include <boost/bind.hpp>

using namespace urban;

BOOST_CLASS_EXPORT_GUID( UrbanModel, "UrbanModel" )

// -----------------------------------------------------------------------------
// Name: UrbanModel constructor
// Created: SLG 2009-08-10
// -----------------------------------------------------------------------------
UrbanModel::UrbanModel()
    : staticModel_( new StaticModel() )
    , model_( new Model( *staticModel_ ) )
{
    //TODO
}


// -----------------------------------------------------------------------------
// Name: UrbanModel destructor
// Created: SLG 2009-08-11
// -----------------------------------------------------------------------------
UrbanModel::~UrbanModel()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::ReadODB
// Created: SLG 2009-08-11
// -----------------------------------------------------------------------------
void UrbanModel::ReadUrbanModel( const MIL_Config& config )
{
    MT_LOG_STARTUP_MESSAGE( "--------------------------------" );
    MT_LOG_STARTUP_MESSAGE( "----  Loading UrbanModel    ----" );
    MT_LOG_STARTUP_MESSAGE( "--------------------------------" );

    std::string directoryPath = boost::filesystem::path( config.GetTerrainFile() ).branch_path().string();
    try
    {
         MT_LOG_INFO_MSG( MT_FormatString( "Loading Urban Model from path '%s'", directoryPath.c_str() ) )
         model_->Load( directoryPath, urban::WorldParameters( directoryPath ) );
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Exception in loading Urban Model caught : " << e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::SendStateToNewClient
// Created: SLG 2009-10-18
// -----------------------------------------------------------------------------
void UrbanModel::SendStateToNewClient() const
{
    model_->blocks_.Apply( boost::bind( &UrbanModel::SendCreation, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::load
// Created: SLG 2009-10-29
// -----------------------------------------------------------------------------
void UrbanModel::load( MIL_CheckPointInArchive& file, const uint )
{
    //TODO
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::save
// Created: SLG 2009-10-29
// -----------------------------------------------------------------------------
void UrbanModel::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    //TODO
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::WriteUrbanModel
// Created: SLG 2009-10-29
// -----------------------------------------------------------------------------
void UrbanModel::WriteUrbanModel( xml::xostream& xos ) const
{
    //TODO
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::WriteUrbanModel
// Created: SLG 2009-10-29
// -----------------------------------------------------------------------------
void UrbanModel::SendCreation( urban::Block& object )
{
    NET_ASN_MsgUrbanCreation asn;
    int i = 0;
    geometry::Polygon2f::T_Vertices points = object.GetFootprint()->Vertices();
    asn().oid                       = object.GetId();
    asn().name                      = object.GetName().c_str();
    asn().location.type             = EnumLocationType::polygon;
    asn().location.coordinates.n    = points.size();

    asn().location.coordinates.elem = new ASN1T_CoordLatLong[ points.size() ];

    for ( geometry::Polygon2f::IT_Vertices it = points.begin(); it != points.end(); ++it )
    {
        asn().location.coordinates.elem[ i ].latitude = (*it).X();
        asn().location.coordinates.elem[ i ].longitude = (*it).Y();
        ++i;
    }

    const Architecture* architecture = object.RetrievePhysicalFeature< Architecture >();
    if ( architecture != 0 )
    {       
        asn().attributes.m.architecturePresent              = 1;
        asn().attributes.architecture.height                = architecture->GetHeight();
        asn().attributes.architecture.floorNumber           = architecture->GetFloorNumber();
        asn().attributes.architecture.basementLevelNumber   = architecture->GetBasement();
        asn().attributes.architecture.roofShape             = architecture->GetRoofShape().c_str();
        asn().attributes.architecture.material              = architecture->GetMaterial().c_str();
        asn().attributes.architecture.innerCluttering       = architecture->GetInnerCluttering();
        asn().attributes.architecture.facadeOpacity         = architecture->GetFacadeOpacity();
    }

    const Soil* soil = object.RetrievePhysicalFeature< Soil >();
    if ( soil != 0 )
    {
        asn().attributes.m.soilPresent          = 1;
        asn().attributes.soil.occupation        = soil->GetOccupation();
        asn().attributes.soil.trafficability    = soil->GetTrafficability();
        asn().attributes.soil.multiple          = soil->GetMultiplicity();
        asn().attributes.soil.compoundClearing  = soil->GetCompoundClearing().c_str();
    }

    const Vegetation* vegetation = object.RetrievePhysicalFeature< Vegetation >();
    if ( vegetation != 0 )
    {
        asn().attributes.m.vegetationPresent    = 1;
        asn().attributes.vegetation.type        = vegetation->GetType().c_str();
        asn().attributes.vegetation.height      = vegetation->GetHeight();
        asn().attributes.vegetation.density     = vegetation->GetDensity();
    }
    asn.Send();
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::FindUrbanBlock
// Created: SLG 2009-10-29
// -----------------------------------------------------------------------------
urban::Block* UrbanModel::FindUrbanBlock( unsigned id ) const
{
    return static_cast< Block* >( model_->blocks_.Find( id ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::FindUrbanBlock
// Created: RPD 2009-11-23
// -----------------------------------------------------------------------------
MT_Float UrbanModel::GetUrbanBlockCost( MT_Float weight, const MT_Vector2D& start, const MT_Vector2D& end ) const
{
    geometry::Point2f from ( start.rX_, start.rY_ ), to ( end.rX_, end.rY_ );
    return model_->GetUrbanBlockCost( ( float ) weight, from, to );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::GetModel
// Created: RPD 2009-12-01
// -----------------------------------------------------------------------------
urban::Model& UrbanModel::GetModel() const
{
    return *model_;
}
