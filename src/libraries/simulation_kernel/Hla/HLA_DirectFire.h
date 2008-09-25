// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-24 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_DirectFire.h $
// $Author: Age $
// $Modtime: 29/11/04 17:18 $
// $Revision: 3 $
// $Workfile: HLA_DirectFire.h $
//
// *****************************************************************************

#ifndef __HLA_DirectFire_h_
#define __HLA_DirectFire_h_

#include "HLA_Interaction_ABC.h"
#include "HLA_TargetedInteraction.h"
#include "HLA_FireInteraction.h"

namespace hla
{
    template< typename T > class Interaction;
    template< typename T > class InteractionNotification_ABC;
}

class PHY_Composante_ABC;
class PHY_FireResults_ABC;

// =============================================================================
/** @class  HLA_DirectFire
    @brief  HLA direct fire interaction
*/
// Created: AGE 2004-11-24
// =============================================================================
class HLA_DirectFire : public HLA_Interaction_ABC, public HLA_TargetedInteraction, public HLA_FireInteraction
{

public:
    //! @name Constructors/Destructor
    //@{
             HLA_DirectFire();
             HLA_DirectFire( const HLA_RoleInterface& role, const PHY_Composante_ABC& composante, const PHY_DotationCategory& ammunition );
    virtual ~HLA_DirectFire();
    //@}

    //! @name Operations
    //@{
    static hla::Interaction< HLA_DirectFire >& CreateInteractionClass( hla::InteractionNotification_ABC< HLA_DirectFire >& callback );

    virtual void Execute() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< PHY_Composante_ABC* > T_ComposanteVector;
    //@}

    //! @name Helpers
    //@{
    void ApplyFire( const PHY_DotationCategory& ammunition, MIL_Agent_ABC& target, const std::string& strTargetComposante, const T_ComposanteVector& allComposantes, PHY_FireResults_ABC& fireResult ) const;
    //@}

    //! @name Member data
    //@{
    std::string strTargetComposante_;
    //@}
};

#endif // __HLA_DirectFire_h_
