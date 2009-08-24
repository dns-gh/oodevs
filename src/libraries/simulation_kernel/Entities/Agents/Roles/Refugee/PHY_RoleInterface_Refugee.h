// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Refugee/PHY_RoleInterface_Refugee.h $
// $Author: Jvt $
// $Modtime: 31/03/05 10:10 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Refugee.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Refugee_h_
#define __PHY_RoleInterface_Refugee_h_

#include "MT_Tools/Role_ABC.h"

class MIL_AgentPion;
class MIL_Object_ABC;
class NET_ASN_MsgUnitAttributes;

// =============================================================================
// @class  PHY_RoleInterface_Refugee
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Refugee : public tools::Role_ABC
                                , private boost::noncopyable
{

public:
    typedef PHY_RoleInterface_Refugee RoleInterface;

public:
             PHY_RoleInterface_Refugee();
    virtual ~PHY_RoleInterface_Refugee();
    
    //! @name Accessors
    //@{
    virtual bool IsManaged() const = 0;
    virtual bool IsManaged( const MIL_Object_ABC& camp ) const = 0;
    //@}

    //! @name Operations
    //@{
    virtual bool Orientate( const MIL_AgentPion& pionManaging ) = 0;
    virtual bool Release  () = 0;
    virtual bool Release  ( const MIL_Object_ABC& camp ) = 0;
    //@}

    virtual void Update    ( bool bIsDead ) = 0;
    virtual void Clean     () = 0;
    virtual bool HasChanged() const = 0;
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const = 0;
    virtual void SendFullState   ( NET_ASN_MsgUnitAttributes& msg ) const = 0;
    //@}
};

#endif // __PHY_RoleInterface_Refugee_h_
