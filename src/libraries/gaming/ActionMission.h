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
    class Mission;
    class Controller;
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
             ActionMission( const kernel::Entity_ABC& entity, const kernel::Mission& mission, kernel::Controller& controller, bool registered = true );
    virtual ~ActionMission();
    //@}

    //! @name Operations
    //@{
    virtual QString GetName() const;
    virtual const kernel::Entity_ABC& GetEntity() const;
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
    kernel::Controller& controller_;
    const kernel::Entity_ABC& entity_;
    const kernel::Mission& mission_;
    bool registered_;
    //@}
};

#endif // __ActionMission_h_
