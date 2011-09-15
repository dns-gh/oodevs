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
#include "simulation_kernel/NetworkUnitAttributesMessageSender_ABC.h"
#include "simulation_kernel/RefugeeActionsNotificationHandler_ABC.h"
#include "MT_Tools/MT_Vector2D.h"
#include "MT_Tools/MT_Profiler.h"

class MIL_AgentPion;
class DEC_Knowledge_Agent;

namespace refugee
{

// =============================================================================
// @class  PHY_RolePion_Refugee
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_Refugee : public PHY_RoleInterface_Refugee
                           , public refugee::RefugeeActionsNotificationHandler_ABC
                           , public network::NetworkUnitAttributesMessageSender_ABC
{
public:
    explicit PHY_RolePion_Refugee( MIL_AgentPion& pion );
    virtual ~PHY_RolePion_Refugee();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    void Update    ( bool bIsDead );
    void Clean     ();
    virtual void UpdateLodging( unsigned int nbrHumansLodgingManaged );
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
    unsigned int GetNbrHumansCampManaged() const;
    unsigned int GetNbrHumansCampUnmanaged() const;
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState( client::UnitAttributes& msg ) const;
    virtual void SendFullState   ( client::UnitAttributes& msg ) const;
    //@}

private:
    //! @name Tools
    //@{
    bool HasChanged() const;
    //@}

    //! @name Helpers
    //@{
    void ManageLodgingCamp();
    void UnmanageLodgingCamp();
    void UpdateSecuritySatisfaction();
    void UpdateHealthSatisfaction();
    void AddAffinityNearUnit( DEC_Knowledge_Agent& knowledge );
    //@}

    MIL_AgentPion&        pion_;
    bool                  bManaged_;
    const MIL_Object_ABC* pCamp_;
    bool                  bHasChanged_;
    unsigned int          nbrHumansLodgingManaged_;
    float                 lodgingSatisfaction_;
    float                 securitySatisfaction_;
    float                 healthSatisfaction_;

    struct NearbyUnitsAffinity
    {
        NearbyUnitsAffinity() : maxSqrDistance( 0.0f ), affinitySum_( 0.0f ), absAffinitySum_ ( 0.0f )
        {
            // NOTHING
        }

        void resetAffinitySum( double maxDistance, const MT_Vector2D& newPosition )
        {
            position = newPosition;
            maxSqrDistance = maxDistance * maxDistance;
            affinitySum_ = absAffinitySum_ = 0.0f;
        }

        double maxSqrDistance;
        float  affinitySum_;
        float  absAffinitySum_;
        MT_Vector2D position;
    };

    NearbyUnitsAffinity nearbyUnitsAffinity;

    template< typename Archive > friend void save_construct_data( Archive& archive, const PHY_RolePion_Refugee* role, const unsigned int /*version*/ );
    template< typename Archive > friend void load_construct_data( Archive& archive, PHY_RolePion_Refugee* role, const unsigned int /*version*/ );
};

} //namespace refugee

BOOST_CLASS_EXPORT_KEY( refugee::PHY_RolePion_Refugee )

#endif // __PHY_RolePion_Refugee_h_
