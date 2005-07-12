// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-09 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Dotations/PHY_RoleHLA_Dotations.h $
// $Author: Nld $
// $Modtime: 18/04/05 16:30 $
// $Revision: 3 $
// $Workfile: PHY_RoleHLA_Dotations.h $
//
// *****************************************************************************

#ifndef __PHY_RoleHLA_Dotations_h_
#define __PHY_RoleHLA_Dotations_h_

#include "PHY_RoleInterface_Dotations.h"

// =============================================================================
/** @class  PHY_RoleHLA_Dotations
    @brief  HLA role dotations
*/
// Created: AGE 2004-11-09
// =============================================================================
class PHY_RoleHLA_Dotations : public PHY_RoleInterface_Dotations
{

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleHLA_Dotations( MT_RoleContainer& roleContainer );
    virtual ~PHY_RoleHLA_Dotations();
    //@}

    //! @name Operations
    //@{
    virtual const PHY_ConsumptionType& GetConsumptionMode() const;
    virtual       bool                 HasDotation       ( const PHY_DotationCategory& category ) const;
    //@}
};

#endif // __PHY_RoleHLA_Dotations_h_
