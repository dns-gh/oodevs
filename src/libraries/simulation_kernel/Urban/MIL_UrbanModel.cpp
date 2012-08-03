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
#include "Urban/MIL_UrbanObject.h"
#include "UrbanGeometryAttribute.h"
#include "UrbanPhysicalAttribute.h"
#include "Tools/ExerciseConfig.h"
#pragma warning( push, 0 )
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#pragma warning( pop )

namespace bfs = boost::filesystem;

// TODO tout passer en double

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
    int CompareOnX( float value, const MIL_UrbanObject_ABC* key ) const
    {
        if( const UrbanGeometryAttribute* geom = key->Retrieve< UrbanGeometryAttribute >() )
            return Intersector( geom->BoundingBox() ).CompareOnX( value );
        return 0;
    }

    int CompareOnY( float value, const MIL_UrbanObject_ABC* key ) const
    {
        if( const UrbanGeometryAttribute* geom = key->Retrieve< UrbanGeometryAttribute >() )
            return Intersector( geom->BoundingBox() ).CompareOnY( value );
        return 0;
    }
};

// -----------------------------------------------------------------------------
// Name: MIL_UrbanModel constructor
// Created: JSR 2012-07-26
// -----------------------------------------------------------------------------
MIL_UrbanModel::MIL_UrbanModel()
    : precision_     ( 0 )
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

        bool operator()( const MIL_UrbanObject_ABC* key )
        {
            if( key && key->Retrieve< UrbanGeometryAttribute >() && wrapper_.Check( key->Get< UrbanGeometryAttribute >().Vertices() ) )
            {
                data_.push_back( key );
                return wrapper_.DoContinue();
            }
            return true;
        }

        std::vector< const MIL_UrbanObject_ABC* > data_;
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
        bool Check( const std::vector< geometry::Point2f >& geometry ) const
        {
            geometry::Polygon2f p( geometry );
            return p.IsInside( pick_ );
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
        bool Check( const std::vector< geometry::Point2f >& geometry ) const
        {
            geometry::Polygon2f p( geometry );
            return p.Intersect( center_, radius_ );
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
        bool Check( const std::vector< geometry::Point2f >& geometry ) const
        {
            geometry::Polygon2f p( geometry );
            return !p.Intersect( segment_ ).empty();
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

        bool operator()( const MIL_UrbanObject_ABC* key )
        {
            if( key && key->Retrieve< UrbanGeometryAttribute >() && wrapper_.Check( key->Get< UrbanGeometryAttribute >().Vertices() ) )
            {
                double tempCost = 0.;
                const UrbanPhysicalAttribute* pPhysical = key->Retrieve< UrbanPhysicalAttribute >();
                if( pPhysical )
                    tempCost = pPhysical->GetPathfindCost( weight_ );
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
void MIL_UrbanModel::GetListWithinCircle( const MT_Vector2D& center, float radius, std::vector< const MIL_UrbanObject_ABC* >& result ) const
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
void MIL_UrbanModel::GetListWithinSegment( const MT_Vector2D& start, const MT_Vector2D& end, std::vector< const MIL_UrbanObject_ABC* >& result ) const
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
const MIL_UrbanObject_ABC* MIL_UrbanModel::FindBlock( const MT_Vector2D& point ) const
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
const std::vector< const MIL_UrbanObject_ABC* >& MIL_UrbanModel::GetCities() const
{
    return cities_;
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
    const MIL_UrbanObject_ABC* GetTerrainObject( const tools::Resolver< MIL_UrbanObject_ABC >& resolver, unsigned int id )
    {
        tools::Iterator< const MIL_UrbanObject_ABC& > it = resolver.CreateIterator();
        while( it.HasMoreElements() )
        {
            const MIL_UrbanObject_ABC* object = &it.NextElement();
            if( object->GetUrbanId() == id )
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
const MIL_UrbanObject_ABC* MIL_UrbanModel::GetTerrainObject( unsigned int id ) const
{
    return ::GetTerrainObject( *this, id );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanModel::Accept
// Created: JSR 2012-07-27
// -----------------------------------------------------------------------------
void MIL_UrbanModel::Accept( MIL_UrbanObjectVisitor_ABC& visitor ) const
{
    tools::Iterator< const MIL_UrbanObject_ABC& > it = CreateIterator();
    while( it.HasMoreElements() )
        it.NextElement().Accept( visitor );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanModel::Load
// Created: JSR 2012-07-27
// -----------------------------------------------------------------------------
void MIL_UrbanModel::Load( const std::string& directoryPath, tools::ExerciseConfig& config )
{
    // TODO Supprimer le chemin en hard
    const bfs::path fullPath = bfs::path( directoryPath ) / "urban" / "urban.xml"; 
    Purge();
    if( bfs::exists( fullPath ) ) // avoid exception
    {
        xml::xifstream input( fullPath.string() );
        Load( input );
        CreateQuadTree( geometry::Rectangle2f( geometry::Point2f( 0, 0 ), geometry::Point2f( config.GetTerrainWidth(), config.GetTerrainHeight() ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanModel::ReadCity
// Created: JSR 2012-07-27
// -----------------------------------------------------------------------------
void MIL_UrbanModel::ReadCity( xml::xistream& xis )
{
    MIL_UrbanObject_ABC& urbanObject = *new MIL_UrbanObject( xis );
    cities_.push_back( &urbanObject );
    tools::Resolver< MIL_UrbanObject_ABC >::Register( urbanObject.GetUrbanId(), urbanObject );
    xis >> xml::optional >> xml::start( "urban-objects" )
            >> xml::list( "urban-object", *this, &MIL_UrbanModel::ReadItem, urbanObject )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanModel::ReadItem
// Created: JSR 2012-07-27
// -----------------------------------------------------------------------------
void MIL_UrbanModel::ReadItem( xml::xistream& xis, MIL_UrbanObject_ABC& parent )
{
    MIL_UrbanObject_ABC& urbanObject = *new MIL_UrbanObject( xis, &parent );
    static_cast< tools::Resolver< MIL_UrbanObject_ABC >& >( parent ).Register( urbanObject.GetUrbanId(), urbanObject );
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
    tools::Iterator< const MIL_UrbanObject_ABC& > cities = CreateIterator();
    while( cities.HasMoreElements() )
    {
        MIL_UrbanObject_ABC& city = const_cast< MIL_UrbanObject_ABC& >( cities.NextElement() );
        tools::Iterator< const MIL_UrbanObject_ABC& > districts = city.CreateIterator();
        while( districts.HasMoreElements() )
        {
            MIL_UrbanObject_ABC& district = const_cast< MIL_UrbanObject_ABC& >( districts.NextElement() );
            district.ComputeConvexHull();
        }
        city.ComputeConvexHull();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanModel::CreateQuadTree
// Created: JSR 2012-07-27
// -----------------------------------------------------------------------------
void MIL_UrbanModel::CreateQuadTree( const geometry::Rectangle2f& rect )
{
    quadTree_.reset( new T_QuadTree( rect ) );
    quadTree_->SetRefinementPolicy( 30 );
    std::vector< const MIL_UrbanObject_ABC* > objects;
    tools::Iterator< const MIL_UrbanObject_ABC& > it = CreateIterator();
    // $$$$ _RC_ JSR 2010-09-17: TODO Optimiser le quadtree en utilisant aussi les villes et quartiers pour accélérer la recherche

    precision_ = sqrt( ( rect.Right() - rect.Left() ) * ( rect.Top() - rect.Bottom() ) * 1e-16f );
    maxElementSize_ = 0;
    while( it.HasMoreElements() )
        it.NextElement().GetUrbanObjectLeaves( objects );
    for( std::vector< const MIL_UrbanObject_ABC* >::const_iterator it = objects.begin(); it != objects.end(); ++it )
    {
        const UrbanGeometryAttribute* pAttribute = ( *it )->Retrieve< UrbanGeometryAttribute >();
        if( pAttribute )
        {
            quadTree_->Insert( *it );
            geometry::Rectangle2f boundingBox = pAttribute->BoundingBox();
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
