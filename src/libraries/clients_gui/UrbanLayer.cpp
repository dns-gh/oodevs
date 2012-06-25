// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "UrbanLayer.h"
#include "View_ABC.h"
#include "clients_kernel/Hierarchies.h"
#include "clients_kernel/ResourceNetwork_ABC.h"
#include "clients_kernel/Infrastructure_ABC.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include <graphics/extensions.h>
#include <boost/noncopyable.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UrbanLayer::UrbanLayer
// Created: SLG 2009-03-23
// -----------------------------------------------------------------------------
UrbanLayer::UrbanLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy,
                        View_ABC& view, const kernel::Profile_ABC& profile )
    : EntityLayer< kernel::UrbanObject_ABC >( controllers, tools, strategy, view, profile )
    , view_          ( view )
    , controllers_   ( controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::~UrbanLayer
// Created: SLG 2009-03-23
// -----------------------------------------------------------------------------
UrbanLayer::~UrbanLayer()
{
    // NOTHING
}

namespace
{
    struct DrawExtensionsFunctor : boost::noncopyable
    {
        DrawExtensionsFunctor( kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools, bool drawInfrastructure )
            : viewport_          ( viewport )
            , tools_             ( tools )
            , drawInfrastructure_( drawInfrastructure )
        {}

        void operator()( const kernel::Entity_ABC& object )
        {
            if( const kernel::UrbanPositions_ABC* positions = object.Retrieve< kernel::UrbanPositions_ABC >() )
            {
                const kernel::ResourceNetwork_ABC* resource = object.Retrieve< kernel::ResourceNetwork_ABC >();
                const kernel::Infrastructure_ABC* infra = drawInfrastructure_ ? object.Retrieve< kernel::Infrastructure_ABC >() : 0;
                if( resource ||  infra )
                {
                    viewport_.SetHotpoint( positions->Barycenter() );
                    if( resource )
                        resource->Draw( viewport_, tools_ );
                    if( infra )
                        infra->Draw( viewport_, tools_ );
                }
            }
        }
        kernel::Viewport_ABC& viewport_;
        const kernel::GlTools_ABC& tools_;
        bool drawInfrastructure_;
    };
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::Paint
// Created: SLG 2006-03-23
// -----------------------------------------------------------------------------
void UrbanLayer::Paint( kernel::Viewport_ABC& viewport )
{
    gl::Initialize();
    // dessin des blocs urbains
    EntityLayer< kernel::UrbanObject_ABC >::Paint( viewport );
    // dessin des extensions(en deux temps pour les afficher par dessus les blocs)
    DrawExtensionsFunctor functor( viewport, tools_, controllers_.options_.GetOption( "Infra", true ).To< bool >() );
    Apply( functor );
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::Reset
// Created: RPD 2009-10-14
// -----------------------------------------------------------------------------
void UrbanLayer::Reset2d()
{
    EntityLayer< kernel::UrbanObject_ABC >::Reset2d();
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::NotifySelectionChanged
// Created: JSR 2012-05-22
// -----------------------------------------------------------------------------
void UrbanLayer::NotifySelectionChanged( const std::vector< const kernel::UrbanObject_ABC* >& elements )
{
    DeselectAll();
    for( std::vector< const kernel::UrbanObject_ABC* >::const_iterator it = elements.begin(); it != elements.end(); ++it )
    {
        static const bool bTrue = true;
         ( *it )->Interface().Apply( &kernel::UrbanPositions_ABC::SetSelection, bTrue );
        DoSelect( *it );
    }
    actualSelection_ = elements;
    EntityLayer< kernel::UrbanObject_ABC >::NotifySelectionChanged( elements );
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::DeselectAll
// Created: JSR 2012-05-25
// -----------------------------------------------------------------------------
void UrbanLayer::DeselectAll()
{
    static const bool bFalse = false;
    for( CIT_Entities it = entities_.begin(); it != entities_.end(); ++it )
        ( *it )->Interface().Apply( &kernel::UrbanPositions_ABC::SetSelection, bFalse );
    actualSelection_.clear();
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::DoSelect
// Created: JSR 2012-05-25
// -----------------------------------------------------------------------------
void UrbanLayer::DoSelect( const kernel::Entity_ABC* urbanObject )
{
    static const bool bTrue = true;
    if( !urbanObject )
        return;
    const kernel::Hierarchies* hierarchies = urbanObject->Retrieve< kernel::Hierarchies >();
    
    // TODO hack for gaming before UrbanHierarchies are implemented
    if( !hierarchies )
    {
        urbanObject->Interface().Apply( &kernel::UrbanPositions_ABC::SetSelection, bTrue );
        return;
    }
    //

    tools::Iterator< const kernel::Entity_ABC& > it = hierarchies->CreateSubordinateIterator();
    if( it.HasMoreElements() )
        while( it.HasMoreElements() )
            DoSelect( &it.NextElement() );
    else
        urbanObject->Interface().Apply( &kernel::UrbanPositions_ABC::SetSelection, bTrue );
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::NotifyDeleted
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
void UrbanLayer::NotifyDeleted( const kernel::UrbanObject_ABC& object )
{
    EntityLayer< kernel::UrbanObject_ABC >::NotifyDeleted( object );
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::ContextMenu
// Created: JSR 2011-02-23
// -----------------------------------------------------------------------------
void UrbanLayer::ContextMenu( const kernel::Entity_ABC& entity, const geometry::Point2f& geoPoint, const QPoint& point )
{
    controllers_.actions_.ContextMenu( static_cast< const kernel::UrbanObject_ABC& >( entity ), kernel::Nothing(), geoPoint, point );
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::ShouldDisplay
// Created: LDC 2010-09-10
// -----------------------------------------------------------------------------
bool UrbanLayer::ShouldDisplay( const kernel::Entity_ABC& )
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::IsInSelection
// Created: LGY 2011-04-18
// -----------------------------------------------------------------------------
bool UrbanLayer::IsInSelection( const kernel::Entity_ABC& entity, const geometry::Point2f& point ) const
{
    if( const kernel::UrbanPositions_ABC* positions = entity.Retrieve< kernel::UrbanPositions_ABC >() )
        return positions->IsInside( point );
    return false;
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::IsInside
// Created: JSR 2012-05-23
// -----------------------------------------------------------------------------
bool UrbanLayer::IsInside( const kernel::Entity_ABC& entity, const geometry::Rectangle2f& rectangle ) const
{
    if( const kernel::UrbanPositions_ABC* positions = entity.Retrieve< kernel::UrbanPositions_ABC >() )
        return positions->IsInside( rectangle );
    return false;
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::Draw
// Created: LGY 2011-04-18
// -----------------------------------------------------------------------------
void UrbanLayer::Draw( const kernel::Entity_ABC& entity, kernel::Viewport_ABC& viewport )
{
    if( ShouldDisplay( entity ) ) // && positions.IsIn( viewport ) )
    {
        if( const kernel::UrbanPositions_ABC* positions = entity.Retrieve< kernel::UrbanPositions_ABC >() )
        {
            SelectColor( entity );
            const geometry::Point2f position = positions->Barycenter();
            viewport.SetHotpoint( position );
            entity.Draw( position, viewport, tools_ );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::ActivateEntity
// Created: LGY 2011-05-02
// -----------------------------------------------------------------------------
void UrbanLayer::ActivateEntity( const kernel::Entity_ABC& entity )
{
    if( const kernel::UrbanPositions_ABC* positions = entity.Retrieve< kernel::UrbanPositions_ABC >() )
        view_.CenterOn( positions->Barycenter() );
}

namespace
{
    void Append( kernel::ActionController::T_Selectables& vector, const kernel::Selectable_ABC* element )
    {
        kernel::ActionController::CIT_Selectables it = std::find( vector.begin(), vector.end(), element );
        if( it == vector.end() )
            vector.push_back( element );
    }

    void Remove( kernel::ActionController::T_Selectables& vector, const kernel::Selectable_ABC* element )
    {
        kernel::ActionController::IT_Selectables it = std::find( vector.begin(), vector.end(), element );
        if( it != vector.end() )
            vector.erase( it );
    }

    void AppendDistrict( kernel::ActionController::T_Selectables& vector, const kernel::Entity_ABC* district, const kernel::Entity_ABC* entity )
    {
        Remove( vector, district );
        tools::Iterator< const kernel::Entity_ABC& > districtIt = district->Get< kernel::Hierarchies >().CreateSubordinateIterator();
        while( districtIt.HasMoreElements() )
        {
            const kernel::Entity_ABC& block = districtIt.NextElement();
            if( &block == entity )
                Remove( vector, entity );
            else
                Append( vector, &block );
        }
    }

    void AppendCity( kernel::ActionController::T_Selectables& vector, const kernel::Entity_ABC* city, const kernel::Entity_ABC* district, const kernel::Entity_ABC* entity )
    {
        Remove( vector, city );
        const kernel::Hierarchies& cityHierarchies = city->Get< kernel::Hierarchies >();
        tools::Iterator< const kernel::Entity_ABC& > cityIt = cityHierarchies.CreateSubordinateIterator();
        while( cityIt.HasMoreElements() )
        {
            const kernel::Entity_ABC& subElement = cityIt.NextElement();
            if( &subElement == district )
                AppendDistrict( vector, district, entity );
            else
                Append( vector, &subElement );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::Select
// Created: JSR 2012-05-29
// -----------------------------------------------------------------------------
void UrbanLayer::Select( const kernel::Entity_ABC& entity, bool control, bool shift )
{
    const kernel::UrbanPositions_ABC* positions = entity.Retrieve< kernel::UrbanPositions_ABC >();
    const kernel::Hierarchies* hierarchies = entity.Retrieve< kernel::Hierarchies >();
    if( !control || !hierarchies || controllers_.actions_.IsSingleSelection( &entity ) || ( positions && !positions->IsSelected() ) )
        EntityLayerBase::Select( entity, control, shift );
    else
    {
        const kernel::Entity_ABC* district = hierarchies->GetSuperior();
        const kernel::Entity_ABC* city = district ? district->Get< kernel::Hierarchies >().GetSuperior() : 0;
        bool citySelected = ( city && std::find( actualSelection_.begin(), actualSelection_.end(), city ) != actualSelection_.end() );
        bool districtSelected = citySelected ? false : ( district && std::find( actualSelection_.begin(), actualSelection_.end(), district ) != actualSelection_.end() );
        if( citySelected || districtSelected )
        {
            kernel::ActionController::T_Selectables newSelection;
            for( std::vector< const kernel::UrbanObject_ABC* >::const_iterator it = actualSelection_.begin(); it != actualSelection_.end(); ++it )
                newSelection.push_back( *it );
            if( citySelected )
                AppendCity( newSelection, city, district, &entity );
            else
                AppendDistrict( newSelection, district, &entity );
            controllers_.actions_.SetMultipleSelection( newSelection );
        }
        else
            EntityLayerBase::Select( entity, control, shift );
    }
}
