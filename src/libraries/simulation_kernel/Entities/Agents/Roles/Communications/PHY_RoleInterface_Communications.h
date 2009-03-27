// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Communications/PHY_RoleInterface_Communications.h $
// $Author: Jvt $
// $Modtime: 30/03/05 16:49 $
// $Revision: 3 $
// $Workfile: PHY_RoleInterface_Communications.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Communications_h_
#define __PHY_RoleInterface_Communications_h_

#include "MIL.h"

#include "MT_Tools/MT_Role_ABC.h"

class MIL_Object_ABC;

// =============================================================================
// @class  PHY_RoleInterface_Communications
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Communications : public MT_Role_ABC
{
    MT_COPYNOTALLOWED( PHY_RoleInterface_Communications )

public:
    typedef PHY_RoleInterface_Communications RoleInterface;

public:
             PHY_RoleInterface_Communications( MT_RoleContainer& role );
             PHY_RoleInterface_Communications();
    virtual ~PHY_RoleInterface_Communications();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    virtual bool CanCommunicate() const = 0;
    virtual void Jam           ( const MIL_Object_ABC& jammer ) = 0;
    virtual void Unjam         ( const MIL_Object_ABC& jammer ) = 0;
    //@}
};

#include "PHY_RoleInterface_Communications.inl"

#endif // __PHY_RoleInterface_Communications_h_
