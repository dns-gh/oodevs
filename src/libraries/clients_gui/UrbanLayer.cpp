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

#include "DrawVisitor.h"
#include "View_ABC.h"
#include "Viewport_ABC.h"
#include "clients_kernel/Hierarchies.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include <graphics/extensions.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UrbanLayer::UrbanLayer
// Created: SLG 2009-03-23
// -----------------------------------------------------------------------------
UrbanLayer::UrbanLayer( kernel::Controllers& controllers, GlTools_ABC& tools, ColorStrategy_ABC& strategy,
                        View_ABC& view, const kernel::Profile_ABC& profile )
    : EntityLayer< kernel::UrbanObject_ABC >( controllers, tools, strategy, view, profile, eLayerTypes_Urban )
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

// -----------------------------------------------------------------------------
// Name: UrbanLayer::IsSelected
// Created: LGY 2013-04-04
// -----------------------------------------------------------------------------
bool UrbanLayer::IsSelected( const kernel::Entity_ABC& entity ) const
{
    return std::find( actualSelection_.begin(), actualSelection_.end(), &entity ) != actualSelection_.end();
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
        ( *it )->GetInterfaces().Apply( &kernel::UrbanPositions_ABC::SetSelection, bTrue );
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
    for( auto it = entities_.begin(); it != entities_.end(); ++it )
        ( *it )->GetInterfaces().Apply( &kernel::UrbanPositions_ABC::SetSelection, bFalse );
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
        urbanObject->GetInterfaces().Apply( &kernel::UrbanPositions_ABC::SetSelection, bTrue );
        return;
    }
    //

    tools::Iterator< const kernel::Entity_ABC& > it = hierarchies->CreateSubordinateIterator();
    if( it.HasMoreElements() )
        while( it.HasMoreElements() )
            DoSelect( &it.NextElement() );
    else
        urbanObject->GetInterfaces().Apply( &kernel::UrbanPositions_ABC::SetSelection, bTrue );
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
void UrbanLayer::ContextMenu( const kernel::GraphicalEntity_ABC& selectable, const geometry::Point2f& geoPoint, const QPoint& point )
{
    const kernel::UrbanObject_ABC& urbanObject = static_cast< const kernel::UrbanObject_ABC& >( selectable );
    controllers_.actions_.ContextMenu( this, urbanObject, kernel::Nothing(), geoPoint, point );
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
// Name: UrbanLayer::ActivateEntity
// Created: LGY 2011-05-02
// -----------------------------------------------------------------------------
void UrbanLayer::ActivateEntity( const kernel::Entity_ABC& entity )
{
    if( const kernel::UrbanPositions_ABC* positions = entity.Retrieve< kernel::UrbanPositions_ABC >() )
    {
        const geometry::Point2f barycenter = positions->Barycenter();
        if( !barycenter.IsZero() )
            view_.CenterOn( barycenter );
    }
}

namespace
{
    void Append( kernel::GraphicalEntity_ABC::T_GraphicalEntities& vector, const kernel::GraphicalEntity_ABC* element )
    {
        auto it = std::find( vector.begin(), vector.end(), element );
        if( it == vector.end() )
            vector.push_back( element );
    }

    void Remove( kernel::GraphicalEntity_ABC::T_GraphicalEntities& vector, const kernel::GraphicalEntity_ABC* element )
    {
        auto it = std::find( vector.begin(), vector.end(), element );
        if( it != vector.end() )
            vector.erase( it );
    }

    void AppendDistrict( kernel::GraphicalEntity_ABC::T_GraphicalEntities& vector, const kernel::Entity_ABC* district, const kernel::Entity_ABC* entity )
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

    void AppendCity( kernel::GraphicalEntity_ABC::T_GraphicalEntities& vector, const kernel::Entity_ABC* city, const kernel::Entity_ABC* district, const kernel::Entity_ABC* entity )
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
void UrbanLayer::Select( unsigned int id, bool control )
{
    const auto* entity = FindEntity( id );
    if( !entity )
        return;

    const kernel::UrbanPositions_ABC* positions = entity->Retrieve< kernel::UrbanPositions_ABC >();
    const kernel::Hierarchies* hierarchies = entity->Retrieve< kernel::Hierarchies >();
    if( !control || !hierarchies || controllers_.actions_.IsSingleSelection( entity ) || ( positions && !positions->IsSelected() ) )
        EntityLayerBase::Select( id, control );
    else
    {
        const kernel::Entity_ABC* district = hierarchies->GetSuperior();
        const kernel::Entity_ABC* city = district ? district->Get< kernel::Hierarchies >().GetSuperior() : 0;
        bool citySelected = ( city && std::find( actualSelection_.begin(), actualSelection_.end(), city ) != actualSelection_.end() );
        bool districtSelected = citySelected ? false : ( district && std::find( actualSelection_.begin(), actualSelection_.end(), district ) != actualSelection_.end() );
        if( citySelected || districtSelected )
        {
            kernel::GraphicalEntity_ABC::T_GraphicalEntities newSelection;
            for( std::vector< const kernel::UrbanObject_ABC* >::const_iterator it = actualSelection_.begin(); it != actualSelection_.end(); ++it )
                newSelection.push_back( *it );
            if( citySelected )
                AppendCity( newSelection, city, district, entity );
            else
                AppendDistrict( newSelection, district, entity );
            controllers_.actions_.SetMultipleSelection( newSelection );
        }
        else
            EntityLayerBase::Select( id, control );
    }
}
