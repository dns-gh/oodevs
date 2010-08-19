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
#include "Tools/MIL_Config.h"
#include <MT/MT_IO/MT_FormatString.h>
#include <Urban/Model.h>
#include <Urban/ObjectVisitor_ABC.h>
#include <Urban/TerrainObject_ABC.h>
#include <Urban/WorldParameters.h>
#include <xeumeuleu/xml.hpp>
#include <boost/filesystem/path.hpp>

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
void UrbanModel::WriteUrbanModel( xml::xostream& /*xos*/ ) const
{
    //TODO
}

namespace
{
    class FindUrbanObjectVisitor : public urban::ObjectVisitor_ABC
    {
    public:
        FindUrbanObjectVisitor( unsigned id )
            : id_         ( id )
            , foundObject_( 0 )
        {}
        ~FindUrbanObjectVisitor()
        {}
        virtual void Visit( const urban::TerrainObject_ABC& object )
        {
            if( object.GetId() == id_ )
                foundObject_ = &object;
        }
        const urban::TerrainObject_ABC* GetObject(){ return foundObject_; }
    private:
        unsigned id_;
        const urban::TerrainObject_ABC* foundObject_;
    };
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::FindUrbanObject
// Created: SLG 2009-10-29
// -----------------------------------------------------------------------------
const urban::TerrainObject_ABC* UrbanModel::FindUrbanObject( unsigned id ) const
{
    FindUrbanObjectVisitor visitor( id );
    model_->Accept( visitor );
    return visitor.GetObject();
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::FindUrbanBlock
// Created: RPD 2009-11-23
// -----------------------------------------------------------------------------
MT_Float UrbanModel::GetUrbanBlockCost( MT_Float weight, const MT_Vector2D& start, const MT_Vector2D& end ) const
{
    const geometry::Point2f from( static_cast< float >( start.rX_ ), static_cast< float >( start.rY_ ) );
    const geometry::Point2f to( static_cast< float >( end.rX_ ), static_cast< float >( end.rY_ ) );
    return model_->GetUrbanBlockCost( static_cast< float >( weight ), from, to );
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
