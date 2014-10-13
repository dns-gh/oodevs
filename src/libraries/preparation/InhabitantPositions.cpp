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

#include "UrbanHierarchies.h"
#include "UrbanModel.h"

#include "clients_gui/GLView_ABC.h"
#include "clients_gui/Infrastructure_ABC.h"
#include "clients_gui/UrbanObject.h"
#include "clients_kernel/AccommodationType.h"
#include "clients_kernel/AccommodationTypes.h"
#include "clients_kernel/InfrastructureType.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/ObjectExtensions.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/UrbanPositions_ABC.h"

#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

namespace
{
    class IntersectionVisitor : public kernel::LocationVisitor_ABC
    {
    public:
        IntersectionVisitor( const UrbanModel& model, std::function< void( const kernel::UrbanObject_ABC&, const geometry::Polygon2f& ) > fun )
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
            tools::Iterator< const kernel::UrbanObject_ABC& > it = model_.tools::Resolver< kernel::UrbanObject_ABC >::CreateIterator();
            while( it.HasMoreElements() )
                fun_( it.NextElement(), poly );
        }

        virtual void VisitLines    ( const T_PointVector& ) {}
        virtual void VisitRectangle( const T_PointVector& ) {}
        virtual void VisitCircle   ( const geometry::Point2f& , float ) {}
        virtual void VisitPoint    ( const geometry::Point2f& ) {}
        virtual void VisitPath     ( const geometry::Point2f& , const T_PointVector& ) {}
        virtual void VisitCurve    ( const T_PointVector& ) {}
        virtual void VisitText     ( const QString&, const QFont&, const geometry::Point2f& ) {}

    private :
        IntersectionVisitor( const IntersectionVisitor& );            //!< Copy constructor
        IntersectionVisitor& operator=( const IntersectionVisitor& ); //!< Assignment operator

    private :
        const UrbanModel& model_;
        std::function< void( const kernel::UrbanObject_ABC&, const geometry::Polygon2f& ) > fun_;
    };
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions constructor
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
InhabitantPositions::InhabitantPositions( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, const kernel::Location_ABC& location,
                                          const UrbanModel& urbanModel, kernel::Inhabitant_ABC& inhabitant, gui::PropertiesDictionary& dictionary )
    : controller_( controller )
    , converter_ ( converter )
    , urbanModel_( urbanModel )
    , inhabitant_( inhabitant )
    , dictionary_( dictionary )
    , position_  ( 0, 0 )
    , livingUrbanObjects_( 0 )
    , nominalCapacity_( 0 )
    , infrastructures_( 0 )
    , medicalInfrastructures_( 0 )
{
    IntersectionVisitor visitor( urbanModel, boost::bind( &InhabitantPositions::Add, this, _1, _2 ) );
    location.Accept( visitor );
    ComputePosition();
    UpdateDictionary();
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions constructor
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
InhabitantPositions::InhabitantPositions( xml::xistream& xis, kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, const UrbanModel& urbanModel,
                                          kernel::Inhabitant_ABC& inhabitant , gui::PropertiesDictionary& dictionary )
    : controller_( controller )
    , converter_ ( converter )
    , urbanModel_( urbanModel )
    , inhabitant_( inhabitant )
    , dictionary_( dictionary )
    , position_  ( 0, 0 )
    , livingUrbanObjects_( 0 )
    , nominalCapacity_( 0 )
    , infrastructures_( 0 )
    , medicalInfrastructures_( 0 )
{
    xis >> xml::start( "living-area" )
            >> xml::list( "urban-block" , *this, &InhabitantPositions::ReadLivingUrbanBlock )
        >> xml::end;
    ComputePosition();
    UpdateDictionary();
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions destructor
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
InhabitantPositions::~InhabitantPositions()
{
    controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::ReadLivingUrbanBlock
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
void InhabitantPositions::ReadLivingUrbanBlock( xml::xistream& xis )
{
    kernel::UrbanObject_ABC* pObject = urbanModel_.tools::Resolver< kernel::UrbanObject_ABC >::Find( xis.attribute< unsigned long >( "id" ) );
    if( pObject )
        livingUrbanObject_.push_back( T_UrbanObject( pObject->GetId(), pObject->GetName().toStdString(), *pObject ) );
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::ComputePosition
// Created: JSR 2011-01-13
// -----------------------------------------------------------------------------
void InhabitantPositions::ComputePosition()
{
    geometry::Polygon2f poly;
    for( auto it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); ++it )
        if( const kernel::UrbanPositions_ABC* positions = ( *it ).pUrban_->Retrieve< kernel::UrbanPositions_ABC >() )
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
    for( auto it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); ++it )
        xos << xml::start( "urban-block" )
                << xml::attribute( "id", ( *it ).id_ )
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
// Name: InhabitantPositions::Draw
// Created: JSR 2011-01-13
// -----------------------------------------------------------------------------
void InhabitantPositions::Draw( const geometry::Point2f& /*where*/, const gui::Viewport_ABC& /*viewport*/, gui::GLView_ABC& tools ) const
{
    for( auto it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); ++it )
        if( const kernel::UrbanPositions_ABC* positions = ( *it ).pUrban_->Retrieve< kernel::UrbanPositions_ABC >() )
            tools.DrawPolygon( positions->Vertices() );
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::UpdateDictionary
// Created: SLG 2011-02-15
// -----------------------------------------------------------------------------
void InhabitantPositions::UpdateDictionary()
{
    accomodationCapacties_.clear();
    infrastructures_ = medicalInfrastructures_ = nominalCapacity_ = 0;
    for( auto it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); ++it )
    {
        auto pProxy = static_cast< const gui::UrbanObject* >( it->pUrban_ );
        nominalCapacity_ += static_cast< unsigned int >( pProxy->GetNominalCapacity() );
        if( auto infra = pProxy->Retrieve< gui::Infrastructure_ABC >() )
        {
            if( infra->GetType() && infra->GetType()->IsMedical() )
                ++medicalInfrastructures_;
            else
                ++infrastructures_;
        }
        const kernel::AccommodationTypes& accommodations = pProxy->GetAccommodations();
        tools::Iterator< const kernel::AccommodationType& > itAcco = accommodations.CreateIterator();
        while( itAcco.HasMoreElements() )
        {
            const kernel::AccommodationType& accomodation = itAcco.NextElement();
            accomodationCapacties_[ QString::fromStdString( accomodation.GetRole() ) ] += static_cast< unsigned int >( pProxy->GetNominalCapacity( accomodation.GetRole() ) );
        }
    }

    livingUrbanObjects_ = static_cast< unsigned int >( livingUrbanObject_.size() );
    dictionary_.Register( inhabitant_, tools::translate( "Population", "Living Area/Urban blocks number" ), livingUrbanObjects_, true );
    dictionary_.Register( inhabitant_, tools::translate( "Population", "Living Area/Total capacity" ),  nominalCapacity_, true );
    dictionary_.Register( inhabitant_, tools::translate( "Population", "Living Area/Medical infrastructures" ), medicalInfrastructures_, true );
    dictionary_.Register( inhabitant_, tools::translate( "Population", "Living Area/Non medical infrastructures" ), infrastructures_, true );
    for( QMap< QString, unsigned int >::const_iterator it = accomodationCapacties_.constBegin(); it != accomodationCapacties_.constEnd(); ++it )
        dictionary_.Register( inhabitant_, tools::translate( "Population", "Living Area/Capacities/%1" ).arg( it.key() ), it.value(), true );
    for( auto it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); ++it )
        dictionary_.Register( inhabitant_, tools::translate( "Population", "Living Area/Urban blocks/%1" ).arg( ( *it ).id_ ), ( *it ).name_, true );
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
void InhabitantPositions::Add( const kernel::UrbanObject_ABC& object, const geometry::Polygon2f& polygon )
{
    const UrbanHierarchies* urbanHierarchies = static_cast< const UrbanHierarchies* >( object.Retrieve< kernel::Hierarchies >() );
    if( urbanHierarchies && urbanHierarchies->GetLevel() == eUrbanLevelBlock )
    {
        if( const kernel::UrbanPositions_ABC* positions = object.Retrieve< kernel::UrbanPositions_ABC >() )
            if( polygon.IsInside( positions->Barycenter() ) && !Exists( object.GetId() ) )
                livingUrbanObject_.push_back( T_UrbanObject( object.GetId(), object.GetName().toStdString(), object ) );
    }
}

namespace
{
    bool Check( InhabitantPositions::T_UrbanObject& urbanObject, const kernel::UrbanObject_ABC& object )
    {
        return urbanObject.pUrban_->GetId() == object.GetId();
    }
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::Remove
// Created: LGY 2011-09-19
// -----------------------------------------------------------------------------
void InhabitantPositions::Remove( const kernel::UrbanObject_ABC& object, const geometry::Polygon2f& polygon )
{
    if( const kernel::UrbanPositions_ABC* positions = object.Retrieve< kernel::UrbanPositions_ABC >() )
        if( polygon.IsInside( positions->Barycenter() ) && Exists( object.GetId() ) )
            livingUrbanObject_.erase( std::remove_if( livingUrbanObject_.begin(), livingUrbanObject_.end(),
                                                      boost::bind( &Check, _1, boost::cref( object ) ) ), livingUrbanObject_.end() );
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::Exists
// Created: LGY 2011-09-19
// -----------------------------------------------------------------------------
bool InhabitantPositions::Exists( unsigned long id ) const
{
    for( auto it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); ++it )
        if( (*it).pUrban_->GetId() == id )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::StartEdition
// Created: LGY 2012-01-11
// -----------------------------------------------------------------------------
void InhabitantPositions::StartEdition()
{
    edition_.clear();
    edition_ = livingUrbanObject_;
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::Accept
// Created: LGY 2012-01-11
// -----------------------------------------------------------------------------
void InhabitantPositions::Accept()
{
    for( auto it = edition_.begin(); it != edition_.end(); ++it )
        dictionary_.Remove( tools::translate( "Population", "Living Area/Urban blocks/%1" ).arg( ( *it ).id_ ) );
    UpdateDictionary();
    controller_.Update( inhabitant_ );
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::Reject
// Created: LGY 2012-01-11
// -----------------------------------------------------------------------------
void InhabitantPositions::Reject()
{
    for( auto it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); ++it )
        dictionary_.Remove( tools::translate( "Population", "Living Area/Urban blocks/%1" ).arg( ( *it ).id_ ) );
    livingUrbanObject_ = edition_;
    UpdateDictionary();
    controller_.Update( inhabitant_ );
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::NotifyDeleted
// Created: JSR 2012-06-08
// -----------------------------------------------------------------------------
void InhabitantPositions::NotifyDeleted( const kernel::UrbanObject_ABC& block )
{
    if( livingUrbanObject_.empty() )
        return;
    auto it = livingUrbanObject_.begin();
    while( it != livingUrbanObject_.end() )
    {
        if( it->pUrban_ == &block )
        {
            dictionary_.Remove( tools::translate( "Population", "Living Area/Urban blocks/%1" ).arg( it->id_ ) );
            it = livingUrbanObject_.erase( it );
        }
        else
            ++it;
    }
    UpdateDictionary();
    controller_.Update( inhabitant_ );
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::Update
// Created: LGY 2012-01-12
// -----------------------------------------------------------------------------
void InhabitantPositions::Update( const geometry::Point2f& point )
{
    tools::Iterator< const kernel::UrbanObject_ABC& > it = urbanModel_.tools::Resolver< kernel::UrbanObject_ABC >::CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::UrbanObject_ABC& object = it.NextElement();
        const UrbanHierarchies* urbanHierarchies = static_cast< const UrbanHierarchies* >( object.Retrieve< kernel::Hierarchies >() );
        if( urbanHierarchies && urbanHierarchies->GetLevel() == eUrbanLevelBlock )
        {
            if( const kernel::UrbanPositions_ABC* positions = object.Retrieve< kernel::UrbanPositions_ABC >() )
                if( positions->IsInside( point ) )
                {
                    if( Exists( object.GetId() ) )
                        livingUrbanObject_.erase( std::remove_if( livingUrbanObject_.begin(), livingUrbanObject_.end(),
                        boost::bind( &Check, _1, boost::cref( object ) ) ), livingUrbanObject_.end() );
                    else
                        livingUrbanObject_.push_back( T_UrbanObject( object.GetId(), object.GetName().toStdString(), object ) );
                    return;
                }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: InhabitantPositions::NotifyUpdated
// Created: MMC 2013-05-29
// -----------------------------------------------------------------------------
void InhabitantPositions::NotifyUpdated( const kernel::UrbanObject_ABC& block )
{
    for( auto it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); ++it )
        if( ( *it ).pUrban_ == &block  && ( *it ).id_ != block.GetId() )
           ( *it ).id_ = block.GetId();
}
