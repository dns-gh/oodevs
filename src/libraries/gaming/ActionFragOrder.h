// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionFragOrder_h_
#define __ActionFragOrder_h_

#include "Action_ABC.h"

namespace kernel
{
    class FragOrder;
    class Controller;
}

// =============================================================================
/** @class  ActionFragOrder
    @brief  ActionFragOrder
*/
// Created: SBO 2007-03-19
// =============================================================================
class ActionFragOrder : public Action_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionFragOrder( const kernel::Entity_ABC& entity, const kernel::FragOrder& fragOrder, kernel::Controller& controller );
    virtual ~ActionFragOrder();
    //@}

    //! @name Operations
    //@{
    virtual QString GetName() const;
    virtual const kernel::Entity_ABC& GetEntity() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionFragOrder( const ActionFragOrder& );            //!< Copy constructor
    ActionFragOrder& operator=( const ActionFragOrder& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Entity_ABC& entity_;
    const kernel::FragOrder& fragOrder_;
    //@}
};

#endif // __ActionFragOrder_h_
