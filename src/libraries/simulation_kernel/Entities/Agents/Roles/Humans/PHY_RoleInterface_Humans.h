// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Humans/PHY_RoleInterface_Humans.h $
// $Author: Jvt $
// $Modtime: 31/03/05 16:42 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Humans.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Humans_h_
#define __PHY_RoleInterface_Humans_h_

#include "MT_Tools/MT_Role_ABC.h"

// =============================================================================
// @class  PHY_RoleInterface_Humans
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Humans : public MT_Role_ABC
{
    MT_COPYNOTALLOWED( PHY_RoleInterface_Humans )

public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_Humans RoleInterface;
    //@}

public:
    explicit PHY_RoleInterface_Humans( MT_RoleContainer& role );
             PHY_RoleInterface_Humans();
    virtual ~PHY_RoleInterface_Humans();
    
    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
};

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Humans::serialize
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RoleInterface_Humans::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MT_Role_ABC >( *this );
}

#endif // __PHY_RoleInterface_Humans_h_
