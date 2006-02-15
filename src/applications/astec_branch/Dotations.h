// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Dotations_h_
#define __Dotations_h_

#include "ASN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"

class Controller;

// =============================================================================
/** @class  Dotations
    @brief  Dotations. Resources actually
*/
// Created: AGE 2006-02-13
// =============================================================================
class Dotations : public Extension_ABC
                , public Updatable_ABC< ASN1T_MsgUnitDotations >
{

public:
    //! @name Constructors/Destructor
    //@{
             Dotations( Controller& controller );
    virtual ~Dotations();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Dotations( const Dotations& );            //!< Copy constructor
    Dotations& operator=( const Dotations& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitDotations& message );
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned long, unsigned > T_Dotations;
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    bool bEmptyGasTank_;
    T_Dotations dotations_;
    //@}
};

#endif // __Dotations_h_
