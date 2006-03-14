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

class Agent;
class Controller;
class Mission;

// =============================================================================
/** @class  AutomatDecisions
    @brief  AutomatDecisions
*/
// Created: AGE 2006-03-14
// =============================================================================
class AutomatDecisions : public Extension_ABC
                       , public Updatable_ABC< ASN1T_MsgUnitAttributes >
{

public:
    //! @name Constructors/Destructor
    //@{
             AutomatDecisions( Controller& controller, const Agent& agent );
    virtual ~AutomatDecisions();
    //@}

    //! @name Operations
    //@{
    const Agent& GetAgent() const;
    bool IsEmbraye() const; // $$$$ AGE 2006-03-14: 
    virtual Iterator< const Mission& > GetMissions() const;
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
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    const Agent& agent_;
    bool bEmbraye_;
    //@}
};

#endif // __AutomatDecisions_h_
