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
#include "simulation_kernel/NetworkUnitAttributesMessageSender_ABC.h"

class PHY_Morale;
class PHY_Experience;
class PHY_Tiredness;
class PHY_Stress;
class MIL_Entity_ABC;

namespace xml
{
    class xistream;
}
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
    class PerceptionDistanceComputer;
}

// =============================================================================
// @class  PHY_RolePion_HumanFactors
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_HumanFactors : public PHY_RoleInterface_HumanFactors
                                , public tools::AlgorithmModifier_ABC< posture::PostureComputer_ABC >
                                , public tools::AlgorithmModifier_ABC< firing::WeaponReloadingComputer_ABC >
                                , public tools::AlgorithmModifier_ABC< moving::SpeedComputer_ABC >
                                , public tools::AlgorithmModifier_ABC< detection::PerceptionDistanceComputer >
                                , public network::NetworkUnitAttributesMessageSender_ABC
{
public:
             PHY_RolePion_HumanFactors();
    explicit PHY_RolePion_HumanFactors( MIL_Entity_ABC& entity );
    virtual ~PHY_RolePion_HumanFactors();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    void Update    ( bool bIsDead );
    void Clean     ();
    virtual void Execute( posture::PostureComputer_ABC& algorithm ) const;
    virtual void Execute( firing::WeaponReloadingComputer_ABC& algorithm ) const;
    virtual void Execute( moving::SpeedComputer_ABC& algorithm ) const;
    virtual void Execute( detection::PerceptionDistanceComputer& algorithm ) const;
    //@}

    //! @name Main
    //@{
    void ReadOverloading( xml::xistream& xis );
    void SetMorale( const PHY_Morale& morale );
    void SetExperience( const PHY_Experience& experience );
    void SetTiredness( const PHY_Tiredness& tiredness, bool updateValue = false );
    void SetStress( const PHY_Stress& stress, bool updateValue = false );
    //@}

    //! @name Accessors
    //@{
    const PHY_Morale& GetMorale() const;
    const PHY_Tiredness& GetTiredness() const;
    //@}

     //! @name Operations
     //@{
    double ModifyPH( double rPH ) const;
    virtual void NotifyAttacked();
     //@}

    //! @name Network
    //@{
    virtual void SendChangedState( client::UnitAttributes& msg ) const;
    virtual void SendFullState   ( client::UnitAttributes& msg ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void Evolution();
    void NoEvolution();
    void UpdateStress();
    void UpdateStressValue();
    void UpdateTiredness();
    void UpdateTirednessValue();
    bool HasChanged() const;
    void ReadFacteursHumains( xml::xistream& xis );
    void ReadFatigue( xml::xistream& xis );
    void ReadMoral( xml::xistream& xis );
    void ReadExperience( xml::xistream& xis );
    void ReadStress( xml::xistream& xis );
//@}

private:
    //! @name Types
    //@{
    typedef void (PHY_RolePion_HumanFactors::*T_EvolutionFunction)();
    //@}

    //! @name Member data
    //@{
    MIL_Entity_ABC*       owner_;
    bool                  bHasChanged_;

    const PHY_Morale*     pMorale_;
    const PHY_Experience* pExperience_;
    const PHY_Tiredness*  pTiredness_;
    const PHY_Stress*     pStress_;
    double                stressValue_;
    double                tirednessValue_;

    T_EvolutionFunction   evolutionFunction_;
    //@}

};

BOOST_CLASS_EXPORT_KEY( PHY_RolePion_HumanFactors )

#endif // __PHY_RolePion_HumanFactors_h_
