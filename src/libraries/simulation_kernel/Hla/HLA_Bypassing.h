// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __HLA_Bypassing_h_
#define __HLA_Bypassing_h_

#include "HLA_Interaction_ABC.h"
#include "HLA_ObjectInteraction.h"

namespace hla
{
    template< typename T > class Interaction;
    template< typename T > class InteractionNotification_ABC;
}

// =============================================================================
/** @class  HLA_Bypassing
    @brief  HLA bypass creation interaction
*/
// Created: AGE 2004-12-06
// =============================================================================
class HLA_Bypassing : public HLA_Interaction_ABC, public HLA_ObjectInteraction
{
public:
    //! @name Constructors/Destructor
    //@{
             HLA_Bypassing();
             HLA_Bypassing( const HLA_Object_ABC& targetObject, double rDelta );
    virtual ~HLA_Bypassing();
    //@}

    //! @name Operations
    //@{
    virtual void Execute() const;

    static hla::Interaction< HLA_Bypassing >& CreateInteractionClass( hla::InteractionNotification_ABC< HLA_Bypassing >& callback );
    //@}

private:
    //! @name Member data
    //@{
    double rDelta_;
    //@}
};

#endif // __HLA_Bypassing_h_
