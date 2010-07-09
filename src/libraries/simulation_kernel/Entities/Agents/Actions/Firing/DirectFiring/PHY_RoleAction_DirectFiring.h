// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/DirectFiring/PHY_RoleAction_DirectFiring.h $
// $Author: Jvt $
// $Modtime: 30/03/05 15:37 $
// $Revision: 4 $
// $Workfile: PHY_RoleAction_DirectFiring.h $
//
// *****************************************************************************

#ifndef __PHY_RoleAction_DirectFiring_h_
#define __PHY_RoleAction_DirectFiring_h_

#include "MIL.h"

#include "MT_Tools/Role_ABC.h"
#include "Entities/Agents/Units/Composantes/PHY_Composante_ABC.h"
#include "PHY_DirectFireData.h"

class PHY_AmmoDotationClass;
class PHY_FireResults_Pion;
class PHY_FireResults_Default;
class MIL_Agent_ABC;
class MIL_AgentPion;
class MIL_Population;
class MIL_Object_ABC;
class DEC_Knowledge_Agent;

namespace firing
{
// =============================================================================
// @class  PHY_RoleAction_DirectFiring
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleAction_DirectFiring : public tools::Role_ABC
                                  , private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef PHY_RoleAction_DirectFiring RoleInterface;
    //@}

public:
             PHY_RoleAction_DirectFiring( MIL_AgentPion& pion );
    virtual ~PHY_RoleAction_DirectFiring();

    //! @name Checkpoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    void Update( bool bIsDead );
    void Clean ();
    //@}

    //! @name Operations
    //@{
    int  FirePopulation         ( unsigned int nTargetKnowledgeID, PHY_FireResults_Pion*& pFireResult );
    void FirePopulationSuspended( unsigned int nTargetKnowledgeID );

    int  FirePion               ( boost::shared_ptr< DEC_Knowledge_Agent > pAgent, PHY_DirectFireData::E_FiringMode nFiringMode, MT_Float rPercentageComposantesToUse, PHY_DirectFireData::E_ComposanteFiringType nComposanteFiringType, PHY_DirectFireData::E_ComposanteFiredType nComposanteFiredType, PHY_FireResults_Pion*& pFireResult, const PHY_AmmoDotationClass* pAmmoDotationClass = 0 );
    void FirePionSuspended      ( boost::shared_ptr< DEC_Knowledge_Agent > pAgent );
    int  IlluminatePion         ( boost::shared_ptr< DEC_Knowledge_Agent > pEnemy );
    void IlluminatePionSuspended( boost::shared_ptr< DEC_Knowledge_Agent > pEnemy );

    void FireZone               ( const MIL_Object_ABC& zone, PHY_FireResults_Default*& pFireResult );
    //@}

    //! @name Return codes
    //@{
    int GetInitialReturnCode() const;
    int GetFinalReturnCode  () const;
    //@}

private:
    //! @name Types
    //@{
    enum E_ReturnCode
    {
        eImpossible,
        eEnemyDestroyed , // -> le tir est termin� parceque le pion adverse est d�truit
        eNoCapacity     , // -> le tir est termin� car il ne reste aucune composante ayant la capacit� de tirer
        eNoAmmo         , // -> le tir est termin� parque qu'il reste des composante capables de tirer mais qu'il ne reste plus de munition ad�quates
        eRunning        , // -> le tir est en cours d'ex�cution
        eFinished         // Stop tir
    };
    //@}

private:
    //! @name Tools
    //@{
    MIL_Population* GetPopulationTarget( unsigned int nTargetKnowledgeID );
    void            FirePion           ( PHY_DirectFireData& firerWeapons, MIL_Agent_ABC& target, const PHY_Composante_ABC::T_ComposanteVector& compTargets, PHY_FireResults_Pion& fireResult );
    template< typename Archive > friend  void save_construct_data( Archive& archive, const PHY_RoleAction_DirectFiring* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, PHY_RoleAction_DirectFiring* role, const unsigned int /*version*/ );
    //@}

private:
    MIL_AgentPion& pion_;

};

} //namespace firing

BOOST_CLASS_EXPORT_KEY( firing::PHY_RoleAction_DirectFiring )

#endif // __PHY_RoleAction_DirectFiring_h_
