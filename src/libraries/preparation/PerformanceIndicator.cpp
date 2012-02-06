// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "PerformanceIndicator.h"
#include "Model.h"
#include "AgentsModel.h"
#include "KnowledgeGroupsModel.h"
#include "ObjectsModel.h"
#include "UrbanModel.h"
#include "tools/Loader_ABC.h"
#include <boost/bind.hpp>
#include <boost/filesystem/operations.hpp>
#include <xeumeuleu/xml.hpp>
#include <xeuseuleu/xsl.hpp>
#include <urban/WorldParameters.h>


namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: PerformanceIndicator constructor
// Created: MMC 2012-02-02
// -----------------------------------------------------------------------------
PerformanceIndicator::PerformanceIndicator( const Model& model )
    : model_( model )
    , limit_( 10.f )
    , globalFactor_( 0.001f )
    , unit_( 0.f )
    , urban_( 0.f )
    , object_( 0.f )
    , terrain_( 0.f )
    , knowledge_( 0.f )
    , unitknowledge_( 0.f )
    , loaded_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerformanceIndicator destructor
// Created: MMC 2012-02-02
// -----------------------------------------------------------------------------
PerformanceIndicator::~PerformanceIndicator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerformanceIndicator::Load
// Created: MMC 2012-02-02
// -----------------------------------------------------------------------------
void PerformanceIndicator::Load( const tools::Loader_ABC& fileLoader, const std::string& file )
{
    try
    {
        if( bfs::exists( file ) )
        {
            loaded_ = true;
            fileLoader.CheckFile( file );
            fileLoader.LoadFile( file, boost::bind( &PerformanceIndicator::Read, this, _1 ) );
        }
    }
    catch( ... )
    {
        // NOTHING
    }
}

// -----------------------------------------------------------------------------
// Name: PerformanceIndicator::Read
// Created: MMC 2012-02-02
// -----------------------------------------------------------------------------
void PerformanceIndicator::Read( xml::xistream& xis )
{
    xis >> xml::start( "parameters" )
        >> xml::attribute( "limit", limit_ )
        >> xml::attribute( "globalfactor", globalFactor_ )
        >> xml::attribute( "unit", unit_ )
        >> xml::attribute( "urban", urban_ )
        >> xml::attribute( "object", object_ )
        >> xml::attribute( "terrain", terrain_ )
        >> xml::attribute( "knowledge", knowledge_ )
        >> xml::attribute( "unitknowledge", unitknowledge_ )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PerformanceIndicator::ComputeLoadIndicator
// Created: MMC 2012-02-02
// -----------------------------------------------------------------------------
float PerformanceIndicator::ComputeLoadIndicator()
{
    PerformanceIndicator::IndicatorValues values;
    return ComputeLoadIndicator( values );
}

// -----------------------------------------------------------------------------
// Name: PerformanceIndicator::ComputeLoadIndicator
// Created: MMC 2012-02-02
// -----------------------------------------------------------------------------
float PerformanceIndicator::ComputeLoadIndicator( PerformanceIndicator::IndicatorValues& values )
{
    values.limit_ = static_cast< unsigned int >( limit_ );
    values.units_ = static_cast< tools::Resolver< kernel::Agent_ABC >* >( &model_.agents_ )->Count();
    values.blocs_ = static_cast< tools::Resolver< gui::TerrainObjectProxy >* >( &model_.urban_ )->Count();
    values.objects_ = model_.objects_.Count();
    values.terrainLoad_ = static_cast< unsigned int >( model_.urban_.GetWorldSize() / 1e6 );
    values.knowledges_ = model_.knowledgeGroups_.Count();

    float units = static_cast< float >( values.units_ );
    float blocs = static_cast< float >( values.blocs_ );
    float objects = static_cast< float >( values.objects_ );
    float terrain = static_cast< float >( values.terrainLoad_ );
    float knowledges = static_cast< float >( values.knowledges_ );
    if ( knowledges < 1.f )
        knowledges = 1.f;

    float load  = units * ( unit_ + blocs * urban_ ) + objects * object_ + terrain * terrain_ + knowledge_ * ( 1.f - unitknowledge_ * units / knowledges );
    load *= globalFactor_;
    if ( load > limit_ )
        load = limit_;

    return load;
}

// -----------------------------------------------------------------------------
// Name: PerformanceIndicator::IsLoaded
// Created: MMC 2012-02-02
// -----------------------------------------------------------------------------
bool PerformanceIndicator::IsLoaded()
{
    return loaded_;
}
