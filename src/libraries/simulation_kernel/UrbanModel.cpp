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
#include "BlockPhFirerModifier.h"
#include "BlockPhTargetModifier.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "Tools/MIL_Config.h"
#include <boost/bind.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <Urban/Model.h>
#include <Urban/BlockModel.h>
#include <Urban/TerrainObject_ABC.h>
#include <Urban/Block.h>
#include <Urban/BlockModel.h>
#include <Urban/BlockPhModifier_ABC.h>
#include <Urban/ColorRGBA.h>
#include <Urban/PhysicalFeature_ABC.h>
#include <Urban/WorldParameters.h>
#include <Urban/Architecture.h>
#include <Urban/Soil.h>
#include <Urban/Vegetation.h>
#include "Tools/MIL_Config.h"
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_IMPLEMENT( UrbanModel )

namespace
{
    UrbanModel* singleton = 0;
}
// -----------------------------------------------------------------------------
// Name: UrbanModel constructor
// Created: SLG 2009-08-10
// -----------------------------------------------------------------------------
UrbanModel::UrbanModel()
    : model_            ( new urban::Model() )
    , phFirerModifier_  ( new BlockPhFirerModifier() )
    , phTargetModifier_ ( new BlockPhTargetModifier() )
{
    if ( singleton )
        throw std::runtime_error( "urbanModel already registered" );
    singleton = this;
}

// -----------------------------------------------------------------------------
// Name: UrbanModel destructor
// Created: SLG 2009-08-11
// -----------------------------------------------------------------------------
UrbanModel::~UrbanModel()
{
    singleton = 0;
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
    client::UrbanCreation message;
    message().set_oid( object.GetId() );
    message().set_name( object.GetName() );
    message().mutable_location()->set_type( Common::MsgLocation_Geometry_polygon );
    const geometry::Polygon2f::T_Vertices& points = object.GetFootprint()->Vertices();
    for( geometry::Polygon2f::CIT_Vertices it = points.begin(); it != points.end(); ++it )
    {
        Common::MsgCoordLatLong* point = message().mutable_location()->mutable_coordinates()->add_elem();
        point->set_latitude( it->X() );
        point->set_longitude( it->Y() );
            
    }

    const ColorRGBA* color = object.GetColor();
    if ( color != 0 )
    {
        message().mutable_attributes()->mutable_color()->set_red( color->Red() );
        message().mutable_attributes()->mutable_color()->set_green( color->Green() );
        message().mutable_attributes()->mutable_color()->set_blue( color->Blue() );
        message().mutable_attributes()->mutable_color()->set_alpha( color->Alpha() );
    }

    const urban::Architecture* architecture = object.RetrievePhysicalFeature< urban::Architecture >();
    if ( architecture != 0 )
    {       
        message().mutable_attributes()->mutable_architecture()->set_height( architecture->GetHeight() );
        message().mutable_attributes()->mutable_architecture()->set_floor_number( architecture->GetFloorNumber() );
        message().mutable_attributes()->mutable_architecture()->set_basement_level_number( architecture->GetBasement() );
        message().mutable_attributes()->mutable_architecture()->set_roof_shape( architecture->GetRoofShape().c_str() );
        message().mutable_attributes()->mutable_architecture()->set_material( architecture->GetMaterial().c_str() );
        message().mutable_attributes()->mutable_architecture()->set_inner_cluttering( architecture->GetInnerCluttering() );
        message().mutable_attributes()->mutable_architecture()->set_facade_opacity( architecture->GetFacadeOpacity() );
    }

    const urban::Soil* soil = object.RetrievePhysicalFeature< urban::Soil >();
    if ( soil != 0 )
    {
        message().mutable_attributes()->mutable_soil()->set_occupation( soil->GetOccupation() );
        message().mutable_attributes()->mutable_soil()->set_trafficability( soil->GetTrafficability() );
        message().mutable_attributes()->mutable_soil()->set_multiple( soil->GetMultiplicity() );
        message().mutable_attributes()->mutable_soil()->set_compound_clearing( soil->GetCompoundClearing().c_str() );
    }

    const urban::Vegetation* vegetation = object.RetrievePhysicalFeature< urban::Vegetation >();
    if ( vegetation != 0 )
    {
        message().mutable_attributes()->mutable_vegetation()->set_type( vegetation->GetType().c_str() );
        message().mutable_attributes()->mutable_vegetation()->set_height( vegetation->GetHeight() );
        message().mutable_attributes()->mutable_vegetation()->set_density( vegetation->GetDensity() );
    }
    message.Send( NET_Publisher_ABC::Publisher() );
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
MT_Float UrbanModel::ComputeUrbanPhModifier( const MT_Vector3D& firerPosition, const MT_Vector3D& targetPosition ) const
{
    geometry::Point2f positionF ( firerPosition.rX_, firerPosition.rY_ );
    float firerModifier =  model_->GetUrbanPhModifier( *phFirerModifier_, positionF );

    geometry::Point2f positionT ( targetPosition.rX_, targetPosition.rY_ );
    float targetModifier = model_->GetUrbanPhModifier( *phTargetModifier_, positionT );

    return firerModifier * targetModifier;
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::GetModel
// Created: RPD 2009-12-01
// -----------------------------------------------------------------------------
urban::Model& UrbanModel::GetModel()
{
    return* model_;
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::GetSingleton
// Created: SLG 2010-01-20
// -----------------------------------------------------------------------------
UrbanModel& UrbanModel::GetSingleton()
{
    if( !singleton )
        throw std::runtime_error( "urbanModel not defined" );
    return *singleton;
}