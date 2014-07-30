// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __FormationLayer_h_
#define __FormationLayer_h_

#include "clients_gui/FormationLayer.h"

// =============================================================================
/** @class  FormationLayer
    @brief  FormationLayer
*/
// Created: JSR 2013-05-29
// =============================================================================
class FormationLayer : public gui::FormationLayer
{
public:
    //! @name Constructors/Destructor
    //@{
             FormationLayer( kernel::Controllers& controllers, gui::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, const kernel::Profile_ABC& profile );
    virtual ~FormationLayer();
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
    virtual void BeforeMultiSelection();
    virtual void MultipleSelect( const std::vector< const kernel::Formation_ABC* >& elements );
    bool IsEligibleForDrag() const;
    virtual bool HandleMousePress( QMouseEvent* event, const geometry::Point2f& point );

private:
    //! @name Member data
    //@{
    kernel::SafePointer< kernel::Formation_ABC > selectedFormation_;
    geometry::Point2f draggingPoint_;
    geometry::Point2f draggingOffset_;
    std::unique_ptr< QWidget > dummy_;
    //@}
};

#endif // __FormationLayer_h_
