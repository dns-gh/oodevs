// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Refugee/PHY_RolePion_Refugee.h $
// $Author: Jvt $
// $Modtime: 31/03/05 10:12 $
// $Revision: 2 $
// $Workfile: PHY_RolePion_Refugee.h $
//
// *****************************************************************************

#ifndef __PHY_RolePion_Refugee_h_
#define __PHY_RolePion_Refugee_h_

#include "PHY_RoleInterface_Refugee.h"
#include "simulation_kernel/RefugeeActionsNotificationHandler_ABC.h"

class NET_ASN_MsgUnitAttributes;
class MIL_AgentPion;

namespace refugee
{

// =============================================================================
// @class  PHY_RolePion_Refugee
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Refugee : public PHY_RoleInterface_Refugee
                           , public refugee::RefugeeActionsNotificationHandler_ABC
{

public:
    explicit PHY_RolePion_Refugee( MIL_AgentPion& pion );
    virtual ~PHY_RolePion_Refugee();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    void Update    ( bool bIsDead );
    void Clean     ();
    //@}

    //! @name Event
    //@{
    virtual void Orientate( MIL_AgentPion& pionManaging );
    virtual void Release  ( MIL_AgentPion& callerAgent );
    virtual void ReleaseCamp( MIL_AgentPion& callerAgent, const MIL_Object_ABC& camp );
    //@}

    //! @name Accessors
    //@{
    virtual bool IsManaged() const;
    virtual bool IsManaged( const MIL_Object_ABC& camp ) const;
    //@}

    //! @name Network
    //@{
    void SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const;
    void SendFullState   ( NET_ASN_MsgUnitAttributes& msg ) const;
    //@}

private:
    //! @name Tools
    //@{
    bool HasChanged() const;
    //@}

          MIL_AgentPion&    pion_;
          bool              bManaged_;
    const MIL_Object_ABC*   pCamp_;
          bool              bHasChanged_;

    template< typename Archive > friend  void save_construct_data( Archive& archive, const PHY_RolePion_Refugee* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, PHY_RolePion_Refugee* role, const unsigned int /*version*/ );

};

} //namespace refugee

#endif // __PHY_RolePion_Refugee_h_
