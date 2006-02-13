// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Automat_h_
#define __Automat_h_

#include "Agent.h"

// =============================================================================
/** @class  Automat
    @brief  Automat
*/
// Created: AGE 2006-02-13
// =============================================================================
class Automat : public Agent
{

public:
    //! @name Constructors/Destructor
    //@{
             Automat();
    virtual ~Automat();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Automat( const Automat& );            //!< Copy constructor
    Automat& operator=( const Automat& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __Automat_h_
