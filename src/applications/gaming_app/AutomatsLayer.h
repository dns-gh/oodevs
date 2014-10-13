// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AutomatsLayer_h_
#define __AutomatsLayer_h_

#include "clients_gui/AutomatsLayer.h"

namespace kernel
{
    class AgentType;
    class AutomatType;
    class Automat_ABC;
}

namespace actions
{
    class ActionsModel;
}

// =============================================================================
/** @class  AutomatsLayer
    @brief  AutomatsLayer
*/
// Created: SBO 2007-04-13
// =============================================================================
class AutomatsLayer : public gui::AutomatsLayer
{
public:
    //! @name Constructors/Destructor
    //@{
             AutomatsLayer( kernel::Controllers& controllers,
                            gui::GLView_ABC& view,
                            gui::ColorStrategy_ABC& strategy,
                            const kernel::Profile_ABC& profile,
                            actions::ActionsModel& actionsModel );
    virtual ~AutomatsLayer();
    //@}

    //! @name Operations
    //@{
    virtual bool HandleMousePress( QMouseEvent* event, const geometry::Point2f& point );
    virtual bool CanDrop( QDragMoveEvent* event, const geometry::Point2f& point ) const;
    virtual bool HandleDropEvent( QDropEvent* event, const geometry::Point2f& point );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void Draw( const kernel::Entity_ABC& entity, gui::Viewport_ABC& viewport, bool pickingMode );
    virtual void NotifySelectionChanged( const std::vector< const kernel::Automat_ABC* >& elements );
    //@}

private:
    //! @name Member data
    //@{
    actions::ActionsModel& actionsModel_;
    kernel::SafePointer< kernel::Automat_ABC > selected_;
    //@}
};

#endif // __AutomatsLayer_h_
