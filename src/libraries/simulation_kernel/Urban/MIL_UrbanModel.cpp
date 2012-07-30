// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_UrbanModel.h"
#include "MT_Tools/MT_Vector2D.h"

#include <urban/WorldParameters.h> // à remplacer par config
#include <urban/CoordinateConverter_ABC.h>
#include <urban/GeometryAttribute.h>
#include <urban/PhysicalAttribute.h>
#include <urban/TerrainObject_ABC.h>
#include <urban/CoordinateConverter.h>
#include <urban/UrbanObject.h>

#pragma warning( push, 0 )
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#pragma warning( pop )

namespace bfs = boost::filesystem;

#define VECTOR_TO_POINT( point ) geometry::Point2f( static_cast< float >( ( point ).rX_ ), static_cast< float >( ( point ).rY_ ) )

namespace
{
    spatialcontainer::SegmentIntersecter< float > Intersector( const geometry::Rectangle2f& boundingBox )
    {
        return spatialcontainer::SegmentIntersecter< float >( boundingBox.BottomLeft(), boundingBox.TopRight() );
    };
}

struct MIL_UrbanModel::QuadTreeTraits
{
    int CompareOnX( float value, const urban::TerrainObject_ABC* key ) const
    {
        if( const urban::GeometryAttribute* geom = key->Retrieve< urban::GeometryAttribute >() )
            return Intersector( geom->Geometry().BoundingBox() ).CompareOnX( value );
        return 0;
    }

    int CompareOnY( float value, const urban::TerrainObject_ABC* key ) const
    {
        if( const urban::GeometryAttribute* geom = key->Retrieve< urban::GeometryAttribute >() )
            return Intersector( geom->Geometry().BoundingBox() ).CompareOnY( value );
        return 0;
    }
};

// -----------------------------------------------------------------------------
// Name: MIL_UrbanModel constructor
// Created: JSR 2012-07-26
// -----------------------------------------------------------------------------
MIL_UrbanModel::MIL_UrbanModel()
    : converter_     ( new urban::CoordinateConverter() )
    , precision_     ( 0 )
    , maxElementSize_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanModel destructor
// Created: JSR 2012-07-26
// -----------------------------------------------------------------------------
MIL_UrbanModel::~MIL_UrbanModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanModel::Purge
// Created: JSR 2012-07-27
// -----------------------------------------------------------------------------
void MIL_UrbanModel::Purge()
{
    DeleteAll();
    quadTree_.reset();
    modelVersion_.clear();
}

namespace
{
    template< typename CollisionChecker >
    struct DataExtractor
    {
        explicit DataExtractor( const CollisionChecker& wrapper )
            : wrapper_( wrapper )
        {
            // NOTHING
        }

        bool operator()( const urban::TerrainObject_ABC* key )
        {
            if( key && key->Retrieve< urban::GeometryAttribute >() && wrapper_.Check( key->Get< urban::GeometryAttribute >().Geometry() ) )
            {
                data_.push_back( key );
                return wrapper_.DoContinue();
            }
            return true;
        }

        std::vector< const urban::TerrainObject_ABC* > data_;
        const CollisionChecker wrapper_;
    };

    class PointChecker
    {
    public:
        explicit PointChecker( const geometry::Point2f& pick )
            : pick_( pick )
        {
            // NOTHING
        }
        bool Check( const geometry::Polygon2f& geometry ) const
        {
            return geometry.IsInside( pick_ );
        }
        bool DoContinue() const
        {
            return false;
        }
    private:
        PointChecker& operator=( const PointChecker& );
    private:
        const geometry::Point2f& pick_;
    };

    class CircleChecker
    {
    public:
        CircleChecker( const geometry::Point2f& center, float radius )
            : center_( center )
            , radius_( radius )
        {
            // NOTHING
        }
        bool Check( const geometry::Polygon2f& geometry ) const
        {
            return geometry.Intersect( center_, radius_ );
        }
        bool DoContinue() const
        {
            return true;
        }
    private:
        CircleChecker& operator=( const CircleChecker& );
    private:
        const geometry::Point2f& center_;
        float radius_;
    };

    class SegmentChecker
    {
    public:
        explicit SegmentChecker( const geometry::Segment2f& segment )
            : segment_( segment )
        {
            // NOTHING
        }
        bool Check( const geometry::Polygon2f& geometry ) const
        {
            return !geometry.Intersect( segment_ ).empty();
        }
        bool DoContinue() const
        {
            return true;
        }
    private:
        SegmentChecker& operator=( const SegmentChecker& );
    private:
        const geometry::Segment2f& segment_;
    };

    template< typename CollisionChecker >
    struct CostExtractor
    {
        CostExtractor( const CollisionChecker& wrapper, float weight )
            : wrapper_( wrapper )
            , weight_ ( weight )
            , cost_   ( 0. )
        {
            // NOTHING
        }

        bool operator()( const urban::TerrainObject_ABC* key )
        {
            if( key && key->Retrieve< urban::GeometryAttribute >() && wrapper_.Check( key->Get< urban::GeometryAttribute >().Geometry() ) )
            {
                double tempCost = 0.;
                const urban::PhysicalAttribute* pPhysical = key->Retrieve< urban::PhysicalAttribute >();
                if( pPhysical && pPhysical->GetArchitecture() )
                    tempCost = pPhysical->GetArchitecture()->GetPathfindCost( weight_ );
                if( tempCost == -1. )
                {
                    cost_ = tempCost;
                    return false;
                }
                else
                    cost_ = std::max( cost_, tempCost );
                return wrapper_.DoContinue();
            }
            return true;
        }

        double weight_;
        double cost_;
        const CollisionChecker wrapper_;
    };
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanModel::GetListWithinCircle
// Created: JSR 2012-07-26
// -----------------------------------------------------------------------------
void MIL_UrbanModel::GetListWithinCircle( const MT_Vector2D& center, float radius, std::vector< const urban::TerrainObject_ABC* >& result ) const
{
    geometry::Point2f geomCenter( VECTOR_TO_POINT( center ) );
    if( quadTree_.get() )
    {
        spatialcontainer::SegmentIntersecter< float > intersecter( geometry::Point2f( geomCenter.X() - radius, geomCenter.Y() - radius ),
            geometry::Point2f( geomCenter.X() + radius, geomCenter.Y() + radius ), maxElementSize_ );
        CircleChecker checker( geomCenter, radius );
        DataExtractor< CircleChecker > extractor( checker );

        // std::cout << "Avg Depth:" << quadTree_->AverageDepth() << " "
        //           << "Size:" << quadTree_->Size() << " "
        //           << "Circle: " << radius;
        quadTree_->Apply( intersecter, extractor );
        std::swap( extractor.data_, result );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanModel::GetListWithinSegment
// Created: JSR 2012-07-26
// -----------------------------------------------------------------------------
void MIL_UrbanModel::GetListWithinSegment( const MT_Vector2D& start, const MT_Vector2D& end, std::vector< const urban::TerrainObject_ABC* >& result ) const
{
    geometry::Point2f geomStart( VECTOR_TO_POINT( start ) );
    geometry::Point2f geomEnd( VECTOR_TO_POINT( end ) );
    if( quadTree_.get() )
    {
        spatialcontainer::SegmentIntersecter< float > intersecter( geomStart, geomEnd, maxElementSize_ );
        geometry::Segment2f segment( geomStart, geomEnd );
        SegmentChecker checker( segment );
        DataExtractor< SegmentChecker > extractor( checker );

        quadTree_->Apply( intersecter, extractor );
        std::swap( extractor.data_, result );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanModel::FindBlock
// Created: JSR 2012-07-26
// -----------------------------------------------------------------------------
const urban::TerrainObject_ABC* MIL_UrbanModel::FindBlock( const MT_Vector2D& point ) const
{
    geometry::Point2f geomPoint( VECTOR_TO_POINT( point ) );
    if( quadTree_.get() )
    {
        spatialcontainer::PointIntersecter< float > intersecter( geomPoint, precision_, maxElementSize_ );
        PointChecker check( geomPoint );
        DataExtractor< PointChecker > extractor( check );

        quadTree_->Apply( intersecter, extractor );
        if( !extractor.data_.empty() )
            return *extractor.data_.begin();
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanModel::GetCities
// Created: JSR 2012-07-26
// -----------------------------------------------------------------------------
std::vector< const urban::TerrainObject_ABC* > MIL_UrbanModel::GetCities() const
{
    // $$$$ _RC_ JSR 2010-09-17: Non utilisé et pas propre (copie de vector). A virer si inutile
    std::vector< const urban::TerrainObject_ABC* > cities;
    tools::Iterator< const urban::TerrainObject_ABC& > it = CreateIterator();
    while( it.HasMoreElements() )
        cities.push_back( &it.NextElement() );
    return cities;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanModel::GetUrbanBlockCost
// Created: JSR 2012-07-26
// -----------------------------------------------------------------------------
float MIL_UrbanModel::GetUrbanBlockCost( float weight, const MT_Vector2D& from, const MT_Vector2D& to ) const
{
    geometry::Point2f geomFrom( VECTOR_TO_POINT( from ) );
    geometry::Point2f geomTo( VECTOR_TO_POINT( to ) );
    if( quadTree_.get() )
    {
        spatialcontainer::SegmentIntersecter< float > intersecter( geomFrom, geomTo, maxElementSize_ );
        geometry::Segment2f segment( geomFrom, geomTo );
        SegmentChecker checker( segment );
        CostExtractor< SegmentChecker > extractor( checker, weight );

        quadTree_->Apply( intersecter, extractor );
        return static_cast< float >( extractor.cost_ );
    }
    return 0;
}

namespace
{
    const urban::TerrainObject_ABC* GetTerrainObject( const tools::Resolver< urban::TerrainObject_ABC >& resolver, unsigned int id )
    {
        tools::Iterator< const urban::TerrainObject_ABC& > it = resolver.CreateIterator();
        while( it.HasMoreElements() )
        {
            const urban::TerrainObject_ABC* object = &it.NextElement();
            if( object->GetId() == id )
                return object;
            object = GetTerrainObject( *object, id );
            if( object )
                return object;
        }
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanModel::GetTerrainObject
// Created: JSR 2012-07-26
// -----------------------------------------------------------------------------
const urban::TerrainObject_ABC* MIL_UrbanModel::GetTerrainObject( unsigned int id ) const
{
    return ::GetTerrainObject( *this, id );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanModel::Accept
// Created: JSR 2012-07-27
// -----------------------------------------------------------------------------
void MIL_UrbanModel::Accept( urban::ObjectVisitor_ABC& visitor )
{
    tools::Iterator< const urban::TerrainObject_ABC& > it = CreateIterator();
    while( it.HasMoreElements() )
        it.NextElement().Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanModel::Load
// Created: JSR 2012-07-27
// -----------------------------------------------------------------------------
void MIL_UrbanModel::Load( const std::string& directoryPath )
{
    urban::WorldParameters world( directoryPath );
    converter_->Load( world );
    // TODO Supprimer le chemin en hard
    const bfs::path fullPath = bfs::path( directoryPath ) / "urban" / "urban.xml"; 
    Purge();
    if( bfs::exists( fullPath ) ) // avoid exception
    {
        xml::xifstream input( fullPath.string() );
        Load( input );
        CreateQuadTree( geometry::Rectangle2f( geometry::Point2f( 0, 0 ), geometry::Point2f( world.GetWidth(), world.GetHeight() ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanModel::ReadCity
// Created: JSR 2012-07-27
// -----------------------------------------------------------------------------
void MIL_UrbanModel::ReadCity( xml::xistream& xis )
{
    urban::TerrainObject_ABC& urbanObject = *new urban::UrbanObject( xis, 0, *converter_ );
    tools::Resolver< urban::TerrainObject_ABC >::Register( urbanObject.GetId(), urbanObject );
    xis >> xml::optional >> xml::start( "urban-objects" )
            >> xml::list( "urban-object", *this, &MIL_UrbanModel::ReadItem, urbanObject )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanModel::ReadItem
// Created: JSR 2012-07-27
// -----------------------------------------------------------------------------
void MIL_UrbanModel::ReadItem( xml::xistream& xis, urban::TerrainObject_ABC& parent )
{
    urban::TerrainObject_ABC& urbanObject = *new urban::UrbanObject( xis, &parent, *converter_ );
    parent.tools::Resolver< urban::TerrainObject_ABC >::Register( urbanObject.GetId(), urbanObject );
    parent.ComputeConvexHull();
    xis >> xml::optional >> xml::start( "urban-objects" )
            >> xml::list( "urban-object", *this, &MIL_UrbanModel::ReadItem, urbanObject )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanModel::Load
// Created: JSR 2012-07-27
// -----------------------------------------------------------------------------
void MIL_UrbanModel::Load( xml::xistream& xis )
{
    xis >> xml::start( "urban" );
    RetrieveVersion( xis );
    xis     >> xml::start( "urban-objects" )
                >> xml::list( "urban-object", *this, &MIL_UrbanModel::ReadCity )
            >> xml::end
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanModel::CreateQuadTree
// Created: JSR 2012-07-27
// -----------------------------------------------------------------------------
void MIL_UrbanModel::CreateQuadTree( const geometry::Rectangle2f& rect )
{
    quadTree_.reset( new T_QuadTree( rect ) );
    quadTree_->SetRefinementPolicy( 30 );
    std::vector< const urban::TerrainObject_ABC* > objects;
    tools::Iterator< const urban::TerrainObject_ABC& > it = CreateIterator();
    // $$$$ _RC_ JSR 2010-09-17: TODO Optimiser le quadtree en utilisant aussi les villes et quartiers pour accélérer la recherche

    precision_ = sqrt( ( rect.Right() - rect.Left() ) * ( rect.Top() - rect.Bottom() ) * 1e-16f );
    maxElementSize_ = 0;
    while( it.HasMoreElements() )
        it.NextElement().GetUrbanObjectLeaves( objects );
    for( std::vector< const urban::TerrainObject_ABC* >::const_iterator it = objects.begin(); it != objects.end(); ++it )
    {
        const urban::GeometryAttribute* pAttribute = ( *it )->Retrieve< urban::GeometryAttribute >();
        if( pAttribute )
        {
            quadTree_->Insert( *it );
            geometry::Rectangle2f boundingBox = pAttribute->Geometry().BoundingBox();
            float size = 1.1f * std::max( boundingBox.Width(), boundingBox.Height() );
            if( maxElementSize_ < size )
                maxElementSize_ = size;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanModel::RetrieveVersion
// Created: JSR 2012-07-27
// -----------------------------------------------------------------------------
void MIL_UrbanModel::RetrieveVersion( xml::xistream& xis )
{
    // TODO virer et utiliser le fileloader
    xis >> xml::optional >> xml::attribute( "model-version", modelVersion_ );
    if( modelVersion_.empty() )
        modelVersion_ = "4.2.3";
}
