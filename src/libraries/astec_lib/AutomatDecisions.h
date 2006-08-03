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
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "Resolver.h"

class Agent_ABC;
class Controller;
class Mission;
class FragOrder;
class Displayer_ABC;
class Publisher_ABC;
class DecisionalModel;

// =============================================================================
/** @class  AutomatDecisions
    @brief  AutomatDecisions
*/
// Created: AGE 2006-03-14
// =============================================================================
class AutomatDecisions : public Extension_ABC
                       , public Updatable_ABC< ASN1T_MsgUnitAttributes >
                       , public Updatable_ABC< ASN1T_MsgAutomateOrder >
                       , public Updatable_ABC< ASN1T_MsgAutomateOrderAck >
{

public:
    //! @name Constructors/Destructor
    //@{
             AutomatDecisions( Controller& controller, Publisher_ABC& publisher, const Agent_ABC& agent );
    virtual ~AutomatDecisions();
    //@}

    //! @name Operations
    //@{
    void DisplayInTooltip( Displayer_ABC& displayer ) const;

    const Agent_ABC& GetAgent() const;
    bool IsEmbraye() const; // $$$$ AGE 2006-03-14: 
    virtual Iterator< const Mission& > GetMissions() const;
    virtual Iterator< const FragOrder& > GetFragOrders() const;
    const Mission* GetCurrentMission() const;
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
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& message );
    virtual void DoUpdate( const ASN1T_MsgAutomateOrder& message );
    virtual void DoUpdate( const ASN1T_MsgAutomateOrderAck& message );

    const DecisionalModel& GetAutomatDecisionalModel() const;
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    Publisher_ABC& publisher_;
    const Agent_ABC& agent_;
    bool bEmbraye_;

    unsigned long lastOrderId_;
    const Mission* current_;
    const Mission* next_;
    //@}
};

#endif // __AutomatDecisions_h_
