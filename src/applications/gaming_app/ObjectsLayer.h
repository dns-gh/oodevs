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

namespace gui
{
    class LayerFilter_ABC;
}

namespace kernel
{
    class Time_ABC;
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
             ObjectsLayer( kernel::Controllers& controllers,
                           gui::GLView_ABC& view,
                           gui::ColorStrategy_ABC& strategy,
                           const kernel::Profile_ABC& profile,
                           actions::ActionsModel& actionsModel,
                           const StaticModel& staticModel,
                           const kernel::Time_ABC& simulation,
                           gui::TerrainPicker& picker );
    virtual ~ObjectsLayer();
    //@}

    //! @name Operations
    //@{
    virtual bool HandleKeyPress( QKeyEvent* key );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelectionChanged( const std::vector< const kernel::Object_ABC* >& elements );
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
