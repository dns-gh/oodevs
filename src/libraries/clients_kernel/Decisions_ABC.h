// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Decisions_ABC_h_
#define __Decisions_ABC_h_

#include <tools/Resolver.h>

namespace kernel
{
    class Mission;
    class FragOrder;
    class Entity_ABC;

// =============================================================================
/** @class  Decisions_ABC
    @brief  Decisions_ABC
*/
// Created: AGE 2007-04-04
// =============================================================================
class Decisions_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Decisions_ABC() {};
    virtual ~Decisions_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual bool CanBeOrdered() const = 0;

    virtual tools::Iterator< const kernel::Mission& > GetMissions() const = 0;
    virtual tools::Iterator< const kernel::FragOrder& > GetFragOrders() const = 0;
    virtual const kernel::Mission* GetCurrentMission() const = 0;
    virtual const kernel::Entity_ABC& GetAgent() const = 0;
    //@}
};

} //! namespace kernel

#endif // __Decisions_ABC_h_
