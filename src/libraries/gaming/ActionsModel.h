// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionsModel_h_
#define __ActionsModel_h_

#include "clients_kernel/Resolver.h"

namespace kernel
{
    class Entity_ABC;
    class Mission;
    class FragOrder;
}

class Action_ABC;
class ActionFactory_ABC;

// =============================================================================
/** @class  ActionsModel
    @brief  ActionsModel
*/
// Created: SBO 2007-03-12
// =============================================================================
class ActionsModel : public kernel::Resolver< Action_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionsModel( ActionFactory_ABC& factory );
    virtual ~ActionsModel();
    //@}

    //! @name Operations
    //@{
    Action_ABC* CreateAction( const kernel::Entity_ABC& target, const kernel::Mission& mission );
    Action_ABC* CreateAction( const kernel::Entity_ABC& target, const kernel::FragOrder& fragOrder );
    void Purge();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionsModel( const ActionsModel& );            //!< Copy constructor
    ActionsModel& operator=( const ActionsModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    ActionFactory_ABC& factory_;
    //@}
};

#endif // __ActionsModel_h_
