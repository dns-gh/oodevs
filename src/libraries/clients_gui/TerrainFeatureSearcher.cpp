// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "TerrainFeatureSearcher.h"
#include "View_ABC.h"
#include "clients_kernel/ModelLoaded.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ApproximativeMap.h"
#include "tools/WorldParameters.h"
#include <graphics/NoVBOShapeLayer.h>
#include <graphics/GraphicSetup_ABC.h>
#pragma warning( disable : 4355 4512 )
#include <boost/filesystem/operations.hpp>
#include <boost/algorithm/string.hpp>

using namespace gui;
namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: TerrainFeatureSearcher constructor
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
TerrainFeatureSearcher::TerrainFeatureSearcher( kernel::Controllers& controllers )
    : controllers_( controllers )
    , nameLocations_( new T_NameLocations( 500 ) ) // threshold...
    , current_( 0 )
    , index_  ( 0 )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: TerrainFeatureSearcher destructor
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
TerrainFeatureSearcher::~TerrainFeatureSearcher()
{
    controllers_.Unregister( *this );
}

namespace
{
    std::string Cleanup( std::string name )
    {
        boost::algorithm::to_lower( name );
        // $$$$ AGE 2008-05-28:
        boost::algorithm::erase_all( name, "rivi�re" );
        boost::algorithm::erase_all( name, "riviere" );
        boost::algorithm::erase_all( name, "ruisseau" );
        boost::algorithm::erase_all( name, " de " );
        boost::algorithm::trim( name );
        return name;
    }

    struct NameShapeLayer : public GraphicSetup_ABC, public NoVBOShapeLayer
    {
        NameShapeLayer( const std::string& filename )
            : NoVBOShapeLayer( *this, filename )
        {}
        virtual void DrawName( const geometry::Point2f& at, const std::string& name )
        {
            names_[ Cleanup( name ) ].push_back( at );
        }

        virtual bool ShouldDisplay      ( const TerrainData& , const geometry::Rectangle2f& ) { return false; }
        virtual bool ShouldDisplayBorder( const TerrainData& , const geometry::Rectangle2f& ) { return false; }
        virtual bool ShouldDisplayNames ( const TerrainData& , const geometry::Rectangle2f& ) { return true; }
        virtual void SetupLineGraphics  ( const Data_ABC* ) {}
        virtual void SetupLineGraphics  ( unsigned int ) {}
        virtual void SetupBorderGraphics( const Data_ABC* ) {}
        virtual void SetupAreaGraphics  ( const Data_ABC* ) {}
        std::map< std::string, T_PointVector > names_;
    };
}

// -----------------------------------------------------------------------------
// Name: TerrainFeatureSearcher::Search
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
bool TerrainFeatureSearcher::Search( const QString& name, geometry::Point2f& point, QString& hint )
{
    if( !pendingSourceFile_.empty() )
        LoadFeatures();
    const T_Feature* feature = nameLocations_->Find( Cleanup( name.ascii() ) );
    if( feature != current_ )
        index_ = 0;
    current_ = feature;
    if( current_ && current_->second.size() > index_ )
    {
        point = current_->second.at( index_ );
        hint  = current_->first;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: TerrainFeatureSearcher::FindNext
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
bool TerrainFeatureSearcher::FindNext( geometry::Point2f& point, QString& hint )
{
    if( !current_ || index_ == ( current_->second.size() - 1 ) )
        index_ = 0;
    else
        ++index_;
    if( current_ )
    {
        index_ = index_ % current_->second.size();
        point = current_->second.at( index_ );
        hint = current_->first;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: TerrainFeatureSearcher::NotifyUpdated
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
void TerrainFeatureSearcher::NotifyUpdated( const kernel::ModelLoaded& model )
{
    current_ = 0;
    index_ = 0;
    nameLocations_.reset( new T_NameLocations( 500 ) );

    tools::WorldParameters parameters( model.config_ );
    if( ! parameters.graphicsDirectory_.empty() )
    {
        const bfs::path dump = bfs::path( parameters.graphicsDirectory_, bfs::native ) / "shapes.dump";
        if( bfs::exists( dump ) )
            pendingSourceFile_ = dump.native_file_string();
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainFeatureSearcher::LoadFeatures
// Created: SBO 2008-08-07
// -----------------------------------------------------------------------------
void TerrainFeatureSearcher::LoadFeatures()
{
    NameShapeLayer layer( pendingSourceFile_ );
    const float inf = std::numeric_limits< float >::infinity();
    const geometry::Rectangle2f universe( -inf, -inf, inf, inf );
    layer.Initialize( universe );
    layer.Paint( universe );
    for( std::map< std::string, T_PointVector >::const_iterator it = layer.names_.begin(); it != layer.names_.end(); ++it )
            nameLocations_->Add( it->first, T_Feature( it->first.c_str(), it->second ) );
    pendingSourceFile_ = "";
}
