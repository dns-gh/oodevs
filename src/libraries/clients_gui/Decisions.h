// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __Decisions_h_
#define __Decisions_h_

#include "Decisions_ABC.h"

namespace kernel
{
    class Controller;
    class DecisionalModel;
    class Entity_ABC;
}

namespace gui
{

// =============================================================================
/** @class  Decisions
    @brief  Decisions
*/
// Created: ABR 2013-12-18
// =============================================================================
class Decisions : public Decisions_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Decisions( kernel::Controller& controller,
                        const kernel::Entity_ABC& agent,
                        const kernel::DecisionalModel& decisionalModel );
    virtual ~Decisions();
    //@}

    //! @name Decisions_ABC partial implementation
    //@{
    virtual bool IsDebugActivated() const;
    virtual tools::Iterator< const kernel::Mission& > GetMissions() const;
    virtual tools::Iterator< const kernel::FragOrder& > GetFragOrders() const;
    virtual const kernel::Mission* GetCurrentMission() const;
    virtual const kernel::Entity_ABC& GetAgent() const;
    virtual const std::string& GetModelName() const;
    //@}

protected:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::DecisionalModel* model_;
    const kernel::Entity_ABC& entity_;
    const kernel::Mission* currentMission_;
    bool brainDebug_;
    //@}
};

} //! namespace gui

#endif // __Decisions_h_
