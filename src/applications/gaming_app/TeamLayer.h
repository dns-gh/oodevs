// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __TeamLayer_h_
#define __TeamLayer_h_

#include "clients_gui/EntityLayer.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Team_ABC;
    class Time_ABC;
}

namespace actions
{
    class ActionsModel;
}

class AgentServerMsgMgr;
class StaticModel;

// =============================================================================
/** @class  TeamLayer
    @brief  TeamLayer
*/
// Created: LDC 2010-10-26
// =============================================================================
class TeamLayer : public gui::EntityLayer< kernel::Team_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             TeamLayer( kernel::Controllers& controllers,
                        gui::GLView_ABC& view,
                        gui::ColorStrategy_ABC& strategy,
                        const kernel::Profile_ABC& profile,
                        actions::ActionsModel& actionsModel,
                        const StaticModel& staticModel,
                        const kernel::Time_ABC& simulation,
                        AgentServerMsgMgr& messageManager );
    virtual ~TeamLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const ViewFrustum& frustum );
    virtual void Paint( const geometry::Rectangle2f& viewport );
    virtual void Paint( gui::Viewport_ABC& viewport );

    virtual bool CanDrop( QDragMoveEvent* event, const geometry::Point2f& point ) const;
    virtual bool HandleDropEvent( QDropEvent* event, const geometry::Point2f& point );
    virtual bool HandleMousePress( QMouseEvent* event, const geometry::Point2f& point );
    virtual bool HandleMouseMove( QMouseEvent* , const geometry::Point2f& point );
    virtual void NotifyActivated( const kernel::Team_ABC& entity );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelectionChanged( const std::vector< const kernel::Team_ABC* >& elements );
    //@}

private:
    //! @name Member data
    //@{
    actions::ActionsModel& actionsModel_;
    const StaticModel& static_;
    const kernel::Time_ABC& simulation_;
    AgentServerMsgMgr& messageManager_;
    kernel::SafePointer< kernel::Team_ABC > selected_;
    //@}
};

#endif // __TeamLayer_h_
