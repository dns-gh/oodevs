// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-12-02 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_Neutralization.h $
// $Author: Age $
// $Modtime: 2/12/04 11:55 $
// $Revision: 1 $
// $Workfile: HLA_Neutralization.h $
//
// *****************************************************************************

#ifndef __HLA_Neutralization_h_
#define __HLA_Neutralization_h_

#include "HLA_TargetedInteraction.h"
#include "HLA_Interaction_ABC.h"

template< typename T > class Interaction;
template< typename T > class InteractionNotification_ABC;

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
             HLA_Neutralization( const HLA_RoleInterface& role );
    virtual ~HLA_Neutralization();
    //@}

    //! @name Operations
    //@{
    static Interaction< HLA_Neutralization >& CreateInteractionClass( InteractionNotification_ABC< HLA_Neutralization >& callback );

    virtual void Execute() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    HLA_Neutralization( const HLA_Neutralization& );            //!< Copy constructor
    HLA_Neutralization& operator=( const HLA_Neutralization& ); //!< Assignement operator
    //@}
};

#endif // __HLA_Neutralization_h_
