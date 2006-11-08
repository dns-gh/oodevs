// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Nbc/PHY_RoleInterface_NBC.h $
// $Author: Jvt $
// $Modtime: 30/03/05 18:31 $
// $Revision: 3 $
// $Workfile: PHY_RoleInterface_NBC.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_NBC_h_
#define __PHY_RoleInterface_NBC_h_

#include "MIL.h"

#include "MT_Tools/MT_Role_ABC.h"

class MIL_NbcAgent;

// =============================================================================
// @class  PHY_RoleInterface_NBC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_NBC : public MT_Role_ABC
{
    MT_COPYNOTALLOWED( PHY_RoleInterface_NBC )

public:
    typedef PHY_RoleInterface_NBC RoleInterface;

public:
             PHY_RoleInterface_NBC( MT_RoleContainer& role );
             PHY_RoleInterface_NBC();
    virtual ~PHY_RoleInterface_NBC();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    virtual bool IsContaminated() const = 0;
    virtual void Poison        ( const MIL_NbcAgent& nbcAgent ) = 0;
    virtual void Contaminate   ( const MIL_NbcAgent& nbcAgent ) = 0;
    virtual void Decontaminate () = 0;
    virtual void Decontaminate ( MT_Float rRatioAgentsWorking ) = 0;
    //@}
};

#include "PHY_RoleInterface_NBC.inl"

#endif // __PHY_RoleInterface_NBC_h_
