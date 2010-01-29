 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Loan_h_
#define __Loan_h_

#include "game_asn/Simulation.h"

namespace kernel
{
    class Agent_ABC;
}

namespace dispatcher
{
    class Model;

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
     Loan( const Model& model, const ASN1T_BorrowedEquipment& asnMsg );
     Loan( const Model& model, const ASN1T_LentEquipment&    asnMsg );
    virtual ~Loan();
    //@}

    //! @name Operations
    //@{
    void Send( ASN1T_BorrowedEquipment& asnMsg ) const;
    void Send( ASN1T_LentEquipment&    asnMsg ) const;
    //@}

private:
    const kernel::Agent_ABC* agent_;         // Borrower / lender
    unsigned int equipmentType_; // XML reference - no resolved by dispatcher
    unsigned int quantity_;
};

}

#endif // __Loan_h_
