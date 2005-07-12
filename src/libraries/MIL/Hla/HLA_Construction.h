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
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_Construction.h $
// $Author: Age $
// $Modtime: 6/12/04 16:35 $
// $Revision: 1 $
// $Workfile: HLA_Construction.h $
//
// *****************************************************************************

#ifndef __HLA_Construction_h_
#define __HLA_Construction_h_

#include "HLA_Interaction_ABC.h"
#include "HLA_ObjectInteraction.h"

template< typename T > class Interaction;
template< typename T > class InteractionNotification_ABC;

// =============================================================================
/** @class  HLA_Construction
    @brief  HLA construction interaction
*/
// Created: AGE 2004-12-06
// =============================================================================
class HLA_Construction : public HLA_Interaction_ABC, public HLA_ObjectInteraction
{

public:
    //! @name Constructors/Destructor
    //@{
             HLA_Construction();
             HLA_Construction( const HLA_Object_ABC& targetObject, double rDelta );
    virtual ~HLA_Construction();
    //@}

    //! @name Operations
    //@{
    virtual void Execute() const;

    static Interaction< HLA_Construction >& CreateInteractionClass( InteractionNotification_ABC< HLA_Construction >& callback );
    //@}

private:
    //! @name Member data
    //@{
    double rDelta_;
    //@}
};

#endif // __HLA_Construction_h_
