// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Controllers_h_
#define __Controllers_h_

namespace kernel
{
    class Options;
    class Controller;
    class ActionController;
    class Observer_ABC;

// =============================================================================
/** @class  Controllers
    @brief  Controllers
*/
// Created: AGE 2006-03-22
// =============================================================================
class Controllers
{

public:
    //! @name Constructors/Destructor
    //@{
             Controllers();
    virtual ~Controllers();
    //@}

    //! @name Operations
    //@{
    void Register( Observer_ABC& observer );
    void Remove  ( Observer_ABC& observer );
    void Update  ( Observer_ABC& observer );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Controllers( const Controllers& );
    Controllers& operator=( const Controllers& );
    //@}

public:
    //! @name Member data
    //@{
    Options&          options_;
    Controller&       controller_;
    ActionController& actions_;
    //@}
};

}

#endif // __Controllers_h_
