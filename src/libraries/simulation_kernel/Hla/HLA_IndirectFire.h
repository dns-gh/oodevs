// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-29 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_IndirectFire.h $
// $Author: Age $
// $Modtime: 29/11/04 17:21 $
// $Revision: 2 $
// $Workfile: HLA_IndirectFire.h $
//
// *****************************************************************************

#ifndef __HLA_IndirectFire_h_
#define __HLA_IndirectFire_h_

#include "HLA_Interaction_ABC.h"
#include "HLA_TargetedInteraction.h"
#include "HLA_FireInteraction.h"

namespace hla
{
    template< typename T > class Interaction;
    template< typename T > class InteractionNotification_ABC;
}

// =============================================================================
/** @class  HLA_IndirectFire
    @brief  HLA indirect fire interaction
*/
// Created: AGE 2004-11-29
// =============================================================================
class HLA_IndirectFire : public HLA_Interaction_ABC, public HLA_TargetedInteraction, public HLA_FireInteraction
{

public:
    //! @name Constructors/Destructor
    //@{
             HLA_IndirectFire();
             HLA_IndirectFire( const HLA_RoleInterface& role, const PHY_DotationCategory& ammunition );
    virtual ~HLA_IndirectFire();
    //@}

    //! @name Operations
    //@{
    static hla::Interaction< HLA_IndirectFire >& CreateInteractionClass( hla::InteractionNotification_ABC< HLA_IndirectFire >& callback );

    virtual void Execute() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    HLA_IndirectFire( const HLA_IndirectFire& );            //!< Copy constructor
    HLA_IndirectFire& operator=( const HLA_IndirectFire& ); //!< Assignement operator
    //@}
};

#endif // __HLA_IndirectFire_h_
