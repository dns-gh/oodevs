// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __LayerComposite_h_
#define __LayerComposite_h_

#include "Layer.h"
#include "LayersHelpers.h"

namespace gui
{
    class Viewport_ABC;

// =============================================================================
/** @class  LayerComposite
    @brief  LayerComposite
*/
// Created: SLI 2014-07-01
// =============================================================================
class LayerComposite : public Layer
{
public:
    //! @name Constructors/Destructor
    //@{
             LayerComposite( kernel::Controllers& controllers,
                             GLView_ABC& tools,
                             const T_LayersMap& layersMap,
                             E_LayerTypes type );
    virtual ~LayerComposite();
    //@}

    //! @name Forwarded operations
    //@{
    virtual void SetAlpha( float alpha );
    virtual void Paint( gui::Viewport_ABC& viewport );
    virtual void Paint( const geometry::Rectangle2f& viewport );

    virtual void Reset();
    virtual void Select( unsigned int id, bool control );
    virtual void ContextMenu( const kernel::GraphicalEntity_ABC&, const geometry::Point2f&, const QPoint& );
    virtual void ExtractElements( T_LayerElements& extractedElement, const T_ObjectsPicking& selection );
    virtual bool IsIn( const kernel::GraphicalEntity_ABC& ) const;

    virtual void Initialize( const geometry::Rectangle2f& extent );

    virtual bool HandleKeyPress        ( QKeyEvent* key );
    virtual bool HandleKeyRelease      ( QKeyEvent* key );
    virtual bool HandleMousePress      ( QMouseEvent*     mouse, const geometry::Point2f& point );
    virtual bool HandleMouseDoubleClick( QMouseEvent*     mouse, const geometry::Point2f& point );
    virtual bool HandleMouseMove       ( QMouseEvent*     mouse, const geometry::Point2f& point );
    virtual bool HandleMouseWheel      ( QWheelEvent*     event, const geometry::Point2f& point );
    virtual bool HandleDropEvent       ( QDropEvent*      event, const geometry::Point2f& point );
    virtual bool HandleEnterDragEvent  ( QDragEnterEvent* event, const geometry::Point2f& point );
    virtual bool HandleMoveDragEvent   ( QDragMoveEvent*  event, const geometry::Point2f& point );
    virtual bool HandleLeaveDragEvent  ( QDragLeaveEvent* event                                 );

    virtual bool CanDrop( QDragMoveEvent* event, const geometry::Point2f& point ) const;

    virtual bool IsReadOnly() const;
    virtual void SetReadOnlyModes( int modes );
    virtual bool ShowTooltip( const T_ObjectPicking& selection );
    virtual void HideTooltip();
    //@}

private:
    T_LayersVector layers_;
};

} //! namespace gui

#endif // __LayerComposite_h_
