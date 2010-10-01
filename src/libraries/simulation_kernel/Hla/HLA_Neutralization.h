// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __HLA_Neutralization_h_
#define __HLA_Neutralization_h_

#include "HLA_TargetedInteraction.h"
#include "HLA_Interaction_ABC.h"

namespace hla
{
    template< typename T > class Interaction;
    template< typename T > class InteractionNotification_ABC;
}

// =============================================================================
/** @class  HLA_Neutralization
    @brief  HLA neutralization interaction
*/
// Created: AGE 2004-12-02
// =============================================================================
class HLA_Neutralization : public HLA_Interaction_ABC, public HLA_TargetedInteraction
{
public:
    //! @name Constructors/Destructor
    //@{
             HLA_Neutralization();
    explicit HLA_Neutralization( const HLA_RoleInterface& role );
    virtual ~HLA_Neutralization();
    //@}

    //! @name Operations
    //@{
    static hla::Interaction< HLA_Neutralization >& CreateInteractionClass( hla::InteractionNotification_ABC< HLA_Neutralization >& callback );

    virtual void Execute() const;
    //@}
};

#endif // __HLA_Neutralization_h_
