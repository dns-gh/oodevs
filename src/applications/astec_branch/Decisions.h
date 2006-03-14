// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Decisions_h_
#define __Decisions_h_

#include "ASN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "Resolver.h"

class Agent;
class Controller;
class Mission;

// =============================================================================
/** @class  Decisions
    @brief  Decisions
*/
// Created: AGE 2006-03-14
// =============================================================================
class Decisions : public Extension_ABC
                , public Updatable_ABC< ASN1T_MsgUnitAttributes >
{

public:
    //! @name Constructors/Destructor
    //@{
             Decisions( Controller& controller, const Agent& agent );
    virtual ~Decisions();
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
    Decisions( const Decisions& );            //!< Copy constructor
    Decisions& operator=( const Decisions& ); //!< Assignement operator
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

#endif // __Decisions_h_
