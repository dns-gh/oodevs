// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationFire_h_
#define __PopulationFire_h_

#include "ASN_Types.h"
#include "Fire_ABC.h"
#include "Resolver_ABC.h"

class Population;

// =============================================================================
/** @class  PopulationFire
    @brief  PopulationFire
*/
// Created: AGE 2006-03-10
// =============================================================================
class PopulationFire : public Fire_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit PopulationFire( const ASN1T_MsgStartPopulationFire& message, const Resolver_ABC< Population >& resolver );
    virtual ~PopulationFire();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationFire( const PopulationFire& );            //!< Copy constructor
    PopulationFire& operator=( const PopulationFire& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    unsigned int id_;
    //@}
};

#endif // __PopulationFire_h_
