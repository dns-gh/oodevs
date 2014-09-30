// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LayerComposite.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LayerComposite::LayerComposite
// Created: SLI 2014-07-01
// -----------------------------------------------------------------------------
LayerComposite::LayerComposite( kernel::Controllers& controllers, GlTools_ABC& tools, E_LayerTypes type )
    : Layer( controllers, tools, type )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::~LayerComposite
// Created: SLI 2014-07-01
// -----------------------------------------------------------------------------
LayerComposite::~LayerComposite()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::Register
// Created: SLI 2014-06-25
// -----------------------------------------------------------------------------
void LayerComposite::Register( const std::shared_ptr< Layer_ABC >& layer )
{
    layers_.push_back( layer );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::SetAlpha
// Created: SLI 2014-06-25
// -----------------------------------------------------------------------------
void LayerComposite::SetAlpha( float alpha )
{
    std::for_each( layers_.begin(), layers_.end(), [&]( const std::shared_ptr< Layer_ABC >& layer ){ layer->SetAlpha( alpha ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::Paint
// Created: SLI 2014-06-25
// -----------------------------------------------------------------------------
void LayerComposite::Paint( gui::Viewport_ABC& viewport )
{
    if( !ShouldDrawPass() )
        return;
    std::for_each( layers_.begin(), layers_.end(), [&]( const std::shared_ptr< Layer_ABC >& layer ){ layer->Paint( viewport ); } );
}

namespace
{
    template< typename Layers, typename Functor >
    void Forward( const Layers& layers, const Functor& functor )
    {
        std::for_each( layers.begin(), layers.end(), functor );
    }
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::Reset
// Created: SLI 2014-06-27
// -----------------------------------------------------------------------------
void LayerComposite::Reset()
{
    Forward( layers_, [&]( const std::shared_ptr< Layer_ABC >& layer ){ layer->Reset(); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::Select
// Created: SLI 2014-06-27
// -----------------------------------------------------------------------------
void LayerComposite::Select( const kernel::GraphicalEntity_ABC& entity, bool control, bool shift )
{
    Forward( layers_, [&]( const std::shared_ptr< Layer_ABC >& layer ){ layer->Select( entity, control, shift ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::ContextMenu
// Created: SLI 2014-06-27
// -----------------------------------------------------------------------------
void LayerComposite::ContextMenu( const kernel::GraphicalEntity_ABC& entity, const geometry::Point2f& point, const QPoint& qpoint )
{
    Forward( layers_, [&]( const std::shared_ptr< Layer_ABC >& layer ){ layer->ContextMenu( entity, point, qpoint ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::ExtractElements
// Created: SLI 2014-06-27
// -----------------------------------------------------------------------------
void LayerComposite::ExtractElements( T_LayerElements& extractedElement, const T_ObjectsPicking& selection )
{
    Forward( layers_, [&]( const std::shared_ptr< Layer_ABC >& layer ){ layer->ExtractElements( extractedElement, selection ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::Initialize
// Created: SLI 2014-07-02
// -----------------------------------------------------------------------------
void LayerComposite::Initialize( const geometry::Rectangle2f& extent )
{
    Forward( layers_, [&]( const std::shared_ptr< Layer_ABC >& layer ){ layer->Initialize( extent ); } );
}

namespace
{
    template< typename Layers, typename Functor >
    bool ForwardResult( const Layers& layers, const Functor& functor )
    {
        for( auto it = layers.begin(); it != layers.end(); ++it )
            if( functor( *it ) )
                return true;
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::CanDrop
// Created: SLI 2014-07-02
// -----------------------------------------------------------------------------
bool LayerComposite::CanDrop( QDragMoveEvent* event, const geometry::Point2f& point ) const
{
    return ForwardResult( layers_, [&]( const std::shared_ptr< Layer_ABC >& layer ) { return layer->CanDrop( event, point ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::HandleKeyPress
// Created: SLI 2014-07-02
// -----------------------------------------------------------------------------
bool LayerComposite::HandleKeyPress( QKeyEvent* key )
{
    return ForwardResult( layers_, [&]( const std::shared_ptr< Layer_ABC >& layer ) { return layer->HandleKeyPress( key ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::HandleEnterDragEvent
// Created: SLI 2014-07-02
// -----------------------------------------------------------------------------
bool LayerComposite::HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& point )
{
    return ForwardResult( layers_, [&]( const std::shared_ptr< Layer_ABC >& layer ) { return layer->HandleEnterDragEvent( event, point ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::HandleDropEvent
// Created: SLI 2014-07-02
// -----------------------------------------------------------------------------
bool LayerComposite::HandleDropEvent( QDropEvent* event, const geometry::Point2f& point )
{
    return ForwardResult( layers_, [&]( const std::shared_ptr< Layer_ABC >& layer ) { return layer->HandleDropEvent( event, point ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::HandleMoveDragEvent
// Created: SLI 2014-07-02
// -----------------------------------------------------------------------------
bool LayerComposite::HandleMoveDragEvent( QDragMoveEvent* event, const geometry::Point2f& point )
{
    return ForwardResult( layers_, [&]( const std::shared_ptr< Layer_ABC >& layer ) { return layer->HandleMoveDragEvent( event, point ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::HandleLeaveDragEvent
// Created: SLI 2014-07-02
// -----------------------------------------------------------------------------
bool LayerComposite::HandleLeaveDragEvent( QDragLeaveEvent* event )
{
    return ForwardResult( layers_, [&]( const std::shared_ptr< Layer_ABC >& layer ) { return layer->HandleLeaveDragEvent( event ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::HandleKeyRelease
// Created: SLI 2014-07-02
// -----------------------------------------------------------------------------
bool LayerComposite::HandleKeyRelease( QKeyEvent* key )
{
    return ForwardResult( layers_, [&]( const std::shared_ptr< Layer_ABC >& layer ) { return layer->HandleKeyRelease( key ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::HandleMousePress
// Created: SLI 2014-07-02
// -----------------------------------------------------------------------------
bool LayerComposite::HandleMousePress( QMouseEvent* mouse, const geometry::Point2f& point )
{
    return ForwardResult( layers_, [&]( const std::shared_ptr< Layer_ABC >& layer ) { return layer->HandleMousePress( mouse, point ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::HandleMouseDoubleClick
// Created: SLI 2014-07-02
// -----------------------------------------------------------------------------
bool LayerComposite::HandleMouseDoubleClick( QMouseEvent* mouse, const geometry::Point2f& point )
{
    return ForwardResult( layers_, [&]( const std::shared_ptr< Layer_ABC >& layer ) { return layer->HandleMouseDoubleClick( mouse, point ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::HandleMouseMove
// Created: SLI 2014-07-02
// -----------------------------------------------------------------------------
bool LayerComposite::HandleMouseMove( QMouseEvent* mouse, const geometry::Point2f& point )
{
    return ForwardResult( layers_, [&]( const std::shared_ptr< Layer_ABC >& layer ) { return layer->HandleMouseMove( mouse, point ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::HandleMouseWheel
// Created: SLI 2014-07-02
// -----------------------------------------------------------------------------
bool LayerComposite::HandleMouseWheel( QWheelEvent* event, const geometry::Point2f& point )
{
    return ForwardResult( layers_, [&]( const std::shared_ptr< Layer_ABC >& layer ) { return layer->HandleMouseWheel( event, point ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::IsReadOnly
// Created: SLI 2014-07-02
// -----------------------------------------------------------------------------
bool LayerComposite::IsReadOnly() const
{
    return ForwardResult( layers_, [&]( const std::shared_ptr< Layer_ABC >& layer ) { return layer->IsReadOnly(); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::IsIn
// Created: MCO 2014-07-16
// -----------------------------------------------------------------------------
bool LayerComposite::IsIn( const kernel::GraphicalEntity_ABC& entity ) const
{
    return ForwardResult( layers_, [&]( const std::shared_ptr< Layer_ABC >& layer ) { return layer->IsIn( entity ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::SetReadOnlyModes
// Created: LDC 2014-07-07
// -----------------------------------------------------------------------------
void LayerComposite::SetReadOnlyModes( int modes )
{
     Forward( layers_, [&]( const std::shared_ptr< Layer_ABC >& layer ) { layer->SetReadOnlyModes( modes ); } );
}
