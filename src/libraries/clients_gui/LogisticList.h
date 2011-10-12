// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __gui_LogisticList_h_
#define __gui_LogisticList_h_

#include <boost/noncopyable.hpp>
#include "EntitySearchBox.h"
#include "clients_kernel/Entity_ABC.h"

namespace actions
{
    class ActionsModel;
}

namespace kernel
{
    class Controllers;
    class Profile_ABC;
    class Time_ABC;
}

class StaticModel;
class ModelBuilder;

namespace gui
{
    class ItemFactory_ABC;
    class EntitySymbols;

// =============================================================================
/** @class  LogisticList
    @brief  LogisticList
*/
// Created: ABR 2011-09-16
// =============================================================================
template< typename CurrentLogisticListView >
class LogisticList : public Q3VBox
                   , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticList( kernel::Controllers& controllers, ItemFactory_ABC& factory, const kernel::Profile_ABC& profile,
                           const EntitySymbols& symbols, ModelBuilder& modelBuilder );
             LogisticList( kernel::Controllers& controllers, ItemFactory_ABC& factory, const kernel::Profile_ABC& profile, 
                           const EntitySymbols& symbols, actions::ActionsModel& actionsModel,
                           const ::StaticModel& staticModel, const kernel::Time_ABC& simulation );
    virtual ~LogisticList();
    //@}

    //! @name Operations
    //@{
    void Purge();
    //@}

private:
    //! @name Member data
    //@{
    CurrentLogisticListView* listView_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: LogisticList constructor
// Created: ABR 2011-09-16
// -----------------------------------------------------------------------------
template< typename CurrentLogisticListView >
LogisticList< CurrentLogisticListView >::LogisticList( kernel::Controllers& controllers, ItemFactory_ABC& factory, const kernel::Profile_ABC& profile,
                                                       const EntitySymbols& symbols, ModelBuilder& modelBuilder )
{
    new EntitySearchBox< kernel::Entity_ABC >( this, controllers );
    listView_ = new CurrentLogisticListView( this, controllers, factory, profile, symbols, modelBuilder );
}

// -----------------------------------------------------------------------------
// Name: LogisticList constructor
// Created: ABR 2011-09-19
// -----------------------------------------------------------------------------
template< typename CurrentLogisticListView >
LogisticList< CurrentLogisticListView >::LogisticList( kernel::Controllers& controllers, ItemFactory_ABC& factory, const kernel::Profile_ABC& profile,
                                                       const EntitySymbols& symbols, actions::ActionsModel& actionsModel,
                                                       const ::StaticModel& staticModel, const kernel::Time_ABC& simulation )
{
    new EntitySearchBox< kernel::Entity_ABC >( this, controllers );
    listView_ = new CurrentLogisticListView( this, controllers, factory, profile, symbols, actionsModel, staticModel, simulation );
}

// -----------------------------------------------------------------------------
// Name: LogisticList destructor
// Created: ABR 2011-09-16
// -----------------------------------------------------------------------------
template< typename CurrentLogisticListView >
LogisticList< CurrentLogisticListView >::~LogisticList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticList::Purge
// Created: JSR 2011-10-12
// -----------------------------------------------------------------------------
template< typename CurrentLogisticListView >
inline
void LogisticList< CurrentLogisticListView >::Purge()
{
    listView_->Purge();
}

}

#endif // __gui_LogisticList_h_
