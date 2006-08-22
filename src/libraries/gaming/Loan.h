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

namespace kernel
{
    class EquipmentType;
    class Agent_ABC;
}

// =============================================================================
/** @class  Loan
    @brief  Loan
*/
// Created: AGE 2006-02-21
// =============================================================================
class Loan
{

public:
    //! @name Constructors/Destructor
    //@{
             Loan( const kernel::EquipmentType& type, const kernel::Agent_ABC& agent, unsigned int quantity );
    virtual ~Loan();
    //@}

public:
    //! @name Member data
    //@{
    const kernel::EquipmentType* type_;
    const kernel::Agent_ABC* agent_;
    unsigned int quantity_;
    //@}
};

#endif // __Loan_h_
