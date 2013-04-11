// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "geostore_pch.h"
#include "Geostore.h"
#include "Database.h"
#include "CreateBlockAutoProcess.h"
#include "CreateBlockProcess.h"
#include <terrain/PointProjector.h>
#include <boost/bind.hpp>

using namespace geostore;

namespace geostore
{
    class Loader : boost::noncopyable
    {
    public:
        Loader( const tools::Path& terrain )
            : file_     ( terrain / "Graphics/geostore.sqlite" )
            , temporary_( file_ )
        {}
        ~Loader()
        {
            try
            {
                temporary_.Rename( file_ );
            }
            catch( ... )
            {}
        }
        Database* Load( const tools::Path& layers, PointProjector_ABC& projector )
        {
            try
            {
                return new Database( temporary_, layers, projector );
            }
            catch( ... )
            {
                temporary_ = tools::Path::TemporaryFile();
                return new Database( temporary_, layers, projector );
            }
        }
    private:
        const tools::Path file_;
        tools::Path temporary_;
    };
}

Geostore::Geostore( const tools::Path& terrain, const SpatialIndexer& index )
    : index_    ( index )
    , loader_   ( new Loader( terrain ) )
    , projector_( new PointProjector( terrain ) )
    , database_ ( loader_->Load( terrain / "Graphics", *projector_ ) )
{
    // NOTHING
}

Geostore::~Geostore()
{
    // NOTHING
}

const Database& Geostore::GetDatabase() const
{
    return *database_;
}

void Geostore::CreateUrbanBlocksOnCities( const geometry::Polygon2f& footprint, double roadWidth, std::vector< geometry::Polygon2f >& urbanBlocks )
{
    try
    {
        CreateBlockAutoProcess process( database_->GetTables(), index_, *projector_, roadWidth );
        process.Run( footprint, urbanBlocks );
    }
    catch( ... ) // Created: AME 2010-08-02 Improve exception catching
    {
        // ?????
    }
}

std::vector< const kernel::UrbanObject_ABC* > Geostore::IntersectedBlocks( const geometry::Polygon2f& footprint )
{
    CreateBlockProcess process;
    return process.IntersectedBlocks( index_, footprint, *projector_ );
}
