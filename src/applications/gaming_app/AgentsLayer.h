// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __AgentsLayer_h_
#define __AgentsLayer_h_

#include "clients_gui/EntityLayer.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"

namespace actions
{
    class ActionsModel;
}

namespace kernel
{
    class Time_ABC;
}

// =============================================================================
/** @class  AgentsLayer
    @brief  AgentsLayer
*/
// Created: SBO 2006-08-18
// =============================================================================
class AgentsLayer : public gui::EntityLayer< kernel::Agent_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentsLayer( kernel::Controllers& controllers,
                          gui::GLView_ABC& view,
                          gui::ColorStrategy_ABC& strategy,
                          const kernel::Profile_ABC& profile,
                          actions::ActionsModel& actionsModel,
                          const kernel::Time_ABC& simulation );
    virtual ~AgentsLayer();
    //@}

    //! @name Operations
    //@{
    virtual bool CanDrop( QDragMoveEvent* event, const geometry::Point2f& point ) const;
    virtual bool HandleDropEvent( QDropEvent* event, const geometry::Point2f& point );
    virtual void Draw( const kernel::Entity_ABC& entity, gui::Viewport_ABC& viewport, bool pickingMode );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelectionChanged( const std::vector< const kernel::Agent_ABC* >& elements );
    void RequestCreation( const geometry::Point2f& point, const kernel::AgentType& type );
    //@}

private:
    //! @name Member data
    //@{
    actions::ActionsModel& actionsModel_;
    const kernel::Time_ABC& simulation_;
    kernel::SafePointer< kernel::Agent_ABC > selected_;
    bool displayDestroyedUnits_;
    //@}
};

#endif // __AgentsLayer_h_
