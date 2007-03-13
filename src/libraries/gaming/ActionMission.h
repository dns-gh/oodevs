// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionMission_h_
#define __ActionMission_h_

#include "Action_ABC.h"

namespace kernel
{
    class Entity_ABC;
    class Mission;
}

// =============================================================================
/** @class  ActionMission
    @brief  ActionMission
*/
// Created: SBO 2007-03-12
// =============================================================================
class ActionMission : public Action_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionMission( kernel::Entity_ABC& target, const kernel::Mission& mission, kernel::Controller& controller );
    virtual ~ActionMission();
    //@}

    //! @name Operations
    //@{
    virtual QString GetName() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionMission( const ActionMission& );            //!< Copy constructor
    ActionMission& operator=( const ActionMission& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    kernel::Entity_ABC& target_;
    const kernel::Mission& mission_;
    //@}
};

#endif // __ActionMission_h_
