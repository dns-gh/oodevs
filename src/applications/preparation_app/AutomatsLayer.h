// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __AutomatsLayer_h_
#define __AutomatsLayer_h_

#include "clients_gui/AutomatsLayer.h"

// =============================================================================
/** @class  AutomatsLayer
    @brief  AutomatsLayer
*/
// Created: JSR 2013-05-29
// =============================================================================
class AutomatsLayer : public gui::AutomatsLayer
{
public:
    //! @name Constructors/Destructor
    //@{
             AutomatsLayer( kernel::Controllers& controllers, gui::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, const kernel::Profile_ABC& profile );
    virtual ~AutomatsLayer();
    //@}

    //! @name Operations
    //@{
    virtual bool CanDrop( QDragMoveEvent* event, const geometry::Point2f& point ) const;
    virtual bool HandleDropEvent( QDropEvent* event, const geometry::Point2f& point );
    virtual bool HandleMoveDragEvent( QDragMoveEvent* event, const geometry::Point2f& point );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void BeforeSelection();
    virtual void MultipleSelect( const std::vector< const kernel::Automat_ABC* >& elements );
    bool IsEligibleForDrag() const;
    virtual bool HandleMousePress( QMouseEvent* event, const geometry::Point2f& point );

private:
    //! @name Member data
    //@{
    kernel::SafePointer< kernel::Automat_ABC > selectedAutomat_;
    geometry::Point2f draggingPoint_;
    geometry::Point2f draggingOffset_;
    std::unique_ptr< QWidget > dummy_;
    //@}
};

#endif // __AutomatsLayer_h_
