// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LogisticListView_h_
#define __LogisticListView_h_

#include "clients_gui/LogisticListView.h"

namespace actions
{
    class ActionsModel;
}

namespace gui
{
    class ItemFactory_ABC;
    class EntitySymbols;
}

namespace kernel
{
    class Controllers;
    class Profile_ABC;
    class Time_ABC;
}

class LogisticLinks;
class StaticModel;

// =============================================================================
/** @class  LogisticListView
    @brief  LogisticListView
*/
// Created: ABR 2011-09-16
// =============================================================================
class LogisticListView : public gui::LogisticListView
                       , public tools::ElementObserver_ABC< LogisticLinks >

{
public:
    //! @name Constructors/Destructor
    //@{
    LogisticListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory,
                      const kernel::Profile_ABC& profile, const gui::EntitySymbols& symbols, actions::ActionsModel& actionsModel,
                      const ::StaticModel& staticModel, const kernel::Time_ABC& simulation );

    virtual ~LogisticListView();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const LogisticLinks& links );
    virtual const kernel::Entity_ABC* RetrieveSuperior( const kernel::Entity_ABC& entity ) const;
    virtual void SetSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC* superior );
    //@}

private:
    //! @name Member data
    //@{
    actions::ActionsModel&  actionsModel_;
    const StaticModel&      static_;
    const kernel::Time_ABC& simulation_;
    //@}
};

#endif // __LogisticListView_h_
