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
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_TargetedInteraction.h $
// $Author: Age $
// $Modtime: 29/11/04 17:17 $
// $Revision: 1 $
// $Workfile: HLA_TargetedInteraction.h $
//
// *****************************************************************************

#ifndef __HLA_TargetedInteraction_h_
#define __HLA_TargetedInteraction_h_

#include "hla/ObjectIdentifier.h"

class HLA_RoleInterface;
class MIL_Agent_ABC;
template < typename T > class Interaction;

// =============================================================================
/** @class  HLA_TargetedInteraction
    @brief  HLA Targeted interaction
*/
// Created: AGE 2004-11-29
// =============================================================================
class HLA_TargetedInteraction
{

public:
    //! @name Constructors/Destructor
    //@{
             HLA_TargetedInteraction();
             HLA_TargetedInteraction( const HLA_RoleInterface& hlaRole );
    virtual ~HLA_TargetedInteraction();
    //@}

protected:
    //! @name Helpers
    //@{
    MIL_Agent_ABC* GetTarget() const;

    template< typename SubClass >
    static void RegisterAttributes( Interaction< SubClass >& interaction );
    //@}

private:
    //! @name Member data
    //@{
    ObjectIdentifier target_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: HLA_TargetedInteraction::RegisterAttributes
// Created: AGE 2004-11-29
// -----------------------------------------------------------------------------
template< typename SubClass >
void HLA_TargetedInteraction::RegisterAttributes( Interaction< SubClass >& interaction )
{
    interaction.Register( ParameterIdentifier( "cible" ), *new Attribute< SubClass, ObjectIdentifier >( & HLA_TargetedInteraction::target_ ) );
}

#endif // __HLA_TargetedInteraction_h_
