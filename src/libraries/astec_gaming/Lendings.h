// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Lendings_h_
#define __Lendings_h_

#include "ASN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "Resolver_ABC.h"
#include "Loan.h"

class Agent_ABC;
class Controller;
class EquipmentType;

// =============================================================================
/** @class  Lendings
    @brief  Lendings
*/
// Created: AGE 2006-02-13
// =============================================================================
class Lendings : public Extension_ABC
               , public Updatable_ABC< ASN1T_MsgUnitDotations >
{

public:
    //! @name Constructors/Destructor
    //@{
             Lendings( Controller& controller, const Resolver_ABC< Agent_ABC >& resolver, const Resolver_ABC< EquipmentType >& equipmentResolver );
    virtual ~Lendings();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Lendings( const Lendings& );            //!< Copy constructor
    Lendings& operator=( const Lendings& ); //!< Assignement operator
    //@}

    typedef std::vector< Loan > T_Lendings;
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
    T_Lendings lendings_;
    //@}
};

#endif // __Lendings_h_
