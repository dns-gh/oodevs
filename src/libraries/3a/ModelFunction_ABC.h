// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ModelFunction_ABC_h_
#define __ModelFunction_ABC_h_

#include "game_asn/asn.h"

// =============================================================================
/** @class  ModelFunction_ABC
    @brief  Model function definition
*/
// Created: AGE 2007-08-29
// =============================================================================
class ModelFunction_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ModelFunction_ABC() {};
    virtual ~ModelFunction_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void BeginTick() = 0;
    virtual void Receive( const ASN1T_MsgsSimToClient& message ) = 0;
    virtual void EndTick() = 0;
    //@}
};

#endif // __ModelFunction_ABC_h_
