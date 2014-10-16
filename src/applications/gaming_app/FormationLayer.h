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

namespace actions
{
    class ActionsModel;
}

namespace kernel
{
    class Automat_ABC;
    class AutomatType;
    struct PopulationPrototype;
}

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
    FormationLayer( kernel::Controllers& controllers,
                    gui::GLView_ABC& view,
                    gui::ColorStrategy_ABC& strategy,
                    const kernel::Profile_ABC& profile,
                    actions::ActionsModel& actionsModel,
                    const StaticModel& staticModel );
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
    virtual void NotifySelectionChanged( const std::vector< const kernel::Formation_ABC* >& elements );
    void RequestCreation( const geometry::Point2f& point, const kernel::AutomatType& type );
    void RequestCreation( const geometry::Point2f& point, const kernel::PopulationPrototype& type );
    //@}

private:
    //! @name Member data
    //@{
    kernel::SafePointer< kernel::Formation_ABC > selected_;
    actions::ActionsModel& actionsModel_;
    const StaticModel& static_;
    //@}
};

#endif // __FormationsLayer_h_
