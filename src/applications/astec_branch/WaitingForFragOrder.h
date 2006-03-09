// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __WaitingForFragOrder_h_
#define __WaitingForFragOrder_h_

#include "ASN_Types.h"
#include "Report_ABC.h"
class Agent_ABC;
class Simulation;

// =============================================================================
/** @class  WaitingForFragOrder
    @brief  WaitingForFragOrder
*/
// Created: AGE 2006-03-09
// =============================================================================
class WaitingForFragOrder : public Report_ABC
{

public:
    //! @name Types
    //@{
    typedef std::vector< uint >            T_FragOrders;
    typedef T_FragOrders::iterator        IT_FragOrders;
    typedef T_FragOrders::const_iterator CIT_FragOrders;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             WaitingForFragOrder( const Agent_ABC& agent, const Simulation& simulation, const ASN1T_MsgAttenteOrdreConduite& asnMsg );
    virtual ~WaitingForFragOrder();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    WaitingForFragOrder( const WaitingForFragOrder& );            //!< Copy constructor
    WaitingForFragOrder& operator=( const WaitingForFragOrder& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void Initialize( const ASN1T_MsgAttenteOrdreConduite& asnMsg );
    //@}

private:
    //! @name Member data
    //@{
    const Agent_ABC& agent_;
    T_FragOrders fragOrders_; // $$$$ AGE 2006-03-09: resolve !
    //@}
};

#endif // __WaitingForFragOrder_h_
