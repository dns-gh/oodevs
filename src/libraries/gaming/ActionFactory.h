// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionFactory_h_
#define __ActionFactory_h_

#include "ActionFactory_ABC.h"

namespace kernel
{
    class Controllers;
}

// =============================================================================
/** @class  ActionFactory
    @brief  ActionFactory
*/
// Created: SBO 2007-03-12
// =============================================================================
class ActionFactory : public ActionFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionFactory( kernel::Controllers& controllers );
    virtual ~ActionFactory();
    //@}

    //! @name Operations
    //@{
    virtual Action_ABC* CreateAction( const kernel::Entity_ABC& target, const kernel::Mission& mission ) const;
    virtual Action_ABC* CreateAction( const kernel::Entity_ABC& target, const kernel::FragOrder& fragOrder ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionFactory( const ActionFactory& );            //!< Copy constructor
    ActionFactory& operator=( const ActionFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};

#endif // __ActionFactory_h_
