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
#include "TerrainObjectProxy.h"
#include "View_ABC.h"
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
    : EntityLayer< TerrainObjectProxy >( controllers, tools, strategy, view, profile, filter )
    , view_          ( view )
    , controllers_   ( controllers )
    , selectedObject_( 0 )
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

        void operator()( const kernel::Entity_ABC& proxy )
        {
            if( const kernel::UrbanPositions_ABC* positions = proxy.Retrieve< kernel::UrbanPositions_ABC >() )
            {
                const kernel::ResourceNetwork_ABC* resource = proxy.Retrieve< kernel::ResourceNetwork_ABC >();
                const kernel::Infrastructure_ABC* infra = drawInfrastructure_ ? proxy.Retrieve< kernel::Infrastructure_ABC >() : 0;
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
    EntityLayer< TerrainObjectProxy >::Paint( viewport );
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
    selectedObject_ = 0;
    EntityLayer< TerrainObjectProxy >::Reset2d();
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::NotifySelected
// Created: SLG 2009-03-23
// -----------------------------------------------------------------------------
void UrbanLayer::NotifySelected( const TerrainObjectProxy* object )
{
    if( selectedObject_ )
        selectedObject_->Interface().Apply( &kernel::UrbanPositions_ABC::ToggleSelection );
    selectedObject_ = object;
    if( selectedObject_ )
        selectedObject_->Interface().Apply( &kernel::UrbanPositions_ABC::ToggleSelection );
    EntityLayer< TerrainObjectProxy >::NotifySelected( object );
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::NotifyDeleted
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
void UrbanLayer::NotifyDeleted( const TerrainObjectProxy& object )
{
    if( &object == selectedObject_ )
        selectedObject_ = 0;
    EntityLayer< TerrainObjectProxy >::NotifyDeleted( object );
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
