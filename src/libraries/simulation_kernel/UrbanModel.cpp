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
#include <Urban/BlockPhModifier_ABC.h>
#include <Urban/ColorRGBA.h>
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

//using namespace urban;

BOOST_CLASS_EXPORT_GUID( UrbanModel, "UrbanModel" )

// -----------------------------------------------------------------------------
// Name: UrbanModel constructor
// Created: SLG 2009-08-10
// -----------------------------------------------------------------------------
UrbanModel::UrbanModel()
    : staticModel_( new urban::StaticModel() )
    , model_( new urban::Model( *staticModel_ ) )
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
         urban::WorldParameters world ( directoryPath );
         model_->Load( directoryPath, world );
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
void UrbanModel::load( MIL_CheckPointInArchive& /*file*/, const unsigned int )
{
    //TODO
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::save
// Created: SLG 2009-10-29
// -----------------------------------------------------------------------------
void UrbanModel::save( MIL_CheckPointOutArchive& /*file*/, const unsigned int ) const
{
    //TODO
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::WriteUrbanModel
// Created: SLG 2009-10-29
// -----------------------------------------------------------------------------
void UrbanModel::WriteUrbanModel( xml::xostream& /*xos*/ ) const
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
    geometry::Polygon2f::T_Vertices points = object.GetFootprint()->Vertices();
    asn().oid                       = object.GetId();
    asn().name                      = object.GetName().c_str();
    asn().location.type             = EnumLocationType::polygon;
    asn().location.coordinates.n    = points.size();

    asn().location.coordinates.elem = new ASN1T_CoordLatLong[ points.size() ];

    int i ( 0 );
    for ( geometry::Polygon2f::IT_Vertices it = points.begin(); it != points.end(); ++it )
    {
        asn().location.coordinates.elem[ i ].latitude = (*it).X();
        asn().location.coordinates.elem[ i ].longitude = (*it).Y();
        ++i;
    }

    const ColorRGBA* color = object.GetColor();
    if ( color != 0 )
    {
        asn().attributes.m.colorPresent = 1;
        asn().attributes.color.red = color->Red();
        asn().attributes.color.green = color->Green();
        asn().attributes.color.blue = color->Blue();
        asn().attributes.color.alpha = color->Alpha();
    }

    const urban::Architecture* architecture = object.RetrievePhysicalFeature< urban::Architecture >();
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

    const urban::Soil* soil = object.RetrievePhysicalFeature< urban::Soil >();
    if ( soil != 0 )
    {
        asn().attributes.m.soilPresent          = 1;
        asn().attributes.soil.occupation        = soil->GetOccupation();
        asn().attributes.soil.trafficability    = soil->GetTrafficability();
        asn().attributes.soil.multiple          = soil->GetMultiplicity();
        asn().attributes.soil.compoundClearing  = soil->GetCompoundClearing().c_str();
    }

    const urban::Vegetation* vegetation = object.RetrievePhysicalFeature< urban::Vegetation >();
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
    return static_cast< urban::Block* >( model_->blocks_.Find( id ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::FindUrbanBlock
// Created: RPD 2009-11-23
// -----------------------------------------------------------------------------
MT_Float UrbanModel::GetUrbanBlockCost( MT_Float weight, const MT_Vector2D& start, const MT_Vector2D& end ) const
{
    geometry::Point2f from ( ( float ) start.rX_, ( float ) start.rY_ ), to ( ( float ) end.rX_, ( float ) end.rY_ );
    return model_->GetUrbanBlockCost( ( float ) weight, from, to );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::ComputeUrbanPhModifier
// Created: SLG 2010-01-07
// -----------------------------------------------------------------------------
MT_Float UrbanModel::ComputeUrbanPhModifier( urban::BlockPhModifier_ABC& phModifier, const MT_Vector2D& firerPosition ) const
{
    geometry::Point2f position ( firerPosition.rX_, firerPosition.rY_ );
    return model_->GetUrbanPhModifier( phModifier, position );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::GetModel
// Created: RPD 2009-12-01
// -----------------------------------------------------------------------------
urban::Model& UrbanModel::GetModel() const
{
    return *model_;
}
