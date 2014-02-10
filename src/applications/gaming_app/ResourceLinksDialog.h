// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ResourceLinksDialog_h_
#define __ResourceLinksDialog_h_

#include "clients_gui/ResourceLinksDialog_ABC.h"

namespace actions
{
    class ActionsModel;
}

namespace kernel
{
    class Time_ABC;
}

class StaticModel;

// =============================================================================
/** @class  ResourceLinksDialog
    @brief  ResourceLinksDialog
*/
// Created: JSR 2010-08-24
// =============================================================================
class ResourceLinksDialog : public gui::ResourceLinksDialog_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ResourceLinksDialog( QMainWindow* parent,
                                  const kernel::Model_ABC& model,
                                  kernel::Controllers& controllers,
                                  actions::ActionsModel& actionsModel,
                                  const ::StaticModel& staticModel );
    virtual ~ResourceLinksDialog();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DoValidate( kernel::Entity_ABC* element = 0 );
    //@}

private:
    //! @name Member data
    //@{
    actions::ActionsModel& actionsModel_;
    //@}
};

#endif // __ResourceLinksDialog_h_
