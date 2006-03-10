// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IndirectFire_h_
#define __IndirectFire_h_

#include "ASN_Types.h"
#include "Fire_ABC.h"
#include "Resolver_ABC.h"

class Agent;

// =============================================================================
/** @class  IndirectFire
    @brief  IndirectFire
*/
// Created: AGE 2006-03-10
// =============================================================================
class IndirectFire : public Fire_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             IndirectFire( const ASN1T_MsgStartPionFire& message, const Resolver_ABC< Agent >& resolver );
    virtual ~IndirectFire();
    //@}

private:
    //! @name Member data
    //@{
    unsigned int id_;
    MT_Vector2D target_;
    //@}
};

#endif // __IndirectFire_h_
