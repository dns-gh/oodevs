// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AutomatDecisions_h_
#define __AutomatDecisions_h_

#include "Decisions_ABC.h"
#include "clients_kernel/AutomatDecisions_ABC.h"
#include "clients_kernel/Displayable_ABC.h"
#include "clients_kernel/Updatable_ABC.h"

namespace kernel
{
    class Automat_ABC;
    class Controller;
    class Displayer_ABC;
    class DecisionalModel;
    class AutomatType;
}

namespace sword
{
    class AutomatAttributes;
    class AutomatOrder;
}

class Publisher_ABC;

// =============================================================================
/** @class  AutomatDecisions
    @brief  Automat decisions
*/
// Created: AGE 2006-03-14
// =============================================================================
class AutomatDecisions : public kernel::AutomatDecisions_ABC
                       , public kernel::Updatable_ABC< sword::AutomatAttributes >
                       , public kernel::Updatable_ABC< sword::AutomatOrder >
                       , public kernel::Displayable_ABC
                       , public Decisions_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AutomatDecisions( kernel::Controller& controller, Publisher_ABC& publisher, const kernel::Automat_ABC& agent,
                               const tools::Resolver_ABC< kernel::DecisionalModel, std::string >& modelResolver,
                               const kernel::AutomatType& type );
    virtual ~AutomatDecisions();
    //@}

    //! @name Operations
    //@{
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;

    virtual bool CanBeOrdered() const;
    virtual tools::Iterator< const kernel::Mission& > GetMissions() const;
    virtual tools::Iterator< const kernel::FragOrder& > GetFragOrders() const;
    virtual const kernel::Mission* GetCurrentMission() const;
    virtual const kernel::Entity_ABC& GetAgent() const;

    bool IsEmbraye() const; // $$$$ AGE 2006-03-14:
    bool IsDebugActivated() const;

    std::string ModelName() const;

    void Engage();
    void Disengage();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::AutomatAttributes& message );
    virtual void DoUpdate( const sword::AutomatOrder& message );
    bool HasEngagedSuperior() const;
    const kernel::DecisionalModel& GetDecisionalModel() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    Publisher_ABC& publisher_;
    const kernel::Automat_ABC& agent_;
    const tools::Resolver_ABC< kernel::DecisionalModel, std::string >& modelResolver_;
    const kernel::DecisionalModel* model_;
    bool brainDebug_;
    bool bEmbraye_;
    const kernel::Mission* current_;
    //@}
};

#endif // __AutomatDecisions_h_
