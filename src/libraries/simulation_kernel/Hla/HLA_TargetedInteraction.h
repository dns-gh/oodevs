// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __HLA_TargetedInteraction_h_
#define __HLA_TargetedInteraction_h_

#include <hla/ObjectIdentifier.h>

namespace hla
{
    template < typename T > class Interaction;
}

class HLA_RoleInterface;
class MIL_Agent_ABC;

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
    explicit HLA_TargetedInteraction( const HLA_RoleInterface& hlaRole );
    virtual ~HLA_TargetedInteraction();
    //@}

protected:
    //! @name Helpers
    //@{
    MIL_Agent_ABC* GetTarget() const;

    template< typename SubClass >
    static void RegisterAttributes( hla::Interaction< SubClass >& interaction );
    //@}

private:
    //! @name Member data
    //@{
    hla::ObjectIdentifier target_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: HLA_TargetedInteraction::RegisterAttributes
// Created: AGE 2004-11-29
// -----------------------------------------------------------------------------
template< typename SubClass >
void HLA_TargetedInteraction::RegisterAttributes( hla::Interaction< SubClass >& interaction )
{
    interaction.Register( ParameterIdentifier( "cible" ), *new hla::Encodable< SubClass, hla::ObjectIdentifier >( & HLA_TargetedInteraction::target_ ) );
}

#endif // __HLA_TargetedInteraction_h_
