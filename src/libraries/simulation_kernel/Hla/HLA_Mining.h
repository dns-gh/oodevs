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
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_Mining.h $
// $Author: Age $
// $Modtime: 6/12/04 16:37 $
// $Revision: 1 $
// $Workfile: HLA_Mining.h $
//
// *****************************************************************************

#ifndef __HLA_Mining_h_
#define __HLA_Mining_h_

#include "HLA_Interaction_ABC.h"
#include "HLA_ObjectInteraction.h"

namespace hla
{
    template< typename T > class Interaction;
    template< typename T > class InteractionNotification_ABC;
}

// =============================================================================
/** @class  HLA_Mining
    @brief  HLA mining interaction
*/
// Created: AGE 2004-12-06
// =============================================================================
class HLA_Mining : public HLA_Interaction_ABC, public HLA_ObjectInteraction
{

public:
    //! @name Constructors/Destructor
    //@{
             HLA_Mining();
             HLA_Mining( const HLA_Object_ABC& object, double rDelta );
    virtual ~HLA_Mining();
    //@}

    //! @name Operations
    //@{
    virtual void Execute() const;

    static hla::Interaction< HLA_Mining >& CreateInteractionClass( hla::InteractionNotification_ABC< HLA_Mining >& callback );
    //@}

private:
    //! @name Member data
    //@{
    double rDelta_;
    //@}
};

#endif // __HLA_Mining_h_
