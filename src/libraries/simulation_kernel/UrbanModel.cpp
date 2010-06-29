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
    if( singleton )
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
            if( object.GetId() == id_ )
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
// Name: UrbanModel::Accept
// Created: SLG 2010-06-23
// -----------------------------------------------------------------------------
void UrbanModel::Accept( urban::ObjectVisitor_ABC& visitor )
{
    model_->Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::Accept
// Created: SLG 2010-06-23
// -----------------------------------------------------------------------------
void UrbanModel::Accept( urban::TerrainObjectVisitor_ABC& visitor )
{
    model_->Accept( visitor );
}
