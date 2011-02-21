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
#include "clients_kernel/ResourceNetwork_ABC.h"
#include "clients_kernel/Infrastructure_ABC.h"
#include <boost/noncopyable.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UrbanLayer::UrbanLayer
// Created: SLG 2009-03-23
// -----------------------------------------------------------------------------
UrbanLayer::UrbanLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view, const kernel::Profile_ABC& profile )
    : EntityLayer< TerrainObjectProxy >( controllers, tools, strategy, view, profile )
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
}

namespace
{
    struct DrawExtensionsFunctor : boost::noncopyable
    {
        DrawExtensionsFunctor( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools )
            : viewport_      ( viewport )
            , tools_         ( tools )
        {}

        void operator()( const kernel::Entity_ABC& proxy )
        {
            // dessin du réseau en dernier par dessus les blocs
            if( const kernel::ResourceNetwork_ABC* resource = proxy.Retrieve< kernel::ResourceNetwork_ABC >() )
                resource->Draw( viewport_, tools_ );
            if( const kernel::Infrastructure_ABC* infra = proxy.Retrieve< kernel::Infrastructure_ABC >() )
                infra->Draw( viewport_, tools_ );
        }

        const kernel::Viewport_ABC& viewport_;
        const kernel::GlTools_ABC& tools_;
    };
}

// -----------------------------------------------------------------------------
// Name: UrbanLayer::Paint
// Created: SLG 2006-03-23
// -----------------------------------------------------------------------------
void UrbanLayer::Paint( kernel::Viewport_ABC& viewport )
{
    // dessin des blocs urbains
    EntityLayer< TerrainObjectProxy >::Paint( viewport );
    // dessin des extensions(en deux temps pour les afficher par dessus les blocs)
    DrawExtensionsFunctor functor( viewport, tools_ );
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
        selectedObject_->SetSelected( false );
    selectedObject_ = object;
    if( selectedObject_ )
        selectedObject_->SetSelected( true );
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
// Name: UrbanLayer::ShouldDisplay
// Created: LDC 2010-09-10
// -----------------------------------------------------------------------------
bool UrbanLayer::ShouldDisplay( const kernel::Entity_ABC& )
{
    return true;
}
