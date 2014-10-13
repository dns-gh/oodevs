// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectsLayer_h_
#define __ObjectsLayer_h_

#include "clients_gui/ObjectsLayer.h"
#include "clients_kernel/OptionsObserver_ABC.h"

// =============================================================================
/** @class  ObjectsLayer
    @brief  ObjectsLayer
*/
// Created: SBO 2006-10-16
// =============================================================================
class ObjectsLayer : public gui::ObjectsLayer
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectsLayer( kernel::Controllers& controllers,
                           gui::GLView_ABC& view,
                           gui::ColorStrategy_ABC& strategy,
                           const kernel::Profile_ABC& profile,
                           gui::TerrainPicker& picker );
    virtual ~ObjectsLayer();
    //@}

    //! @name Operations
    //@{
    virtual bool HandleKeyPress( QKeyEvent* key );
    virtual bool CanDrop( QDragMoveEvent* event, const geometry::Point2f& point ) const;
    virtual bool HandleDropEvent( QDropEvent* event, const geometry::Point2f& point );
    virtual bool HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& point );
    virtual bool HandleMoveDragEvent( QDragMoveEvent* event, const geometry::Point2f& point );
    virtual bool HandleLeaveDragEvent( QDragLeaveEvent* event );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelectionChanged( const std::vector< const kernel::Object_ABC* >& elements );
    virtual bool HandleMousePress( QMouseEvent* event, const geometry::Point2f& point );
    bool IsEligibleForDrag() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::SafePointer< kernel::Object_ABC > selected_;
    geometry::Point2f draggingPoint_;
    geometry::Point2f oldPosition_;
    std::unique_ptr< QWidget > dummy_;
    //@}
};

#endif // __ObjectsLayer_h_
