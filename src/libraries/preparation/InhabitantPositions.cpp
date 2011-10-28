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
#include <boost/function.hpp>
#include <boost/bind.hpp>

namespace
{
    class IntersectionVisitor : public kernel::LocationVisitor_ABC
    {
    public:
        IntersectionVisitor( const UrbanModel& model, boost::function< void( const gui::TerrainObjectProxy&, const geometry::Polygon2f& ) > fun )
            : model_( model )
            , fun_  ( fun )
        {
            // NOTHING
        }
        virtual ~IntersectionVisitor()
        {
            // NOTHING
        }
        virtual void VisitPolygon( const T_PointVector& points )
        {
            const geometry::Polygon2f poly( points );
            tools::Iterator< const gui::TerrainObjectProxy& > it = model_.tools::Resolver< gui::TerrainObjectProxy >::CreateIterator();
            while( it.HasMoreElements() )
                fun_( it.NextElement(), poly );
        }

        virtual void VisitLines    ( const T_PointVector& ) {}
        virtual void VisitRectangle( const T_PointVector& ) {}
        virtual void VisitCircle   ( const geometry::Point2f& , float ) {}
        virtual void VisitPoint    ( const geometry::Point2f& ) {}
        virtual void VisitPath     ( const geometry::Point2f& , const T_PointVector& ) {}

    private :
        IntersectionVisitor( const IntersectionVisitor& );            //!< Copy constructor
        IntersectionVisitor& operator=( const IntersectionVisitor& ); //!< Assignment operator

    private :
        const UrbanModel& model_;
        boost::function< void( const gui::TerrainObjectProxy&, const geometry::Polygon2f& ) > fun_;
    };
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions constructor
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
InhabitantPositions::InhabitantPositions( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location,
                                          const UrbanModel& urbanModel, kernel::Inhabitant_ABC& inhabitant, kernel::PropertiesDictionary& dictionary )
    : controller_( controller )
    , converter_ ( converter )
    , urbanModel_( urbanModel )
    , inhabitant_( inhabitant )
    , dictionary_( dictionary )
    , position_  ( 0, 0 )
{
    IntersectionVisitor visitor( urbanModel, boost::bind( &InhabitantPositions::Add, this, _1, _2 ) );
    location.Accept( visitor );
    ComputePosition();
    UpdateDictionary();
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions constructor
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
InhabitantPositions::InhabitantPositions( xml::xistream& xis, kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, const UrbanModel& urbanModel,
                                          kernel::Inhabitant_ABC& inhabitant , kernel::PropertiesDictionary& dictionary )
    : controller_( controller )
    , converter_ ( converter )
    , urbanModel_( urbanModel )
    , inhabitant_( inhabitant )
    , dictionary_( dictionary )
    , position_  ( 0, 0 )
{
    xis >> xml::start( "living-area" )
            >> xml::list( "urban-block" , *this, &InhabitantPositions::ReadLivingUrbanBlock )
        >> xml::end;
    ComputePosition();
    UpdateDictionary();
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
// Name: InhabitantPositions::ReadLivingUrbanBlock
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
void InhabitantPositions::ReadLivingUrbanBlock( xml::xistream& xis )
{
    gui::TerrainObjectProxy* pObject = urbanModel_.tools::Resolver< gui::TerrainObjectProxy >::Find( xis.attribute< unsigned long >( "id" ) );
    if( pObject )
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
        xos << xml::start( "urban-block" )
                << xml::attribute( "id", ( *it ).get< 0 >() )
            << xml::end;
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
        if( const kernel::UrbanPositions_ABC* positions = ( *it ).get< 2 >()->Retrieve< kernel::UrbanPositions_ABC >() )
            tools.DrawPolygon( positions->Vertices() );
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::UpdateDictionary
// Created: SLG 2011-02-15
// -----------------------------------------------------------------------------
void InhabitantPositions::UpdateDictionary()
{
    for( CIT_UrbanObjectVector it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); ++it )
        dictionary_.Register( inhabitant_, tools::translate( "Population", "Living Area/%1" ).arg( ( *it ).get< 0 >() ), ( *it ).get< 1 >() );
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::Add
// Created: LGY 2011-09-19
// -----------------------------------------------------------------------------
void InhabitantPositions::Add( const kernel::Location_ABC& location )
{
    IntersectionVisitor visitor( urbanModel_, boost::bind( &InhabitantPositions::Add, this, _1, _2 ) );
    location.Accept( visitor );
    ComputePosition();
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::Remove
// Created: LGY 2011-09-19
// -----------------------------------------------------------------------------
void InhabitantPositions::Remove( const kernel::Location_ABC& location )
{
    IntersectionVisitor visitor( urbanModel_, boost::bind( &InhabitantPositions::Remove, this, _1, _2 ) );
    location.Accept( visitor );
    ComputePosition();
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::Add
// Created: LGY 2011-09-19
// -----------------------------------------------------------------------------
void InhabitantPositions::Add( const gui::TerrainObjectProxy& object, const geometry::Polygon2f& polygon )
{
    for( CIT_UrbanObjectVector it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); ++it )
        dictionary_.Remove( tools::translate( "Population", "Living Area/%1" ).arg( ( *it ).get< 0 >() ) );
    if( const kernel::UrbanPositions_ABC* positions = object.Retrieve< kernel::UrbanPositions_ABC >() )
        if( polygon.IsInside( positions->Barycenter() ) && !Exists( object.GetId() ) )
            livingUrbanObject_.push_back( boost::make_tuple( object.GetId(), object.GetName(), &object ) );
    UpdateDictionary();
    controller_.Update( inhabitant_ );
}

namespace
{
    bool Check( InhabitantPositions::T_UrbanObject& urbanObject, const gui::TerrainObjectProxy& object )
    {
        return urbanObject.get< 2 >()->GetId() == object.GetId();
    }
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::Remove
// Created: LGY 2011-09-19
// -----------------------------------------------------------------------------
void InhabitantPositions::Remove( const gui::TerrainObjectProxy& object, const geometry::Polygon2f& polygon )
{
    for( CIT_UrbanObjectVector it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); ++it )
        dictionary_.Remove( tools::translate( "Population", "Living Area/%1" ).arg( ( *it ).get< 0 >() ) );
    if( const kernel::UrbanPositions_ABC* positions = object.Retrieve< kernel::UrbanPositions_ABC >() )
        if( polygon.IsInside( positions->Barycenter() ) && Exists( object.GetId() ) )
            livingUrbanObject_.erase( std::remove_if( livingUrbanObject_.begin(), livingUrbanObject_.end(),
                                                      boost::bind( &Check, _1, boost::cref( object ) ) ), livingUrbanObject_.end() );
    UpdateDictionary();
    controller_.Update( inhabitant_ );
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::Exists
// Created: LGY 2011-09-19
// -----------------------------------------------------------------------------
bool InhabitantPositions::Exists( unsigned long id ) const
{
    for( CIT_UrbanObjectVector it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); ++it )
        if( (*it).get< 2 >()->GetId() == id )
            return true;
    return false;
}
