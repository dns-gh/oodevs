// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Transported/PHY_RolePion_Transported.h $
// $Author: Jvt $
// $Modtime: 12/05/05 17:09 $
// $Revision: 7 $
// $Workfile: PHY_RolePion_Transported.h $
//
// *****************************************************************************

#ifndef __PHY_RolePion_Transported_h_
#define __PHY_RolePion_Transported_h_

#include "MIL.h"

#include "PHY_RoleInterface_Transported.h"

class MIL_AgentPion;
class NET_ASN_MsgUnitAttributes;

// =============================================================================
// @class  PHY_RolePion_Transported
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Transported : public PHY_RoleInterface_Transported
{
    MT_COPYNOTALLOWED( PHY_RolePion_Transported )

public:
             PHY_RolePion_Transported( MT_RoleContainer& role, MIL_AgentPion& pion );
             PHY_RolePion_Transported();
    virtual ~PHY_RolePion_Transported();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive& , const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}
   
    //! @name Operations
    //@{
    void Update    ( bool bIsDead );
    void Clean     ();
    bool HasChanged() const;
    //@}

    //! @name Transport
    //@{
    virtual bool LoadForTransport   ( const MIL_Agent_ABC& transporter, bool bTransportOnlyLoadable );
    virtual bool UnloadFromTransport( const MIL_Agent_ABC& transporter, bool bTransportOnlyLoadable );
    virtual bool CancelTransport    ( const MIL_Agent_ABC& transporter );
    virtual void GetTransportWeight ( bool bTransportOnlyLoadable, MT_Float& rTotalWeight, MT_Float& rHeaviestComposanteWeight ) const;
    virtual bool IsTransported      () const;
    //@}

    //! @name Human transporters ... $$$
    //@{
    bool HasHumanTransportersReady    () const;
    void DisableHumanTransporters     ( const MT_Vector2D& vPosition );
    void RecoverHumanTransporters     ();
    bool HasHumanTransportersToRecover() const;
    //@}

    //! @name Network
    //@{
    void SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const;
    void SendFullState   ( NET_ASN_MsgUnitAttributes& msg ) const;
    //@}

private:
          MIL_AgentPion* pPion_;
          bool           bHasChanged_;
    const MIL_Agent_ABC* pTransporter_;

    MT_Vector2D  vLoadingPosition_; 
    MT_Vector2D  vHumanTransporterPosition_; // position des 'VABs' quand seuls les humains sont héliportés
};

#include "PHY_RolePion_Transported.inl"

#endif // __PHY_RolePion_Transported_h_
