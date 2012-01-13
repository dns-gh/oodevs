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
#include "EditorProxy.h"
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
class GhostsLayer : public QObject
                  , public EditorProxy< gui::EntityLayer< kernel::Ghost_ABC > >
                  , public tools::SelectionObserver_Base< kernel::Automat_ABC >
                  , public tools::SelectionObserver_Base< kernel::Formation_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             GhostsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy,
                          gui::View_ABC& view, Model& model, const kernel::Profile_ABC& profile, const gui::LayerFilter_ABC& filter );
    virtual ~GhostsLayer();
    //@}

    //! @name Operations
    //@{
    virtual bool CanDrop( QDragMoveEvent* event, const geometry::Point2f& point ) const;
    virtual bool HandleDropEvent( QDropEvent* event, const geometry::Point2f& point );
    virtual bool HandleMousePress( QMouseEvent* event, const geometry::Point2f& point );
    virtual bool HandleMoveDragEvent( QDragMoveEvent* event, const geometry::Point2f& point );
    virtual bool HandleKeyPress( QKeyEvent* key );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void Select( const kernel::Ghost_ABC& element );
    virtual void Select( const kernel::Automat_ABC& element );
    virtual void Select( const kernel::Formation_ABC& element );
    virtual void SetAlpha( float alpha );
    bool IsEligibleForDrag( const geometry::Point2f& point );
    //@}

private:
    //! @name Member data
    //@{
    Model& model_;
    kernel::SafePointer< kernel::Ghost_ABC >     selectedGhost_;
    kernel::SafePointer< kernel::Automat_ABC >   selectedAutomat_;
    kernel::SafePointer< kernel::Formation_ABC > selectedFormation_;
    kernel::SafePointer< kernel::Ghost_ABC >     highLightedGhost_;
    QWidget* dummy_;
    geometry::Point2f draggingPoint_;
    geometry::Point2f draggingOffset_;
    //@}
};

#endif // __GhostsLayer_h_
