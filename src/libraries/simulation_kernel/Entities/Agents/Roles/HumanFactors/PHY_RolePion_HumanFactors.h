// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/HumanFactors/PHY_RolePion_HumanFactors.h $
// $Author: Jvt $
// $Modtime: 30/03/05 17:09 $
// $Revision: 3 $
// $Workfile: PHY_RolePion_HumanFactors.h $
//
// *****************************************************************************

#ifndef __PHY_RolePion_HumanFactors_h_
#define __PHY_RolePion_HumanFactors_h_

#include "MIL.h"
#include "PHY_RoleInterface_HumanFactors.h"
#include "MT_Tools/AlgorithmModifier_ABC.h"
#include "simulation_kernel/NetworkUnitMessageNotificationHandler_ABC.h"

namespace xml
{
    class xistream;
}

class NET_ASN_MsgUnitAttributes;
class PHY_Morale;
class PHY_Experience;
class PHY_Tiredness;
class MIL_Entity_ABC;

namespace posture
{
    class PostureComputer_ABC;
}

namespace firing
{
    class WeaponReloadingComputer_ABC;
}
namespace moving
{
	class SpeedComputer_ABC;
}
namespace detection
{
    class PerceptionDistanceComputer_ABC;
}
// =============================================================================
// @class  PHY_RolePion_HumanFactors
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_HumanFactors : public PHY_RoleInterface_HumanFactors
                                , public tools::AlgorithmModifier_ABC< posture::PostureComputer_ABC >
                                , public tools::AlgorithmModifier_ABC< firing::WeaponReloadingComputer_ABC >
                                , public tools::AlgorithmModifier_ABC< moving::SpeedComputer_ABC >
                                , public tools::AlgorithmModifier_ABC< detection::PerceptionDistanceComputer_ABC >
                                , public network::NetworkUnitMessageNotificationHandler_ABC
{

public:
             PHY_RolePion_HumanFactors( MIL_Entity_ABC& entity );
    virtual ~PHY_RolePion_HumanFactors();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    
    //! @name Operations
    //@{
    void Update    ( bool bIsDead );
    void Clean     ();
    virtual void Execute( posture::PostureComputer_ABC& algorithm ) const;
    virtual void Execute( firing::WeaponReloadingComputer_ABC& algorithm ) const;
    virtual void Execute( moving::SpeedComputer_ABC& algorithm ) const;
    virtual void Execute( detection::PerceptionDistanceComputer_ABC& algorithm ) const;
    //@}

    //! @name Main
    //@{
    void ReadOverloading( xml::xistream& xis );
    void SetMorale      ( const PHY_Morale& morale         );
    void SetExperience  ( const PHY_Experience& experience );
    void SetTiredness   ( const PHY_Tiredness&  tiredness  );
    //@}

    //! @name Accessors
    //@{
    const PHY_Morale&     GetMorale    () const;
    const PHY_Tiredness&  GetTiredness () const;
    //@}
            
     //! @name Operations
     //@{
    MT_Float ModifyPH                    ( MT_Float rPH       ) const;
     //@}

    //! @name Network
    //@{
    virtual void SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const;
    virtual void SendFullState   ( NET_ASN_MsgUnitAttributes& msg ) const;
    //@}

private: 
    bool bHasChanged_;

    const PHY_Morale*     pMorale_;
    const PHY_Experience* pExperience_;
    const PHY_Tiredness*  pTiredness_;

    MIL_Entity_ABC& entity_;

private:
    //! @name Helpers
    //@{
    bool HasChanged() const;
    void ReadFacteursHumains( xml::xistream& xis );
    void ReadFatigue        ( xml::xistream& xis );
    void ReadMoral          ( xml::xistream& xis );
    void ReadExperience     ( xml::xistream& xis );

    template< typename Archive > friend  void save_construct_data( Archive& archive, const PHY_RolePion_HumanFactors* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, PHY_RolePion_HumanFactors* role, const unsigned int /*version*/ );

    //@}
};

#endif // __PHY_RolePion_HumanFactors_h_
