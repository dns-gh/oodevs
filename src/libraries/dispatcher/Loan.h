 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Loan_h_
#define __Loan_h_

#include "game_asn/Asn.h"

namespace dispatcher
{
    class Model;
    class Agent;

// =============================================================================
/** @class  Loan
    @brief  Loan
*/
// Created: NLD 2006-09-19
// =============================================================================
class Loan
{
public:
    //! @name Constructors/Destructor
    //@{
     Loan( const Model& model, ASN1T_BorrowedEquipment& asnMsg );
     Loan( const Model& model, ASN1T_LentEquipment&    asnMsg );
    ~Loan();
    //@}

    //! @name Operations
    //@{
    void Send( ASN1T_BorrowedEquipment& asnMsg ) const;
    void Send( ASN1T_LentEquipment&    asnMsg ) const;
    //@}

private:
    Agent*       pAgent_;         // Borrower / lender
    unsigned int nEquipmentType_; // XML reference - no resolved by dispatcher
    unsigned int nQuantity_;
};

}

#endif // __Loan_h_
