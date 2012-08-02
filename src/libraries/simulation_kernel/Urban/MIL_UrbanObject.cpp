// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_UrbanObject.h"
#include "UrbanColorAttribute.h"
#include "UrbanGeometryAttribute.h"
#include "UrbanPhysicalAttribute.h"
#include "UrbanResourceNetworkAttribute.h"
#include "MIL_UrbanObjectVisitor_ABC.h"
#include <boost/lexical_cast.hpp>

using namespace urban;

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject constructor
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
MIL_UrbanObject::MIL_UrbanObject( xml::xistream& xis, urban::CoordinateConverter_ABC& converter, MIL_UrbanObject_ABC* parent /*= 0*/ )
    : nUrbanId_ ( xis.attribute< unsigned long >( "id" ) )
    , name_     ( xis.attribute< std::string >( "name" ) )
    , converter_( converter )
    , parent_   ( parent )
{
    if( name_.empty() )
        name_ = boost::lexical_cast< std::string >( nUrbanId_ );

    // TEMP
    // TODO refaire au propre quand on n'aura plus les attributs dans urban et qu'on aura viré le wrapper
    // Passer par l'attribute factory? 
    Attach( *new UrbanGeometryAttribute( xis, converter_ ) );
    Attach( *new UrbanColorAttribute( xis ) );
    if( xis.has_child( "physical" ) || ( parent && parent->GetParent() ) )
        Attach( *new UrbanPhysicalAttribute( xis ) );

    xis >> xml::optional >> xml::start( "infrastructures" )
            >> xml::optional >> xml::start( "infrastructure" )
                >> xml::attribute( "type", infrastructure_ )
            >>xml::end
        >>xml::end;
    if( xis.has_child( "resources") )
        Attach( *new UrbanResourceNetworkAttribute( xis ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject destructor
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
MIL_UrbanObject::~MIL_UrbanObject()
{
    DestroyExtensions();
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::GetUrbanId
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
unsigned long MIL_UrbanObject::GetUrbanId() const
{
    return nUrbanId_;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::GetName
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
const std::string& MIL_UrbanObject::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::GetParent
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
MIL_UrbanObject_ABC* MIL_UrbanObject::GetParent() const
{
    return parent_;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::Accept
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
void MIL_UrbanObject::Accept( MIL_UrbanObjectVisitor_ABC& visitor ) const
{
    tools::Iterator< const MIL_UrbanObject_ABC& > it = CreateIterator();
    while( it.HasMoreElements() )
        it.NextElement().Accept( visitor );
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::Accept
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
void MIL_UrbanObject::Accept( MIL_UrbanMotivationsVisitor_ABC& visitor ) const
{
    const UrbanPhysicalAttribute* pPhysical = Retrieve< UrbanPhysicalAttribute >();
    if( pPhysical )
        pPhysical->Accept( visitor );
}

namespace
{
    template< typename IT >
    bool FindOuterPoint( IT begin, IT end, const geometry::Point2f& from, const geometry::Vector2f& direction, geometry::Point2f& worst )
    {
        bool bFound = false;
        float rMaxProjection = 0;
        for( IT it = begin; it != end; ++it )
        {
            const geometry::Vector2f v( from, *it );
            const float rProjection = direction.CrossProduct( v );
            if( rProjection < -1.f ) // epsilon
            {
                bFound = true;
                if( rMaxProjection > rProjection )
                {
                    rMaxProjection = rProjection;
                    worst = *it;
                }
            }
        }
        return bFound;
    }
    
    typedef std::vector< geometry::Point2f > T_PointVector;
    typedef T_PointVector::iterator         IT_PointVector;
    typedef T_PointVector::const_iterator  CIT_PointVector;

    void ComputeHull( const T_PointVector& vertices, T_PointVector& hull )
    {
        if( !vertices.empty() )
        {
            geometry::Point2f maxLeft = *vertices.begin();
            geometry::Point2f maxRight = maxLeft;

            for( CIT_PointVector it = vertices.begin(); it != vertices.end(); ++it )
            {
                if( it->X() < maxLeft.X() )
                    maxLeft = *it;
                if( it->X() > maxRight.X() )
                    maxRight = *it;
            }
            hull.push_back( maxLeft );
            hull.push_back( maxRight );
            unsigned int nPoint = 0;
            while( nPoint != hull.size() )
            {
                unsigned int nFollowingPoint = ( nPoint + 1 ) % hull.size();
                geometry::Vector2f direction( hull[ nPoint ], hull[ nFollowingPoint ] );
                direction.Normalize();
                geometry::Point2f worst;
                if( FindOuterPoint( vertices.begin(), vertices.end(), hull[ nPoint ], direction, worst ) )
                {
                    hull.insert( hull.begin() + nFollowingPoint, worst );
                    nPoint = 0;
                }
                else
                    ++nPoint;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::ComputeConvexHull
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
void MIL_UrbanObject::ComputeConvexHull()
{
    T_PointVector vertices;
    tools::Iterator< const MIL_UrbanObject_ABC& > it = CreateIterator();
    while( it.HasMoreElements() )
    {
        const MIL_UrbanObject_ABC& object = it.NextElement();
        if( const UrbanGeometryAttribute* attribute = object.Retrieve< UrbanGeometryAttribute >() )
        {
            const std::vector< geometry::Point2f >& objectVertices = attribute->Vertices();
            vertices.insert( vertices.end(), objectVertices.begin(), objectVertices.end() );
        }
    }
    T_PointVector hull;
    ComputeHull( vertices, hull );
    Get< UrbanGeometryAttribute >().SetGeometry( hull );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::GetUrbanObjectLeaves
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
void MIL_UrbanObject::GetUrbanObjectLeaves( std::vector< const MIL_UrbanObject_ABC* >& leaves ) const
{
    tools::Iterator< const MIL_UrbanObject_ABC& > it = CreateIterator();
    if( !it.HasMoreElements()  )
        leaves.push_back( this );
    else
        while( it.HasMoreElements() )
            it.NextElement().GetUrbanObjectLeaves( leaves );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::HasChild
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
bool MIL_UrbanObject::HasChild() const
{
    return Count() > 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::GetLivingSpace
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
float MIL_UrbanObject::GetLivingSpace() const
{
    if( const UrbanGeometryAttribute* geometry = Retrieve< UrbanGeometryAttribute>() )
    {
        const UrbanPhysicalAttribute* physical = Retrieve< UrbanPhysicalAttribute >();
        double factor = physical ? ( physical->GetArchitecture().floorNumber_ + 1 ) * physical->GetArchitecture().occupation_ : 1.;
        return geometry->ComputeArea() * static_cast< float >( factor );
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::ComputeComplexity
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
float MIL_UrbanObject::ComputeComplexity() const
{
    // $$$$ _RC_ JSR 2010-09-16: A refaire (voir PHY_RolePion_UrbanLocation::Execute dans la sim)
    const UrbanGeometryAttribute* geometry = Retrieve< UrbanGeometryAttribute>();
    double complexity = geometry ? geometry->ComputeArea() * 0.1f : 1.f;
    const UrbanPhysicalAttribute* physical = Retrieve< UrbanPhysicalAttribute >();
    if( physical )
        complexity *= ( ( 1 + physical->GetArchitecture().floorNumber_ ) * physical->GetArchitecture().occupation_ );
    return static_cast< float >( complexity );
}

// -----------------------------------------------------------------------------
// Name: MIL_UrbanObject::GetInfrastructure
// Created: JSR 2012-08-01
// -----------------------------------------------------------------------------
const std::string& MIL_UrbanObject::GetInfrastructure() const
{
    return infrastructure_;
}
