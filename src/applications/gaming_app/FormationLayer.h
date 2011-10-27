// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __FormationLayer_h_
#define __FormationLayer_h_

#include "clients_gui/FormationLayer.h"
#include "tools/Resolver_ABC.h"

namespace actions
{
    class ActionsModel;
}

namespace kernel
{
    class Automat_ABC;
    class AutomatType;
    struct PopulationPrototype;
    class Time_ABC;
}

class AgentServerMsgMgr;
class StaticModel;

// =============================================================================
/** @class  FormationLayer
    @brief  FormationLayer
*/
// Created: LDC 2010-10-06
// =============================================================================
class FormationLayer : public gui::FormationLayer
{

public:
    //! @name Constructors/Destructor
    //@{
    FormationLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy,
                    gui::View_ABC& view, const kernel::Profile_ABC& profile, actions::ActionsModel& actionsModel, const StaticModel& staticModel,
                    const kernel::Time_ABC& simulation, AgentServerMsgMgr& messageManager, tools::Resolver_ABC< kernel::Automat_ABC >& agentsModel,
                    const gui::LayerFilter_ABC& filter );
    virtual ~FormationLayer();
    //@}

    //! @name Operations
    //@{
    virtual bool CanDrop( QDragMoveEvent* event, const geometry::Point2f& point ) const;
    virtual bool HandleDropEvent( QDropEvent* event, const geometry::Point2f& point );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Formation_ABC* formation );
    void RequestCreation( const geometry::Point2f& point, const kernel::AutomatType& type );
    void RequestCreation( const geometry::Point2f& point, const kernel::PopulationPrototype& type );
    //@}

private:
    //! @name Member data
    //@{
    kernel::SafePointer< kernel::Formation_ABC > selected_;
    actions::ActionsModel& actionsModel_;
    const StaticModel& static_;
    const kernel::Time_ABC& simulation_;
    AgentServerMsgMgr& messageManager_;
    tools::Resolver_ABC< kernel::Automat_ABC >& agentsModel_;
    //@}
};

#endif // __FormationsLayer_h_
