// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ActionWithTarget_ABC_h_
#define __ActionWithTarget_ABC_h_

#include "Action_ABC.h"

namespace actions {

// =============================================================================
/** @class  ActionWithTarget_ABC
    @brief  ActionWithTarget_ABC
*/
// Created: JSR 2010-04-02
// =============================================================================
class ActionWithTarget_ABC : public Action_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ActionWithTarget_ABC( kernel::Controller& controller, const kernel::OrderType& type, const kernel::Entity_ABC& target );
             ActionWithTarget_ABC( xml::xistream& xis, kernel::Controller& controller, const kernel::OrderType& type, const kernel::Entity_ABC& target );
    virtual ~ActionWithTarget_ABC();
    //@}

    //! @name Member
    //@{
    unsigned int GetEntityId() const { return entityId_; }
    //@}

protected:
    //! @name Member data
    //@{
    const unsigned int entityId_;
    const std::string entityTypeName_;
    //@}
};

} // end namespace actions

#endif // __ActionWithTarget_ABC_h_
