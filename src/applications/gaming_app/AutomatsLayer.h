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
    class CoordinateConverter_ABC;
    class AgentType;
}

namespace actions
{
    class ActionsModel;
}

class ActionPublisher;
class Simulation;
class StaticModel;

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
             AutomatsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, const kernel::Profile_ABC& profile, gui::AgentsLayer& agents, ActionPublisher& actionPublisher, actions::ActionsModel& actionsModel, const StaticModel& staticModel, const Simulation& simulation );
    virtual ~AutomatsLayer();
    //@}

    //! @name Operations
    //@{
    virtual bool HandleEnterDragEvent( QDragEnterEvent* event, const geometry::Point2f& point );
    virtual bool HandleDropEvent( QDropEvent* event, const geometry::Point2f& point );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AutomatsLayer( const AutomatsLayer& );            //!< Copy constructor
    AutomatsLayer& operator=( const AutomatsLayer& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Draw( const kernel::Entity_ABC& entity, kernel::Viewport_ABC& viewport );
    virtual void NotifySelected( const kernel::Automat_ABC* automat );
    void RequestCreation( const geometry::Point2f& point, const kernel::AgentType& type );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::GlTools_ABC& tools_;
    ActionPublisher& actionPublisher_;
    actions::ActionsModel& actionsModel_;
    const StaticModel& static_;
    const Simulation& simulation_;
    kernel::SafePointer< kernel::Automat_ABC > selected_;
    //@}
};

#endif // __AutomatsLayer_h_
