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

#include "network/Simulation_Asn.h"

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
     Loan( const Model& model, ASN1T_EquipementEmprunte& asnMsg );
     Loan( const Model& model, ASN1T_EquipementPrete&    asnMsg );
    ~Loan();
    //@}

    //! @name Operations
    //@{
    void Send( ASN1T_EquipementEmprunte& asnMsg ) const;
    void Send( ASN1T_EquipementPrete&    asnMsg ) const;
    //@}

private:
    Agent*       pAgent_;         // Borrower / lender
    unsigned int nEquipmentType_; // XML reference - no resolved by dispatcher
    unsigned int nQuantity_;
};

}

#endif // __Loan_h_
