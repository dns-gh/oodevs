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

#include "MT_Tools/Role_ABC.h"

class MIL_ToxicEffectManipulator;

// =============================================================================
// @class  PHY_RoleInterface_NBC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_NBC : public tools::Role_ABC
                            , private boost::noncopyable
{

public:
    typedef PHY_RoleInterface_NBC RoleInterface;

public:
             PHY_RoleInterface_NBC();
    virtual ~PHY_RoleInterface_NBC();
    
    //! @name Operations
    //@{
    virtual bool IsContaminated() const = 0;
    virtual void Poison        ( const MIL_ToxicEffectManipulator& contamination ) = 0;
    virtual void Contaminate   ( const MIL_ToxicEffectManipulator& contamination ) = 0;
    virtual void Decontaminate () = 0;
    virtual void Decontaminate ( MT_Float rRatioAgentsWorking ) = 0;
    //@}
};

#endif // __PHY_RoleInterface_NBC_h_
