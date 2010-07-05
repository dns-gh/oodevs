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

namespace kernel
{
    class Time_ABC;
}

namespace actions
{
    class ActionsModel;
}

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
             ObjectsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::View_ABC& view, const kernel::Profile_ABC& profile, actions::ActionsModel& actionsModel, const StaticModel& staticModel, const kernel::Time_ABC& simulation );
    virtual ~ObjectsLayer();
    //@}

    //! @name Operations
    //@{
    virtual bool HandleKeyPress( QKeyEvent* key );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectsLayer( const ObjectsLayer& );            //!< Copy constructor
    ObjectsLayer& operator=( const ObjectsLayer& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Object_ABC* object );
    //@}

private:
    //! @name Member data
    //@{
    actions::ActionsModel& actionsModel_;
    const StaticModel& static_;
    const kernel::Time_ABC& simulation_;
    kernel::SafePointer< kernel::Object_ABC > selected_;
    //@}
};

#endif // __ObjectsLayer_h_
