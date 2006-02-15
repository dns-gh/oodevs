// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Lends_h_
#define __Lends_h_

#include "ASN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "Resolver_ABC.h"

class Agent;
class Controller;

// =============================================================================
/** @class  Lends
    @brief  Lends
*/
// Created: AGE 2006-02-13
// =============================================================================
class Lends : public Extension_ABC
            , public Updatable_ABC< ASN1T_MsgUnitDotations >
{

public:
    //! @name Constructors/Destructor
    //@{
             Lends( Controller& controller, const Resolver_ABC< Agent >& resolver );
    virtual ~Lends();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Lends( const Lends& );            //!< Copy constructor
    Lends& operator=( const Lends& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    struct Lend
    {
        const Agent* borrower_;
        unsigned long nEquipment_;
        int nQuantity_;
    };
    typedef std::vector< Lend > T_Lends;
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitDotations& message );
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    const Resolver_ABC< Agent >& resolver_;
    T_Lends lends_;
    //@}
};

#endif // __Lends_h_
