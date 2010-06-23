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
#include "Entities/Objects/StructuralCapacity.h"
#include "Entities/Objects/UrbanObjectWrapper.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <boost/bind.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <Urban/Model.h>
#include <Urban/TerrainObject_ABC.h>
#include <Urban/ColorRGBA.h>
#include <Urban/PhysicalFeature_ABC.h>
#include <Urban/WorldParameters.h>
#include <Urban/Architecture.h>
#include <Urban/TerrainObjectVisitor_ABC.h>
#include "Tools/MIL_Config.h"
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_IMPLEMENT( UrbanModel )

using namespace xml;

namespace
{
    UrbanModel* singleton = 0;
}
// -----------------------------------------------------------------------------
// Name: UrbanModel constructor
// Created: SLG 2009-08-10
// -----------------------------------------------------------------------------
UrbanModel::UrbanModel()
    : model_( new urban::Model() )
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
    for( std::vector< UrbanObjectWrapper* >::iterator it = urbanWrappers_.begin(); it != urbanWrappers_.end(); ++it )
    {
        delete *it;
    }
    singleton = 0;
}

namespace
{
    class UrbanWrapperVisitor : public urban::TerrainObjectVisitor_ABC
    {
    public:
        UrbanWrapperVisitor( UrbanModel& model ) : model_( model ) 
        {}
        ~UrbanWrapperVisitor(){}
        virtual void VisitBlock( urban::TerrainObject_ABC& object )
        {
             model_.CreateObjectWrapper( object );
        }
    private: 
        UrbanModel& model_;
    };
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
         UrbanWrapperVisitor visitor( *this );
         model_->Accept( visitor );
         LoadCapacities( config );
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Exception in loading Urban Model caught : " << e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::CreateObjectWrapper
// Created: SLG 2010-06-18
// -----------------------------------------------------------------------------
void UrbanModel::CreateObjectWrapper( urban::TerrainObject_ABC& object )
{
    urbanWrappers_.push_back( new UrbanObjectWrapper( object ) );
}

namespace
{
    class UrbanSendingCreationVisitor : public urban::TerrainObjectVisitor_ABC
    {
    public:
        UrbanSendingCreationVisitor( const UrbanModel& model ) : model_( model )
        {}
        ~UrbanSendingCreationVisitor()
        {}
        virtual void VisitBlock( urban::TerrainObject_ABC& urbanObject )
        {
            model_.SendCreation( urbanObject );
        }
    private:
        const UrbanModel& model_;
    };
}
// -----------------------------------------------------------------------------
// Name: UrbanModel::SendStateToNewClient
// Created: SLG 2009-10-18
// -----------------------------------------------------------------------------
void UrbanModel::SendStateToNewClient() const
{
    UrbanSendingCreationVisitor visitor( *this );
    model_->Accept( visitor );   // $$$$ _RC_ SLG 2010-06-04: seul les feuilles sont envoyées a gaming ie les blocs urbains
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
void UrbanModel::WriteUrbanModel( xostream& /*xos*/ ) const
{
    //TODO
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::SendCreation
// Created: SLG 2009-10-29
// -----------------------------------------------------------------------------
void UrbanModel::SendCreation( urban::TerrainObject_ABC& object )
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
        message().mutable_attributes()->mutable_architecture()->set_roof_shape( architecture->GetRoofShape().c_str() );
        message().mutable_attributes()->mutable_architecture()->set_material( architecture->GetMaterial().c_str() );
        message().mutable_attributes()->mutable_architecture()->set_occupation( architecture->GetOccupation() );
        message().mutable_attributes()->mutable_architecture()->set_trafficability( architecture->GetTrafficability() );
    }
    message.Send( NET_Publisher_ABC::Publisher() );
}

namespace
{
    class FindUrbanBlockVisitor : public urban::TerrainObjectVisitor_ABC
    {
    public:
        FindUrbanBlockVisitor( unsigned id ) : id_( id ), foundObject_( 0 )
        {}
        ~FindUrbanBlockVisitor()
        {}
        virtual void VisitBlock( urban::TerrainObject_ABC& object )
        {
            if ( object.GetId() == id_ )
                foundObject_ = &object;
        }
        urban::TerrainObject_ABC* GetObject(){ return foundObject_; }
    private:
        unsigned id_;
        urban::TerrainObject_ABC* foundObject_;
    };
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::FindUrbanBlock
// Created: SLG 2009-10-29
// -----------------------------------------------------------------------------
urban::TerrainObject_ABC* UrbanModel::FindUrbanBlock( unsigned id ) const
{
    FindUrbanBlockVisitor visitor( id );
    model_->Accept( visitor );
    return visitor.GetObject();
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

// -----------------------------------------------------------------------------
// Name: UrbanModel::FindWrapper
// Created: SLG 2010-06-21
// -----------------------------------------------------------------------------
UrbanObjectWrapper& UrbanModel::FindWrapper( const urban::TerrainObject_ABC& terrain )
{
    for( std::vector< UrbanObjectWrapper* >::const_iterator it = urbanWrappers_.begin(); it != urbanWrappers_.end(); ++it )
        if( &( ( *it )->GetObject() ) == &terrain )
            return **it;
    throw std::exception( "UrbanObjectWrapper not found" );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::FindWrapper
// Created: JSR 2010-06-21
// -----------------------------------------------------------------------------
UrbanObjectWrapper& UrbanModel::FindWrapper( unsigned int id )
{
    for( std::vector< UrbanObjectWrapper* >::const_iterator it = urbanWrappers_.begin(); it != urbanWrappers_.end(); ++it )
        if( ( *it )->GetID() == id )
            return **it;
    throw std::exception( "UrbanObjectWrapper not found" );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::LoadCapacities
// Created: JSR 2010-06-21
// -----------------------------------------------------------------------------
void UrbanModel::LoadCapacities( const MIL_Config& config )
{
    const std::string strUrbanState = config.GetUrbanStateFile();
    if( strUrbanState.empty() )
        return;

    MT_LOG_INFO_MSG( MT_FormatString( "UrbanState file name : '%s'", strUrbanState.c_str() ) );

    xifstream xis( strUrbanState );
    xis >> start( "urban-state" )
            >> start( "blocks" )
                >> xml::list( "block", boost::bind( &UrbanModel::ReadBlock, boost::ref( *this ), _1 ) )
            >> end()
        >> end();
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::ReadBlock
// Created: JSR 2010-06-21
// -----------------------------------------------------------------------------
void UrbanModel::ReadBlock( xistream& xis )
{
    unsigned int id;
    xis >> attribute( "id", id );
    try
    {
        UrbanObjectWrapper& wrapper = FindWrapper( id );
        xis >> optional() >> start( "capacities" )
            >> xml::list( *this, &UrbanModel::ReadCapacity, wrapper )
            >> end();
    }
    catch( std::exception& )
    {
        // NOTHING
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::ReadAttribute
// Created: JSR 2010-06-21
// -----------------------------------------------------------------------------
void UrbanModel::ReadCapacity( const std::string& capacity, xistream& xis, UrbanObjectWrapper& wrapper )
{
    if( capacity == "structural" )
        wrapper.AddCapacity( new StructuralCapacity( xis ) );
}
