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
#include "tools/Resolver_ABC.h"

namespace kernel
{
    class AgentType;
    class Automat_ABC;
    class CoordinateConverter_ABC;
    class Time_ABC;
}

namespace actions
{
    class ActionsModel;
}

class AgentServerMsgMgr;

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
             AutomatsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy,
                            gui::View_ABC& view, const kernel::Profile_ABC& profile, actions::ActionsModel& actionsModel,
                            const kernel::Time_ABC& simulation, AgentServerMsgMgr& messageManager, tools::Resolver_ABC< kernel::Automat_ABC >& agentsModel,
                            const gui::LayerFilter_ABC& filter );
    virtual ~AutomatsLayer();
    //@}

    //! @name Operations
    //@{
    virtual bool HandleMousePress( QMouseEvent* event, const geometry::Point2f& point );
    virtual bool HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& point );
    virtual bool HandleDropEvent( QDropEvent* event, const geometry::Point2f& point );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void Draw( const kernel::Entity_ABC& entity, kernel::Viewport_ABC& viewport );
    virtual void NotifySelected( const kernel::Automat_ABC* automat );
    void RequestCreation( const geometry::Point2f& point, const kernel::AgentType& type );
    void RequestCreation( const geometry::Point2f& point, const kernel::AutomatType& type );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::GlTools_ABC& tools_;
    actions::ActionsModel& actionsModel_;
    const kernel::Time_ABC& simulation_;
    kernel::SafePointer< kernel::Automat_ABC > selected_;
    AgentServerMsgMgr& messageManager_;
    tools::Resolver_ABC< kernel::Automat_ABC >& agentsModel_;
    //@}
};

#endif // __AutomatsLayer_h_
