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

#include "ASN_Types.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Resolver.h"
#include "clients_kernel/Displayable_ABC.h"

namespace kernel
{
    class Automat_ABC;
    class Controller;
    class Mission;
    class FragOrder;
    class Displayer_ABC;
    class DecisionalModel;
}

class Publisher_ABC;

// =============================================================================
/** @class  AutomatDecisions
    @brief  Automat decisions
*/
// Created: AGE 2006-03-14
// =============================================================================
class AutomatDecisions : public kernel::Extension_ABC
                       , public kernel::Updatable_ABC< ASN1T_MsgAutomateAttributes >
                       , public kernel::Updatable_ABC< ASN1T_MsgAutomateOrder >
                       , public kernel::Updatable_ABC< ASN1T_MsgAutomateOrderAck >
                       , public kernel::Displayable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AutomatDecisions( kernel::Controller& controller, Publisher_ABC& publisher, const kernel::Automat_ABC& agent );
    virtual ~AutomatDecisions();
    //@}

    //! @name Operations
    //@{
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;

    const kernel::Automat_ABC& GetAgent() const; // $$$$ AGE 2006-10-06: 
    bool IsEmbraye() const; // $$$$ AGE 2006-03-14: 

    virtual kernel::Iterator< const kernel::Mission& > GetMissions() const;
    virtual kernel::Iterator< const kernel::FragOrder& > GetFragOrders() const;
    const kernel::Mission* GetCurrentMission() const;
    void Engage() const;
    void Disengage() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AutomatDecisions( const AutomatDecisions& );            //!< Copy constructor
    AutomatDecisions& operator=( const AutomatDecisions& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgAutomateAttributes& message );
    virtual void DoUpdate( const ASN1T_MsgAutomateOrder& message );
    virtual void DoUpdate( const ASN1T_MsgAutomateOrderAck& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    Publisher_ABC& publisher_;
    const kernel::Automat_ABC& agent_;
    const kernel::DecisionalModel& model_;
    bool bEmbraye_;

    long lastOrderId_;
    const kernel::Mission* current_;
    const kernel::Mission* next_;
    //@}
};

#endif // __AutomatDecisions_h_
