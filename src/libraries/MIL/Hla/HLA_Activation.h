// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-12-06 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_Activation.h $
// $Author: Age $
// $Modtime: 6/12/04 16:39 $
// $Revision: 1 $
// $Workfile: HLA_Activation.h $
//
// *****************************************************************************

#ifndef __HLA_Activation_h_
#define __HLA_Activation_h_

#include "HLA_Interaction_ABC.h"
#include "HLA_ObjectInteraction.h"

template< typename T > class Interaction;
template< typename T > class InteractionNotification_ABC;

// =============================================================================
/** @class  HLA_Activation
    @brief  HLA activation interaction
*/
// Created: AGE 2004-12-06
// =============================================================================
class HLA_Activation : public HLA_Interaction_ABC, public HLA_ObjectInteraction
{

public:
    //! @name Constructors/Destructor
    //@{
             HLA_Activation();
             HLA_Activation( const HLA_Object_ABC& targetObject, bool bActivate );
    virtual ~HLA_Activation();
    //@}

    //! @name Operations
    //@{
    virtual void Execute() const;

    static Interaction< HLA_Activation >& CreateInteractionClass( InteractionNotification_ABC< HLA_Activation >& callback );
    //@}

private:
    //! @name Member data
    //@{
    bool bActivate_;
    //@}
};

#endif // __HLA_Activation_h_
