// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ActionProperties_h_
#define __ActionProperties_h_

#include "clients_kernel/SafePointer.h"
#include "tools/ElementObserver_ABC.h"
#include "tools/SelectionObserver_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
    class Displayer_ABC;
}

namespace actions
{
    class Action_ABC;
    class ActionTiming;
    class Parameter_ABC;
}

namespace gui
{
    class ItemFactory_ABC;
}

// =============================================================================
/** @class  ActionProperties
    @brief  ActionProperties
*/
// Created: SBO 2010-05-04
// =============================================================================
class ActionProperties : public QSplitter
                       , public tools::Observer_ABC
                       , public tools::ElementObserver_ABC< actions::ActionTiming >
                       , public tools::SelectionObserver< actions::Action_ABC >
                       , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ActionProperties( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~ActionProperties();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const actions::Action_ABC* action );
    virtual void NotifyUpdated( const actions::ActionTiming& timing );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< actions::Action_ABC > selected_;
    std::auto_ptr< kernel::Displayer_ABC > display_;
    //@}
};

#endif // __ActionProperties_h_
