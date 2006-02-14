// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Troops_h_
#define __Troops_h_

#include "ASN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"

class Controller;

// =============================================================================
/** @class  Troops
    @brief  Troops
*/
// Created: AGE 2006-02-13
// =============================================================================
class Troops : public Extension_ABC
             , public Updatable_ABC< ASN1T_MsgUnitDotations >
{

public:
    //! @name Constructors/Destructor
    //@{
             Troops( Controller& controller );
    virtual ~Troops();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Troops( const Troops& );            //!< Copy constructor
    Troops& operator=( const Troops& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void Update( const ASN1T_MsgUnitDotations& message );
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    unsigned officers_   [eTroopHealthStateNbrStates];
    unsigned subOfficers_[eTroopHealthStateNbrStates];
    unsigned troopers_   [eTroopHealthStateNbrStates];
    //@}
};

#endif // __Troops_h_
