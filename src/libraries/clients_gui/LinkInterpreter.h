// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef gui_LinkInterpreter_h_
#define gui_LinkInterpreter_h_

#include "LinkInterpreter_ABC.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

namespace gui
{
// =============================================================================
/** @class  LinkInterpreter
    @brief  LinkInterpreter
*/
// Created: AGE 2006-08-11
// =============================================================================
class LinkInterpreter : public LinkInterpreter_ABC
                      , public tools::Observer_ABC
                      , public tools::ElementObserver_ABC< kernel::Entity_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             LinkInterpreter( QObject* parent, kernel::Controllers& controllers );
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
    virtual void NotifyCreated( const kernel::Entity_ABC& entity );
    virtual void NotifyDeleted( const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};
}

#endif // gui_LinkInterpreter_h_
