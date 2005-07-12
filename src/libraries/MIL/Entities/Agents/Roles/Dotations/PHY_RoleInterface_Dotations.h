// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h $
// $Author: Nld $
// $Modtime: 18/04/05 16:40 $
// $Revision: 4 $
// $Workfile: PHY_RoleInterface_Dotations.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Dotations_h_
#define __PHY_RoleInterface_Dotations_h_

#include "MIL.h"

#include "MT_Tools/MT_Role_ABC.h"

class PHY_ConsumptionType;
class PHY_DotationCategory;

// =============================================================================
// @class  PHY_RoleInterface_Dotations
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Dotations : public MT_Role_ABC
{
    MT_COPYNOTALLOWED( PHY_RoleInterface_Dotations )

public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_Dotations RoleInterface;
    //@}
    
public:
             PHY_RoleInterface_Dotations( MT_RoleContainer& roleContainer );
             PHY_RoleInterface_Dotations();
    virtual ~PHY_RoleInterface_Dotations();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Operations
    //@{
    virtual const PHY_ConsumptionType& GetConsumptionMode() const = 0;
    virtual       bool                 HasDotation       ( const PHY_DotationCategory& category ) const = 0;
    //@}
};

#include "PHY_RoleInterface_Dotations.inl"

#endif // __PHY_RoleInterface_Dotations_h_
