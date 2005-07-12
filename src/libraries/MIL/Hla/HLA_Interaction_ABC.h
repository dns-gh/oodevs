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
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_Interaction_ABC.h $
// $Author: Age $
// $Modtime: 29/11/04 16:42 $
// $Revision: 1 $
// $Workfile: HLA_Interaction_ABC.h $
//
// *****************************************************************************

#ifndef __HLA_Interaction_ABC_h_
#define __HLA_Interaction_ABC_h_

// =============================================================================
/** @class  HLA_Interaction_ABC
    @brief  HLA interaction definition
*/
// Created: AGE 2004-11-29
// =============================================================================
class HLA_Interaction_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             HLA_Interaction_ABC();
    virtual ~HLA_Interaction_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Execute() const = 0;
    //@}
};

#endif // __HLA_Interaction_ABC_h_
