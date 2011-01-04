// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "InhabitantPositions.h"
#include "LocationSerializer.h"
#include "UrbanModel.h"
#include "clients_kernel/Circle.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Lines.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/InhabitantType.h"
#include "clients_kernel/Point.h"
#include "clients_kernel/Polygon.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "tools/Resolver.h"
#include "clients_gui/TerrainObjectProxy.h"
#include <xeumeuleu/xml.hpp>

namespace
{
    class IntersectionVisitor : public kernel::LocationVisitor_ABC
    {
    public:
        IntersectionVisitor( const UrbanModel& model )
            : model_( model )
        {}
        virtual ~IntersectionVisitor()
        {}
        virtual void VisitPolygon( const T_PointVector& points )
        {
            tools::Iterator< const gui::TerrainObjectProxy& > it = model_.tools::Resolver< gui::TerrainObjectProxy >::CreateIterator();
            while( it.HasMoreElements() )
            {
                const gui::TerrainObjectProxy& object = it.NextElement();
                geometry::Polygon2f poly( points );
                if( poly.IsInside( object.Barycenter() ) )
                    vect_.push_back( &object );
            }
        }

        virtual void VisitLines     ( const T_PointVector& ) {}
        virtual void VisitRectangle ( const T_PointVector& ) {}
        virtual void VisitCircle    ( const geometry::Point2f& , float  ) {}
        virtual void VisitPoint     ( const geometry::Point2f&  ) {}
        virtual void VisitPath      ( const geometry::Point2f& , const T_PointVector&  ) {}
        std::vector< const gui::TerrainObjectProxy* > GetVect(){ return vect_; }

    private :
        IntersectionVisitor( const IntersectionVisitor& );            //!< Copy constructor
        IntersectionVisitor& operator=( const IntersectionVisitor& ); //!< Assignment operator

    private :
        const UrbanModel& model_;
        std::vector< const gui::TerrainObjectProxy* > vect_;
    };
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions constructor
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
InhabitantPositions::InhabitantPositions( const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location, UrbanModel& urbanModel )
    : converter_ ( converter )
    , urbanModel_( urbanModel )
{
    IntersectionVisitor visitor( urbanModel_ );
    location.Accept( visitor );
    livingUrbanObject_ = visitor.GetVect();
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions constructor
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
InhabitantPositions::InhabitantPositions( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter, UrbanModel& urbanModel )
    : converter_ ( converter )
    , urbanModel_( urbanModel )
{
    ReadLocation( xis );
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions destructor
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
InhabitantPositions::~InhabitantPositions()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::ReadLocation
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
void InhabitantPositions::ReadLocation( xml::xistream& xis )
{
    xis >> xml::start( "living-area" )
        >> xml::list( "urban-block" , *this, &InhabitantPositions::ReadLivingUrbanBlock )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::ReadLivingUrbanBlock
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
void InhabitantPositions::ReadLivingUrbanBlock( xml::xistream& xis )
{
    unsigned long id;
    xis >> xml::attribute( "id", id );
    gui::TerrainObjectProxy* pObject = urbanModel_.tools::Resolver< gui::TerrainObjectProxy >::Find( id );
    if( !pObject )
        xis.error( "error in loading living urban block for population" );
    else
        livingUrbanObject_.push_back( pObject );
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::SerializeAttributes
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
void InhabitantPositions::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "living-area" );
    for( CIT_UrbanObjectVector it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); ++it )
    {
        xos << xml::start( "urban-block" )
                << xml::attribute( "id", ( *it )->GetId() )
            << xml::end;
    }
       //TODO
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::GetPosition
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
geometry::Point2f InhabitantPositions::GetPosition( bool /*aggregated*/ ) const
{
    geometry::Polygon2f poly;
    for( CIT_UrbanObjectVector it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); ++it )
        poly.Add( (*it)->Barycenter() );
    return poly.Barycenter();
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::GetHeight
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
float InhabitantPositions::GetHeight( bool /*aggregated*/ ) const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::IsAt
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
bool InhabitantPositions::IsAt( const geometry::Point2f& /*pos*/, float /*precision*/, float /*adaptiveFactor*/ ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::IsIn
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
bool InhabitantPositions::IsIn( const geometry::Rectangle2f& /*rectangle*/ ) const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::GetBoundingBox
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
geometry::Rectangle2f InhabitantPositions::GetBoundingBox() const
{
    return geometry::Rectangle2f();
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::Accept
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
void InhabitantPositions::Accept( kernel::LocationVisitor_ABC& /*visitor*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::CanAggregate
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
bool InhabitantPositions::CanAggregate() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::InhabitantPositions::DrawPositions
// Created: SLG 2010-11-26
// -----------------------------------------------------------------------------
void InhabitantPositions::Draw( const kernel::GlTools_ABC& tools ) const
{
    for( CIT_UrbanObjectVector it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); ++it )
    {
        const gui::TerrainObjectProxy& object = **it;
        const geometry::Polygon2f* footprint = object.GetFootprint();
        if( footprint )
            tools.DrawConvexPolygon( footprint->Vertices() );
    }
}