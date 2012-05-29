// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
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
                        View_ABC& view, const kernel::Profile_ABC& profile, const gui::LayerFilter_ABC& filter )
    : EntityLayer< kernel::UrbanObject_ABC >( controllers, tools, strategy, view, profile, filter )
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
    selectedObjects_.clear();
    EntityLayer< kernel::UrbanObject_ABC >::Reset2d();
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::NotifySelected
// Created: SLG 2009-03-23
// -----------------------------------------------------------------------------
void UrbanLayer::NotifySelected( const kernel::UrbanObject_ABC* object )
{
    DeselectAll();
    DoSelect( object );
    EntityLayer< kernel::UrbanObject_ABC >::NotifySelected( object );
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::NotifySelectionChanged
// Created: JSR 2012-05-22
// -----------------------------------------------------------------------------
void UrbanLayer::NotifySelectionChanged( const std::vector< const kernel::UrbanObject_ABC* >& elements )
{
    DeselectAll();
    for( std::vector< const kernel::UrbanObject_ABC* >::const_iterator it = elements.begin(); it != elements.end(); ++it )
        DoSelect( *it );
    EntityLayer< kernel::UrbanObject_ABC >::NotifySelectionChanged( elements );
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::DeselectAll
// Created: JSR 2012-05-25
// -----------------------------------------------------------------------------
void UrbanLayer::DeselectAll()
{
    static const bool bFalse = false;
    for( std::set< const kernel::Entity_ABC* >::const_iterator it = selectedObjects_.begin(); it != selectedObjects_.end(); ++it )
        ( *it )->Interface().Apply( &kernel::UrbanPositions_ABC::SetSelection, bFalse );
    selectedObjects_.clear();
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
        selectedObjects_.insert( urbanObject );
        urbanObject->Interface().Apply( &kernel::UrbanPositions_ABC::SetSelection, bTrue );
        return;
    }
    //

    tools::Iterator< const kernel::Entity_ABC& > it = hierarchies->CreateSubordinateIterator();
    if( it.HasMoreElements() )
    {
        while( it.HasMoreElements() )
            DoSelect( &it.NextElement() );
    }
    else
    {
        selectedObjects_.insert( urbanObject );
        urbanObject->Interface().Apply( &kernel::UrbanPositions_ABC::SetSelection, bTrue );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::NotifyDeleted
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
void UrbanLayer::NotifyDeleted( const kernel::UrbanObject_ABC& object )
{
    std::set< const kernel::Entity_ABC* >::iterator it = selectedObjects_.find( &object );
    if( it != selectedObjects_.end() )
        selectedObjects_.erase( it );
    EntityLayer< kernel::UrbanObject_ABC >::NotifyDeleted( object );
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::ContextMenu
// Created: JSR 2011-02-23
// -----------------------------------------------------------------------------
void UrbanLayer::ContextMenu( const kernel::Entity_ABC& entity, const geometry::Point2f& geoPoint, const QPoint& point )
{
    controllers_.actions_.ContextMenu( static_cast< const kernel::Object_ABC& >( entity ), kernel::Nothing(), geoPoint, point );
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
