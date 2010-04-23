// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectsLayer_h_
#define __ObjectsLayer_h_

#include "clients_gui/ObjectsLayer.h"
#include "protocol/ServerPublisher_ABC.h"

namespace actions
{
    class ActionsModel;
}

class ActionPublisher;
class Simulation;
class StaticModel;

// =============================================================================
/** @class  ObjectsLayer
    @brief  Objects layer
*/
// Created: SBO 2007-04-26
// =============================================================================
class ObjectsLayer : public gui::ObjectsLayer
{

public:
    //! @name Constructors/Destructor
    //@{
    ObjectsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, const kernel::Profile_ABC& profile, ActionPublisher& actionPublisher, actions::ActionsModel& actionsModel, const StaticModel& staticModel, const Simulation& simulation );
    virtual ~ObjectsLayer();
    //@}

    //! @name Operations
    //@{
    virtual bool HandleKeyPress( QKeyEvent* key );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectsLayer( const ObjectsLayer& );            //!< Copy constructor
    ObjectsLayer& operator=( const ObjectsLayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Object_ABC* object );
    //@}

private:
    //! @name Member data
    //@{
    ActionPublisher& actionPublisher_;
    actions::ActionsModel& actionsModel_;
    const StaticModel& static_;
    const Simulation& simulation_;
    kernel::SafePointer< kernel::Object_ABC > selected_;
    //@}
};

#endif // __ObjectsLayer_h_
