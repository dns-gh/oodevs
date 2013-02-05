// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ObjectsLayer.h"

#include "ResourceNetwork_ABC.h"
#include "TerrainPicker.h"

#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Pickable_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectsLayer constructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
ObjectsLayer::ObjectsLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view, const Profile_ABC& profile, TerrainPicker& picker )
    : EntityLayer< Object_ABC >( controllers, tools, strategy, view, profile, tr( "Objects" ) )
{
    picker.RegisterLayer( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer destructor
// Created: AGE 2006-03-23
// ------------------------------------------------------------------------------
ObjectsLayer::~ObjectsLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::ContextMenu
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void ObjectsLayer::ContextMenu( const Selectable_ABC& selectable, const geometry::Point2f& point, const QPoint& where )
{
    const Entity_ABC& entity = static_cast< const Entity_ABC& >( selectable );
    const Object_ABC& object = static_cast< const Object_ABC& >( entity );
    if( object.GetType().IsUrban() )
        controllers_.actions_.ContextMenu( object, point, Nothing(), where );
    else
        controllers_.actions_.ContextMenu( object, entity, point, where );
}

namespace
{
    struct DrawExtensionsFunctor : boost::noncopyable
    {
        DrawExtensionsFunctor( const Viewport_ABC& viewport, const GlTools_ABC& tools )
            : viewport_( viewport )
            , tools_( tools )
        {}

        void operator()( const Entity_ABC& object )
        {
            // dessin du réseau en dernier par dessus les objets
            if( const ResourceNetwork_ABC* resource = object.Retrieve< ResourceNetwork_ABC >() )
                resource->Draw( viewport_, tools_ );
        }

        const Viewport_ABC& viewport_;
        const GlTools_ABC& tools_;
    };
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::Paint
// Created: JSR 2010-09-06
// -----------------------------------------------------------------------------
void ObjectsLayer::Paint( Viewport_ABC& viewport )
{
    // dessin des objets
    EntityLayer< kernel::Object_ABC >::Paint( viewport );
    // dessin des extensions(en deux temps pour les afficher par dessus les objets)
    DrawExtensionsFunctor functor( viewport, tools_ );
    Apply( functor );
}

namespace
{
    struct TerrainPickFunctor : boost::noncopyable
    {
        TerrainPickFunctor( const geometry::Point2f& terrainCoordinates )
            : terrainCoordinates_( terrainCoordinates )
        {}

        void operator()( const Entity_ABC& object )
        {
            object.GetInterfaces().Apply( &Pickable_ABC::TerrainPicked, terrainCoordinates_, infos_ );
        }

        QStringList infos_;
        const geometry::Point2f& terrainCoordinates_;
    };
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::TerrainPick
// Created: BCI 2011-01-04
// -----------------------------------------------------------------------------
QStringList ObjectsLayer::TerrainPick( const geometry::Point2f& terrainCoordinates )
{
    TerrainPickFunctor functor( terrainCoordinates );
    Apply( functor );
    return functor.infos_;
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::IsInSelection
// Created: ABR 2013-01-29
// -----------------------------------------------------------------------------
//bool ObjectsLayer::IsInSelection( const kernel::Entity_ABC& entity, const geometry::Point2f& point ) const
//{
//    return EntityLayer< kernel::Object_ABC >::IsInSelection( entity, point );
//    //const kernel::Object_ABC& object = static_cast< const kernel::Object_ABC& >( entity );
//}
