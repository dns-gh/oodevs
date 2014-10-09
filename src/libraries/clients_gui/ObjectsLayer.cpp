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

#include "Pickable_ABC.h"
#include "TerrainPicker.h"

#include "clients_kernel/ObjectType.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectsLayer constructor
// Created: AGE 2006-03-23
// -----------------------------------------------------------------------------
ObjectsLayer::ObjectsLayer( Controllers& controllers, GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view,
                            const Profile_ABC& profile, TerrainPicker& picker )
    : EntityLayer< Object_ABC >( controllers, tools, strategy, view, profile, eLayerTypes_Objects )
    , picker_( picker )
{
    picker_.RegisterLayer( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer destructor
// Created: AGE 2006-03-23
// ------------------------------------------------------------------------------
ObjectsLayer::~ObjectsLayer()
{
    picker_.UnregisterLayer( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectsLayer::ContextMenu
// Created: SBO 2006-11-29
// -----------------------------------------------------------------------------
void ObjectsLayer::ContextMenu( const GraphicalEntity_ABC& selectable, const geometry::Point2f& point, const QPoint& where )
{
    const Entity_ABC& entity = static_cast< const Entity_ABC& >( selectable );
    const Object_ABC& object = static_cast< const Object_ABC& >( entity );
    if( object.GetType().IsUrban() )
        controllers_.actions_.ContextMenu( this, object, point, Nothing(), where );
    else
        controllers_.actions_.ContextMenu( this, object, entity, point, where );
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
