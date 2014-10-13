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
LayerComposite::LayerComposite( kernel::Controllers& controllers,
                                GLView_ABC& tools,
                                const T_LayersMap& layersMap,
                                E_LayerTypes type )
    : Layer( controllers, tools, type )
{
    const auto& childrenTypes = GetChildrenTypes();
    if( childrenTypes.empty() )
        throw MASA_EXCEPTION( "bad composite descriptor, no children defined" );
    for( auto itType = childrenTypes.begin(); itType != childrenTypes.end(); ++itType )
    {
        auto itLayer = layersMap.find( *itType );
        if( itLayer != layersMap.end() && itLayer->second )
            layers_.push_back( itLayer->second );
    }
    if( layers_.size() < 2 )
        throw MASA_EXCEPTION( "bad composite initialisation, not enough children" );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::~LayerComposite
// Created: SLI 2014-07-01
// -----------------------------------------------------------------------------
LayerComposite::~LayerComposite()
{
    // NOTHING
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
// Name: LayerComposite::SetAlpha
// Created: SLI 2014-06-25
// -----------------------------------------------------------------------------
void LayerComposite::SetAlpha( float alpha )
{
    Forward( layers_, [&]( const T_Layer& layer ){ layer->SetAlpha( alpha ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::Paint
// Created: SLI 2014-06-25
// -----------------------------------------------------------------------------
void LayerComposite::Paint( gui::Viewport_ABC& viewport )
{
    if( !ShouldDrawPass() )
        return;
    Forward( layers_, [&]( const T_Layer& layer ){ layer->Paint( viewport ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::Paint
// Created: ABR 2014-10-14
// -----------------------------------------------------------------------------
void LayerComposite::Paint( const geometry::Rectangle2f& viewport )
{
    if( !ShouldDrawPass() )
        return;
    Forward( layers_, [&]( const T_Layer& layer ){ layer->Paint( viewport ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::Reset
// Created: SLI 2014-06-27
// -----------------------------------------------------------------------------
void LayerComposite::Reset()
{
    Forward( layers_, [&]( const T_Layer& layer ){ layer->Reset(); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::Select
// Created: SLI 2014-06-27
// -----------------------------------------------------------------------------
void LayerComposite::Select( unsigned int id, bool control )
{
    Forward( layers_, [&]( const T_Layer& layer ){ layer->Select( id, control ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::ContextMenu
// Created: SLI 2014-06-27
// -----------------------------------------------------------------------------
void LayerComposite::ContextMenu( const kernel::GraphicalEntity_ABC& entity, const geometry::Point2f& point, const QPoint& qpoint )
{
    Forward( layers_, [&]( const T_Layer& layer ){ layer->ContextMenu( entity, point, qpoint ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::ExtractElements
// Created: SLI 2014-06-27
// -----------------------------------------------------------------------------
void LayerComposite::ExtractElements( T_LayerElements& extractedElement, const T_ObjectsPicking& selection )
{
    Forward( layers_, [&]( const T_Layer& layer ){ layer->ExtractElements( extractedElement, selection ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::Initialize
// Created: SLI 2014-07-02
// -----------------------------------------------------------------------------
void LayerComposite::Initialize( const geometry::Rectangle2f& extent )
{
    Forward( layers_, [&]( const T_Layer& layer ){ layer->Initialize( extent ); } );
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
    return ForwardResult( layers_, [&]( const T_Layer& layer ) { return layer->CanDrop( event, point ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::HandleKeyPress
// Created: SLI 2014-07-02
// -----------------------------------------------------------------------------
bool LayerComposite::HandleKeyPress( QKeyEvent* key )
{
    return ForwardResult( layers_, [&]( const T_Layer& layer ) { return layer->HandleKeyPress( key ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::HandleEnterDragEvent
// Created: SLI 2014-07-02
// -----------------------------------------------------------------------------
bool LayerComposite::HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& point )
{
    return ForwardResult( layers_, [&]( const T_Layer& layer ) { return layer->HandleEnterDragEvent( event, point ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::HandleDropEvent
// Created: SLI 2014-07-02
// -----------------------------------------------------------------------------
bool LayerComposite::HandleDropEvent( QDropEvent* event, const geometry::Point2f& point )
{
    return ForwardResult( layers_, [&]( const T_Layer& layer ) { return layer->HandleDropEvent( event, point ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::HandleMoveDragEvent
// Created: SLI 2014-07-02
// -----------------------------------------------------------------------------
bool LayerComposite::HandleMoveDragEvent( QDragMoveEvent* event, const geometry::Point2f& point )
{
    return ForwardResult( layers_, [&]( const T_Layer& layer ) { return layer->HandleMoveDragEvent( event, point ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::HandleLeaveDragEvent
// Created: SLI 2014-07-02
// -----------------------------------------------------------------------------
bool LayerComposite::HandleLeaveDragEvent( QDragLeaveEvent* event )
{
    return ForwardResult( layers_, [&]( const T_Layer& layer ) { return layer->HandleLeaveDragEvent( event ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::HandleKeyRelease
// Created: SLI 2014-07-02
// -----------------------------------------------------------------------------
bool LayerComposite::HandleKeyRelease( QKeyEvent* key )
{
    return ForwardResult( layers_, [&]( const T_Layer& layer ) { return layer->HandleKeyRelease( key ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::HandleMousePress
// Created: SLI 2014-07-02
// -----------------------------------------------------------------------------
bool LayerComposite::HandleMousePress( QMouseEvent* mouse, const geometry::Point2f& point )
{
    return ForwardResult( layers_, [&]( const T_Layer& layer ) { return layer->HandleMousePress( mouse, point ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::HandleMouseDoubleClick
// Created: SLI 2014-07-02
// -----------------------------------------------------------------------------
bool LayerComposite::HandleMouseDoubleClick( QMouseEvent* mouse, const geometry::Point2f& point )
{
    return ForwardResult( layers_, [&]( const T_Layer& layer ) { return layer->HandleMouseDoubleClick( mouse, point ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::HandleMouseMove
// Created: SLI 2014-07-02
// -----------------------------------------------------------------------------
bool LayerComposite::HandleMouseMove( QMouseEvent* mouse, const geometry::Point2f& point )
{
    return ForwardResult( layers_, [&]( const T_Layer& layer ) { return layer->HandleMouseMove( mouse, point ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::HandleMouseWheel
// Created: SLI 2014-07-02
// -----------------------------------------------------------------------------
bool LayerComposite::HandleMouseWheel( QWheelEvent* event, const geometry::Point2f& point )
{
    return ForwardResult( layers_, [&]( const T_Layer& layer ) { return layer->HandleMouseWheel( event, point ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::IsReadOnly
// Created: SLI 2014-07-02
// -----------------------------------------------------------------------------
bool LayerComposite::IsReadOnly() const
{
    return ForwardResult( layers_, [&]( const T_Layer& layer ) { return layer->IsReadOnly(); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::IsIn
// Created: MCO 2014-07-16
// -----------------------------------------------------------------------------
bool LayerComposite::IsIn( const kernel::GraphicalEntity_ABC& entity ) const
{
    return ForwardResult( layers_, [&]( const T_Layer& layer ) { return layer->IsIn( entity ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::SetReadOnlyModes
// Created: LDC 2014-07-07
// -----------------------------------------------------------------------------
void LayerComposite::SetReadOnlyModes( int modes )
{
     Forward( layers_, [&]( const T_Layer& layer ) { layer->SetReadOnlyModes( modes ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::ShowTooltip
// Created: ABR 2014-10-13
// -----------------------------------------------------------------------------
bool LayerComposite::ShowTooltip( const T_ObjectPicking& selection )
{
    return ForwardResult( layers_, [&]( const T_Layer& layer ) { return layer->ShowTooltip( selection ); } );
}

// -----------------------------------------------------------------------------
// Name: LayerComposite::HideTooltip
// Created: ABR 2014-10-13
// -----------------------------------------------------------------------------
void LayerComposite::HideTooltip()
{
    Forward( layers_, [&]( const T_Layer& layer ) { layer->HideTooltip(); } );
}
