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

namespace gui
{
    class ItemFactory_ABC;
    class EntitySymbols;
}

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

class LogisticBaseStates;
class LogisticLevelAttritube;
class ModelBuilder;

// =============================================================================
/** @class  LogisticListView
    @brief  LogisticListView
*/
// Created: ABR 2011-09-14
// =============================================================================
class LogisticListView : public gui::LogisticListView
                       , public tools::ElementObserver_ABC< kernel::Entity_ABC >
                       , public tools::ElementObserver_ABC< LogisticBaseStates >
                       , public tools::ElementObserver_ABC< LogisticLevelAttritube >
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const kernel::Profile_ABC& profile,
                               const gui::EntitySymbols& symbols, ModelBuilder& modelBuilder );
    virtual ~LogisticListView();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::Entity_ABC& entity );
    virtual void NotifyUpdated( const LogisticBaseStates& hierarchy );
    virtual void NotifyUpdated( const LogisticLevelAttritube& hierarchy );
    virtual const kernel::Entity_ABC* RetrieveSuperior( const kernel::Entity_ABC& entity ) const;
    virtual void SetSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC* superior );
    virtual void keyPressEvent( QKeyEvent* event );
    //@}

private:
    //! @name Member Data
    //@{
    ModelBuilder& modelBuilder_;
    //@}
};

#endif // __LogisticListView_h_
