// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
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
{
public:
    //! @name Types
    //@{
    typedef PHY_RoleAction_DirectFiring RoleInterface;
    //@}

public:
             PHY_RoleAction_DirectFiring();
    explicit PHY_RoleAction_DirectFiring( MIL_AgentPion& pion );
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
    int  FirePopulation         ( unsigned int nTargetKnowledgeID, PHY_FireResults_Pion*& pFireResult, const PHY_AmmoDotationClass* dotationClass );
    void FirePopulationSuspended( unsigned int nTargetKnowledgeID );

    int  FirePion               ( boost::shared_ptr< DEC_Knowledge_Agent > pAgent, PHY_DirectFireData::E_FiringMode nFiringMode, double rPercentageComposantesToUse, PHY_DirectFireData::E_ComposanteFiringType nComposanteFiringType, PHY_DirectFireData::E_ComposanteFiredType nComposanteFiredType, PHY_FireResults_Pion*& pFireResult, bool mustReport, const PHY_AmmoDotationClass* pAmmoDotationClass = 0 );
    void FirePionSuspended      ( boost::shared_ptr< DEC_Knowledge_Agent > pAgent, bool mustReport );
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
        eEnemyDestroyed,    // -> le tir est terminé parceque le pion adverse est détruit
        eNoCapacity,        // -> le tir est terminé car il ne reste aucune composante ayant la capacité de tirer
        eNoAmmo,            // -> le tir est terminé parque qu'il reste des composante capables de tirer mais qu'il ne reste plus de munition adéquates
        eRunning,           // -> le tir est en cours d'exécution
        eFinished,          // Stop tir
        eTemporarilyBlocked // -> Le tir ne peut pas etre effectue tout de suite pour cause d'encombrement en zone urbaine
    };
    //@}

private:
    //! @name Tools
    //@{
    MIL_Population* GetPopulationTarget( unsigned int nTargetKnowledgeID );
    int FirePion( PHY_DirectFireData& firerWeapons, MIL_Agent_ABC& target, const PHY_Composante_ABC::T_ComposanteVector& compTargets, PHY_FireResults_Pion& fireResult );
    //@}

private:
    //! @name Member data
    //@{
    MIL_AgentPion* owner_;
    //@}
};

} //namespace firing

BOOST_CLASS_EXPORT_KEY( firing::PHY_RoleAction_DirectFiring )

#endif // __PHY_RoleAction_DirectFiring_h_
