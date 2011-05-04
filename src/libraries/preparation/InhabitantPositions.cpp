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
#include "UrbanModel.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_gui/TerrainObjectProxy.h"
#include "Tools.h"
#include <xeumeuleu/xml.hpp>

namespace
{
    class IntersectionVisitor : public kernel::LocationVisitor_ABC
    {
    public:
        IntersectionVisitor( const UrbanModel& model, InhabitantPositions::T_UrbanObjectVector& vector )
            : model_ ( model )
            , vector_( vector )
        {
            // NOTHING
        }
        virtual ~IntersectionVisitor()
        {
            // NOTHING
        }
        virtual void VisitPolygon( const T_PointVector& points )
        {
            geometry::Polygon2f poly( points );
            tools::Iterator< const gui::TerrainObjectProxy& > it = model_.tools::Resolver< gui::TerrainObjectProxy >::CreateIterator();
            while( it.HasMoreElements() )
            {
                const gui::TerrainObjectProxy& object = it.NextElement();
                if( const kernel::UrbanPositions_ABC* positions = object.Retrieve< kernel::UrbanPositions_ABC >() )
                    if( poly.IsInside( positions->Barycenter() ) )
                        vector_.push_back( boost::make_tuple( object.GetId(), object.GetName(), &object ) );
            }
        }

        virtual void VisitLines     ( const T_PointVector& ) {}
        virtual void VisitRectangle ( const T_PointVector& ) {}
        virtual void VisitCircle    ( const geometry::Point2f& , float  ) {}
        virtual void VisitPoint     ( const geometry::Point2f&  ) {}
        virtual void VisitPath      ( const geometry::Point2f& , const T_PointVector&  ) {}

    private :
        IntersectionVisitor( const IntersectionVisitor& );            //!< Copy constructor
        IntersectionVisitor& operator=( const IntersectionVisitor& ); //!< Assignment operator

    private :
        const UrbanModel& model_;
        InhabitantPositions::T_UrbanObjectVector& vector_;
    };
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions constructor
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
InhabitantPositions::InhabitantPositions( const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location, const UrbanModel& urbanModel, kernel::Inhabitant_ABC& inhabitant, kernel::PropertiesDictionary& dico )
    : converter_ ( converter )
    , position_( 0, 0 )
{
    IntersectionVisitor visitor( urbanModel, livingUrbanObject_ );
    location.Accept( visitor );
    ComputePosition();
    UpdateDico( inhabitant, dico );
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions constructor
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
InhabitantPositions::InhabitantPositions( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter, const UrbanModel& urbanModel, kernel::Inhabitant_ABC& inhabitant , kernel::PropertiesDictionary& dico )
    : converter_ ( converter )
    , position_( 0, 0 )
{
    ReadLocation( xis, urbanModel );
    ComputePosition();
    UpdateDico( inhabitant, dico );
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
void InhabitantPositions::ReadLocation( xml::xistream& xis, const UrbanModel& urbanModel )
{
    xis >> xml::start( "living-area" )
        >> xml::list( "urban-block" , *this, &InhabitantPositions::ReadLivingUrbanBlock, urbanModel )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::ReadLivingUrbanBlock
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
void InhabitantPositions::ReadLivingUrbanBlock( xml::xistream& xis, const UrbanModel& urbanModel )
{
    gui::TerrainObjectProxy* pObject = urbanModel.tools::Resolver< gui::TerrainObjectProxy >::Find( xis.attribute< unsigned long >( "id" ) );
    if( !pObject )
        xis.error( "error in loading living urban block for population" );
    else
        livingUrbanObject_.push_back( boost::make_tuple( pObject->GetId(), pObject->GetName(), pObject ) );
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::ComputePosition
// Created: JSR 2011-01-13
// -----------------------------------------------------------------------------
void InhabitantPositions::ComputePosition()
{
    geometry::Polygon2f poly;
    for( CIT_UrbanObjectVector it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); ++it )
        if( const kernel::UrbanPositions_ABC* positions = ( *it ).get< 2 >()->Retrieve< kernel::UrbanPositions_ABC >() )
            poly.Add( positions->Barycenter() );
    if( !poly.IsEmpty() )
        position_ = poly.Barycenter();
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
                << xml::attribute( "id", ( *it ).get< 0 >() )
            << xml::end;
    }
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::GetPosition
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
geometry::Point2f InhabitantPositions::GetPosition( bool /*aggregated*/ ) const
{
    return position_;
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
// Name: InhabitantPositions::IsAggregated
// Created: LGY 2011-03-04
// -----------------------------------------------------------------------------
bool InhabitantPositions::IsAggregated() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::Draw
// Created: JSR 2011-01-13
// -----------------------------------------------------------------------------
void InhabitantPositions::Draw( const geometry::Point2f& /*where*/, const kernel::Viewport_ABC& /*viewport*/, const kernel::GlTools_ABC& tools ) const
{
    for( CIT_UrbanObjectVector it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); ++it )
    {
        const gui::TerrainObjectProxy& object = *( *it ).get< 2 >();
        if( const kernel::UrbanPositions_ABC* positions = object.Retrieve< kernel::UrbanPositions_ABC >() )
            tools.DrawPolygon( positions->Vertices() );
    }
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::UpdateDico
// Created: SLG 2011-02-15
// -----------------------------------------------------------------------------
void InhabitantPositions::UpdateDico( kernel::Inhabitant_ABC& inhabitant, kernel::PropertiesDictionary& dico )
{
    for ( CIT_UrbanObjectVector it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); ++it )
        dico.Register( inhabitant, tools::translate( "Population", "Living Area/%1" ).arg( ( *it ).get< 0 >() ), ( *it ).get< 1 >() );
}
