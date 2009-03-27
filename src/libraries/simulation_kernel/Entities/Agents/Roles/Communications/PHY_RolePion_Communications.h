// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Communications/PHY_RolePion_Communications.h $
// $Author: Jvt $
// $Modtime: 30/03/05 16:59 $
// $Revision: 3 $
// $Workfile: PHY_RolePion_Communications.h $
//
// *****************************************************************************

#ifndef __PHY_RolePion_Communications_h_
#define __PHY_RolePion_Communications_h_

#include "MIL.h"

#include "PHY_RoleInterface_Communications.h"

namespace xml
{
    class xistream;
}

class MIL_CommunicationsAgentType;
class NET_ASN_MsgUnitAttributes;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_RolePion_Communications
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Communications : public PHY_RoleInterface_Communications
{
    MT_COPYNOTALLOWED( PHY_RolePion_Communications )

public:
    //! @name Static
    //@{
    static void Initialize( xml::xistream& xis );
    //@}

             PHY_RolePion_Communications( MT_RoleContainer& role, MIL_AgentPion& pion );
             PHY_RolePion_Communications();
    virtual ~PHY_RolePion_Communications();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    void Update    ( bool bIsDead );
    void Clean     ();
    bool HasChanged() const;
    //@}

    //! @name Main
    //@{
    virtual bool CanCommunicate() const;
    virtual void Jam           ( const MIL_Object_ABC& jammer );
    virtual void Unjam         ( const MIL_Object_ABC& jammer );

            void ActivateBlackout  ();
            void DeactivateBlackout();
    //@}

    //! @name Operations
    //@{
    MT_Float ModifySpeed            ( MT_Float rSpeed    ) const;
    MT_Float ModifyReloadingDuration( MT_Float rDuration ) const;
    //@}

    //! @name Network
    //@{
    void SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const;
    void SendFullState   ( NET_ASN_MsgUnitAttributes& msg ) const;
    //@}

public:
    //! @name Types
    //@{
    typedef std::set< const MIL_Object_ABC* > T_JammerSet;
    typedef T_JammerSet::const_iterator           CIT_JammerSet;
    //@}

private:
    MIL_AgentPion* pPion_;
    T_JammerSet    jammers_;
    bool           bBlackoutActivated_;
    bool           bHasChanged_;

private:
    static MT_Float rCoefSpeedModificator_;
    static MT_Float rCoefReloadingTimeModificator_;
};

#include "PHY_RolePion_Communications.inl"

#endif // __PHY_RolePion_Communications_h_
