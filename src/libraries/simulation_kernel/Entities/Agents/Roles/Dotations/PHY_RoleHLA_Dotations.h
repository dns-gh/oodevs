// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PHY_RoleHLA_Dotations_h_
#define __PHY_RoleHLA_Dotations_h_

#include "PHY_RoleInterface_Dotations.h"
class MIL_Entity_ABC;

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
             PHY_RoleHLA_Dotations( MIL_Entity_ABC& entity );
    virtual ~PHY_RoleHLA_Dotations();
    //@}

    //! @name Operations
    //@{
    virtual const PHY_ConsumptionType& GetConsumptionMode() const;
    virtual       bool                 HasDotation       ( const PHY_DotationCategory& category ) const;
    //@}

private:
    //! @name Member data
    //@{
    MIL_Entity_ABC& entity_;
    //@}
};

#endif // __PHY_RoleHLA_Dotations_h_
