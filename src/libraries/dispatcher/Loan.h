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

namespace MsgsSimToClient
{
    class BorrowedEquipments_BorrowedEquipment;
    class LentEquipments_LentEquipment;
}

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
     Loan( const Model& model, const MsgsSimToClient::BorrowedEquipments_BorrowedEquipment& message );
     Loan( const Model& model, const MsgsSimToClient::LentEquipments_LentEquipment&         message );
    virtual ~Loan();
    //@}

    //! @name Operations
    //@{
    void Send( MsgsSimToClient::BorrowedEquipments_BorrowedEquipment&   message ) const;
    void Send( MsgsSimToClient::LentEquipments_LentEquipment&           message ) const;
    //@}

private:
    const kernel::Agent_ABC* agent_;         // Borrower / lender
    unsigned int equipmentType_; // XML reference - no resolved by dispatcher
    unsigned int quantity_;
};

}

#endif // __Loan_h_
