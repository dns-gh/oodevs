// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Borrowings_h_
#define __Borrowings_h_

#include "ASN_Types.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Resolver_ABC.h"
#include "Loan.h"

class Agent_ABC;
class Controller;
class EquipmentType;

// =============================================================================
/** @class  Borrowings
    @brief  Borrowings
*/
// Created: AGE 2006-02-13
// =============================================================================
class Borrowings : public Extension_ABC
                 , public Updatable_ABC< ASN1T_MsgUnitDotations >
{

public:
    //! @name Constructors/Destructor
    //@{
             Borrowings( Controller& controller, const Resolver_ABC< Agent_ABC >& resolver, const Resolver_ABC< EquipmentType >& equipmentResolver );
    virtual ~Borrowings();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Borrowings( const Borrowings& );            //!< Copy constructor
    Borrowings& operator=( const Borrowings& ); //!< Assignement operator
    //@}

    typedef std::vector< Loan > T_Borrowings;
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitDotations& message );
    //@}

public:
    //! @name Member data
    //@{
    Controller& controller_;
    const Resolver_ABC< Agent_ABC >& resolver_;
    const Resolver_ABC< EquipmentType >& equipmentResolver_;
    T_Borrowings borrowings_;
    //@}
};

#endif // __Borrowings_h_
