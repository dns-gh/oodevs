// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LinkInterpreter_h_
#define __LinkInterpreter_h_

#include "clients_gui/LinkInterpreter_ABC.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

class ProfileFilter;

// =============================================================================
/** @class  LinkInterpreter
    @brief  LinkInterpreter
*/
// Created: AGE 2006-08-11
// =============================================================================
class LinkInterpreter : public gui::LinkInterpreter_ABC
                      , public tools::Observer_ABC
                      , public tools::ElementObserver_ABC< kernel::Entity_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             LinkInterpreter( QObject* parent, kernel::Controllers& controllers, ProfileFilter& filter );
    virtual ~LinkInterpreter();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LinkInterpreter( const LinkInterpreter& );            //!< Copy constructor
    LinkInterpreter& operator=( const LinkInterpreter& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual bool InterpreteEntity( const kernel::Entity_ABC& entity, const QString& action );
    virtual void NotifyCreated( const kernel::Entity_ABC& entity );
    virtual void NotifyDeleted( const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    ProfileFilter& filter_;
    //@}
};

#endif // __LinkInterpreter_h_
