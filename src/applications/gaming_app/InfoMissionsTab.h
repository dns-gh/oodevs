// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoMissionsTab_h_
#define __InfoMissionsTab_h_

#include "clients_gui/ListDisplayer.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SelectionObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

namespace actions
{
    class Action_ABC;
    class Parameter_ABC;
}
class MissionParameters;

// =============================================================================
/** @class  InfoMissionsTab
    @brief  InfoMissionsTab
*/
// Created: SBO 2007-04-18
// =============================================================================
class InfoMissionsTab : public gui::ListDisplayer< InfoMissionsTab >
                      , public kernel::Observer_ABC
                      , public kernel::SelectionObserver< kernel::Entity_ABC >
                      , public kernel::ElementObserver_ABC< MissionParameters >
{

public:
    //! @name Constructors/Destructor
    //@{
             InfoMissionsTab( QTabWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~InfoMissionsTab();
    //@}

    //! @name Operations
    //@{
    virtual void Display( const actions::Action_ABC& action, kernel::Displayer_ABC& displayer, gui::ValuedListItem* item );
    virtual void Display( const actions::Parameter_ABC& param, kernel::Displayer_ABC& displayer, gui::ValuedListItem* item );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InfoMissionsTab( const InfoMissionsTab& );            //!< Copy constructor
    InfoMissionsTab& operator=( const InfoMissionsTab& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void AddColumn( const QString& column );
    virtual void NotifySelected( const kernel::Entity_ABC* entity );
    virtual void NotifyUpdated( const MissionParameters& extension );
    bool ShouldUpdate( const MissionParameters& extension ) const;
    virtual void showEvent( QShowEvent* event );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::ListItemDisplayer* sub_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    QTabWidget* parent_;
    //@}
};

#endif // __InfoMissionsTab_h_
