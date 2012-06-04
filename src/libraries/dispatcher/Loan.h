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

#include <boost/noncopyable.hpp>

namespace sword
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
    class Model_ABC;

// =============================================================================
/** @class  Loan
    @brief  Loan
*/
// Created: NLD 2006-09-19
// =============================================================================
class Loan : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Loan( const Model_ABC& model, const sword::BorrowedEquipments_BorrowedEquipment& message );
             Loan( const Model_ABC& model, const sword::LentEquipments_LentEquipment& message );
    virtual ~Loan();
    //@}

    //! @name Operations
    //@{
    void Send( sword::BorrowedEquipments_BorrowedEquipment& message ) const;
    void Send( sword::LentEquipments_LentEquipment& message ) const;
    //@}

private:
    int idAgent_;         // Borrower / lender
    unsigned int equipmentType_; // XML reference - no resolved by dispatcher
    unsigned int quantity_;
    int nCounter_;
};

}

#endif // __Loan_h_
