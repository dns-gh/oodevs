// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationsLayer_h_
#define __PopulationsLayer_h_

#include "clients_gui/PopulationsLayer.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include "clients_kernel/MultipleSelectionObserver_ABC.h"

namespace kernel
{
    class Entity_ABC;
}

class Model;

// =============================================================================
/** @class  PopulationsLayer
    @brief  PopulationsLayer
*/
// Created: SBO 2006-11-09
// =============================================================================
class PopulationsLayer : public gui::PopulationsLayer
                       , public kernel::MultipleSelectionObserver_Base< kernel::Entity_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationsLayer( kernel::Controllers& controllers, gui::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy,
                               gui::View_ABC& view, Model& model, const kernel::Profile_ABC& profile );
    virtual ~PopulationsLayer();
    //@}

    //! @name Operations
    //@{
    virtual bool CanDrop( QDragMoveEvent* event, const geometry::Point2f& point ) const;
    virtual bool HandleDropEvent( QDropEvent* event, const geometry::Point2f& point );
    virtual bool HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& point );
    virtual bool HandleMoveDragEvent( QDragMoveEvent* event, const geometry::Point2f& point );
    virtual bool HandleLeaveDragEvent( QDragLeaveEvent* event );
    virtual bool HandleKeyPress( QKeyEvent* key );
    virtual bool HandleMousePress( QMouseEvent* event, const geometry::Point2f& point );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void MultipleSelect( const std::vector< const kernel::Entity_ABC* >& elements );
    virtual void MultipleSelect( const std::vector< const kernel::Population_ABC* >& elements );
    bool IsEligibleForDrag();
    //@}

private:
    //! @name Member data
    //@{
    Model& model_;
    kernel::SafePointer< kernel::Entity_ABC > selectedEntity_;
    kernel::SafePointer< kernel::Population_ABC > selectedPopulation_;
    geometry::Point2f draggingPoint_;
    geometry::Point2f draggingOffset_;
    geometry::Point2f oldPosition_;
    std::unique_ptr< QWidget > dummy_;
    //@}
};

#endif // __PopulationsLayer_h_
