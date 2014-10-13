// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __GhostsLayer_h_
#define __GhostsLayer_h_

#include "clients_gui/EntityLayer.h"
#include "clients_kernel/Ghost_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Automat_ABC;
    class Formation_ABC;
}

class Model;

// =============================================================================
/** @class  GhostsLayer
    @brief  GhostsLayer
*/
// Created: ABR 2011-10-19
// =============================================================================
class GhostsLayer : public gui::EntityLayer< kernel::Ghost_ABC >
                  , public kernel::MultipleSelectionObserver_Base< kernel::Automat_ABC >
                  , public kernel::MultipleSelectionObserver_Base< kernel::Formation_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             GhostsLayer( kernel::Controllers& controllers,
                          gui::GLView_ABC& view,
                          gui::ColorStrategy_ABC& strategy,
                          Model& model,
                          const kernel::Profile_ABC& profile );
    virtual ~GhostsLayer();
    //@}

    //! @name Operations
    //@{
    virtual bool CanDrop( QDragMoveEvent* event, const geometry::Point2f& point ) const;
    virtual bool HandleDropEvent( QDropEvent* event, const geometry::Point2f& point );
    virtual bool HandleMousePress( QMouseEvent* event, const geometry::Point2f& point );
    virtual bool HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& point );
    virtual bool HandleMoveDragEvent( QDragMoveEvent* event, const geometry::Point2f& point );
    virtual bool HandleLeaveDragEvent( QDragLeaveEvent* event );
    virtual bool HandleKeyPress( QKeyEvent* key );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void BeforeMultiSelection();
    virtual void MultipleSelect( const std::vector< const kernel::Ghost_ABC* >& elements );
    virtual void MultipleSelect( const std::vector< const kernel::Automat_ABC* >& elements );
    virtual void MultipleSelect( const std::vector< const kernel::Formation_ABC* >& elements );
    virtual void SetAlpha( float alpha );
    bool IsEligibleForDrag();
    //@}

private:
    //! @name Member data
    //@{
    Model& model_;
    kernel::SafePointer< kernel::Ghost_ABC >     selectedGhost_;
    kernel::SafePointer< kernel::Automat_ABC >   selectedAutomat_;
    kernel::SafePointer< kernel::Formation_ABC > selectedFormation_;
    kernel::SafePointer< kernel::Ghost_ABC >     highLightedGhost_;
    geometry::Point2f draggingPoint_;
    geometry::Point2f draggingOffset_;
    geometry::Point2f oldPosition_;
    std::unique_ptr< QWidget > dummy_;
    //@}
};

#endif // __GhostsLayer_h_
