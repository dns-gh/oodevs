//*****************************************************************************
// Created: LDC 2011-12-28
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_UrbanCache.h"
#include "Entities/MIL_EntityManager.h"
#include "MT_Tools/MT_Rect.h"
#include "MT_Tools/MT_Logger.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "Urban/UrbanPhysicalCapacity.h"

namespace
{
    spatialcontainer::SegmentIntersecter< double > Intersector( const MT_Rect& boundingBox )
    {
        return spatialcontainer::SegmentIntersecter< double >( geometry::Point2< double >( boundingBox.GetLeft(), boundingBox.GetBottom() )
            , geometry::Point2< double >( boundingBox.GetRight(), boundingBox.GetTop() ) );
    };
}

struct MIL_UrbanCache::QuadTreeTraits
{
    int CompareOnX( double value, const MIL_UrbanObject_ABC* key ) const
    {
        return Intersector( key->GetLocalisation().GetBoundingBox() ).CompareOnX( value );
    }

    int CompareOnY( double value, const MIL_UrbanObject_ABC* key ) const
    {
        return Intersector( key->GetLocalisation().GetBoundingBox() ).CompareOnY( value );
    }
};

// -----------------------------------------------------------------------------
// Name: MIL_UrbanCache constructor
// Created: LDC 2011-12-28
// -----------------------------------------------------------------------------
MIL_UrbanCache::MIL_UrbanCache()
    : precision_     ( 0 )
    , maxElementSize_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanCache destructor
// Created: JSR 2012-07-26
// -----------------------------------------------------------------------------
MIL_UrbanCache::~MIL_UrbanCache()
{
    quadTree_.reset();
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
            if( key && wrapper_.Check( key->GetLocalisation() ) )
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
        explicit PointChecker( const MT_Vector2D& pick )
            : pick_( pick )
        {
            // NOTHING
        }
        bool Check( const TER_Localisation& localisation ) const
        {
            return localisation.IsInside( pick_, 1e-8 );
        }
        bool DoContinue() const
        {
            return false;
        }
    private:
        PointChecker& operator=( const PointChecker& );
    private:
        const MT_Vector2D& pick_;
    };

    class CircleChecker
    {
    public:
        CircleChecker( const MT_Vector2D& center, double radius )
            : center_( center )
            , radius_( radius )
        {
            // NOTHING
        }
        bool Check( const TER_Localisation& localisation ) const
        {
            return localisation.Intersect2DWithCircle( center_, radius_ );
        }
        bool DoContinue() const
        {
            return true;
        }
    private:
        CircleChecker& operator=( const CircleChecker& );
    private:
        const MT_Vector2D& center_;
        double radius_;
    };

    class SegmentChecker
    {
    public:
        explicit SegmentChecker( const MT_Line& segment )
            : segment_( segment )
        {
            // NOTHING
        }
        bool Check( const TER_Localisation& localisation ) const
        {
            return localisation.Intersect2D( segment_, 1e-8 );
        }
        bool DoContinue() const
        {
            return true;
        }
    private:
        SegmentChecker& operator=( const SegmentChecker& );
    private:
        const MT_Line& segment_;
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
            if( key && wrapper_.Check( key->GetLocalisation() ) )
            {
                double tempCost = 0.;
                const UrbanPhysicalCapacity* pPhysical = key->Retrieve< UrbanPhysicalCapacity >();
                if( pPhysical )
                {
                    if( weight_ > pPhysical->GetTrafficability() )
                    {
                        cost_ = -1.f;
                        return false;
                    }
                    tempCost =  pPhysical->GetOccupation() / key->GetStructuralState();
                }
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
// Name: MIL_UrbanCache::CreateQuadTree
// Created: JSR 2012-08-03
// -----------------------------------------------------------------------------
void MIL_UrbanCache::CreateQuadTree( std::vector< const MIL_UrbanObject_ABC* >& cities, const geometry::Rectangle2d& rect )
{
    cities_ = cities;
    if( cities.empty() )
        return;
    quadTree_.reset( new T_QuadTree( rect ) );
    quadTree_->SetRefinementPolicy( 30 );
    precision_ = sqrt( ( rect.Right() - rect.Left() ) * ( rect.Top() - rect.Bottom() ) * 1e-16f );
    maxElementSize_ = 0;
    // $$$$ _RC_ JSR 2010-09-17: TODO Optimiser le quadtree en utilisant aussi les villes et quartiers pour accélérer la recherche
    for( std::vector< const MIL_UrbanObject_ABC* >::const_iterator it = cities.begin(); it != cities.end(); ++it )
        ( *it )->GetUrbanBlocks( urbanBlocks_ );
    for( std::vector< const MIL_UrbanObject_ABC* >::const_iterator it = urbanBlocks_.begin(); it != urbanBlocks_.end(); ++it )
    {
        const MT_Rect& boundingBox = ( *it )->GetLocalisation().GetBoundingBox();
        if( boundingBox.GetWidth() != 0 && boundingBox.GetHeight() != 0 )
        {
            quadTree_->Insert( *it );
            double size = 1.1 * std::max( boundingBox.GetWidth(), boundingBox.GetHeight() );
            if( maxElementSize_ < size )
                maxElementSize_ = size;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanCache::GetUrbanBlocksWithinSegment
// Created: LDC 2011-12-28
// -----------------------------------------------------------------------------
void MIL_UrbanCache::GetUrbanBlocksWithinSegment( const MT_Vector2D& vSourcePoint, const MT_Vector2D& vTargetPoint, std::vector< const MIL_UrbanObject_ABC* >& list )
{
    MT_Vector2D start;
    MT_Vector2D end;
    if( vSourcePoint < vTargetPoint )
    {
        start = vSourcePoint;
        end = vTargetPoint;
    }
    else
    {
        start = vTargetPoint;
        end = vSourcePoint;
    }
    T_Cache::const_iterator it = cache_.find( start );
    if( it != cache_.end() )
    {
        T_InnerCache::const_iterator inner = it->second.find( end );
        if( inner != it->second.end() )
        {
            list = inner->second;
            return;
        }
    }

    if( quadTree_.get() )
    {
        spatialcontainer::SegmentIntersecter< double > intersecter( geometry::Point2d( vSourcePoint.rX_, vSourcePoint.rY_ ),
                                                                    geometry::Point2d( vTargetPoint.rX_, vTargetPoint.rY_ ), maxElementSize_ );
        MT_Line segment( vSourcePoint, vTargetPoint );
        SegmentChecker checker( segment );
        DataExtractor< SegmentChecker > extractor( checker );

        quadTree_->Apply( intersecter, extractor );
        std::swap( extractor.data_, list );
    }

    cache_[ start ][ end ] = list;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanCache::Clear
// Created: LDC 2011-12-28
// -----------------------------------------------------------------------------
void MIL_UrbanCache::Clear()
{
    cache_.clear();
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanCache::GetListWithinCircle
// Created: LDC 2011-12-30
// -----------------------------------------------------------------------------
void MIL_UrbanCache::GetListWithinCircle( const MT_Vector2D& center, float radius, std::vector< const MIL_UrbanObject_ABC* >& result ) const
{
    std::vector< const MIL_UrbanObject_ABC* > tmpList;
    if( quadTree_.get() )
    {
        spatialcontainer::SegmentIntersecter< double > intersecter( geometry::Point2d( center.rX_ - radius, center.rY_ - radius ),
            geometry::Point2d( center.rX_ + radius, center.rY_ + radius ), maxElementSize_ );
        CircleChecker checker( center, radius );
        DataExtractor< CircleChecker > extractor( checker );
        quadTree_->Apply( intersecter, extractor );
        std::swap( extractor.data_, result );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanCache::FindBlock
// Created: JSR 2012-04-20
// -----------------------------------------------------------------------------
const MIL_UrbanObject_ABC* MIL_UrbanCache::FindBlock( const MT_Vector2D& point ) const
{
    if( quadTree_.get() )
    {
        spatialcontainer::PointIntersecter< double > intersecter( geometry::Point2d( point.rX_, point.rY_ ), precision_, maxElementSize_ );
        PointChecker check( point );
        DataExtractor< PointChecker > extractor( check );
        quadTree_->Apply( intersecter, extractor );
        if( !extractor.data_.empty() )
            return *extractor.data_.begin();
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanCache::GetCities
// Created: JSR 2012-04-20
// -----------------------------------------------------------------------------
const std::vector< const MIL_UrbanObject_ABC* >& MIL_UrbanCache::GetCities() const
{
    return cities_;
}

// -----------------------------------------------------------------------------
// Name: std::vector< const MIL_UrbanObject_ABC* >& MIL_UrbanCache::GetUrbanBlocks
// Created: JSR 2012-08-29
// -----------------------------------------------------------------------------
const std::vector< const MIL_UrbanObject_ABC* >& MIL_UrbanCache::GetUrbanBlocks() const
{
    return urbanBlocks_;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanCache::GetUrbanBlockCost
// Created: JSR 2012-04-23
// -----------------------------------------------------------------------------
double MIL_UrbanCache::GetUrbanBlockCost( float weight, const MT_Vector2D& from, const MT_Vector2D& to ) const
{
    if( !quadTree_.get() )
        return 0;
    const spatialcontainer::SegmentIntersecter< double > intersecter( geometry::Point2d( from.rX_, from.rY_ ),
                                                                      geometry::Point2d( to.rX_, to.rY_ ), maxElementSize_ );
    const MT_Line segment( from, to );
    const SegmentChecker checker( segment );
    CostExtractor< SegmentChecker > extractor( checker, weight );
    quadTree_->Apply( intersecter, extractor );
    return extractor.cost_;
}
