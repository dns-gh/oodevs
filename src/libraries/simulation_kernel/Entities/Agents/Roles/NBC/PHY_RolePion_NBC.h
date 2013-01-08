// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __PHY_RolePion_NBC_h_
#define __PHY_RolePion_NBC_h_

#include "MIL.h"
#include "PHY_RoleInterface_NBC.h"
#include "MT_Tools/AlgorithmModifier_ABC.h"
#include "simulation_kernel/NetworkUnitAttributesMessageSender_ABC.h"

class MIL_ToxicEffectManipulator;
class MIL_NbcAgentType;
class MIL_AgentPion;
class MIL_DisasterType;

namespace firing
{
    class WeaponReloadingComputer_ABC;
}

namespace moving
{
    class SpeedComputer_ABC;
}

namespace nbc
{
// =============================================================================
// @class  PHY_RolePion_NBC
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RolePion_NBC : public PHY_RoleInterface_NBC
                       , public tools::AlgorithmModifier_ABC<moving::SpeedComputer_ABC>
                       , public tools::AlgorithmModifier_ABC< firing::WeaponReloadingComputer_ABC >
                       , public network::NetworkUnitAttributesMessageSender_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PHY_RolePion_NBC( MIL_AgentPion& pion );
    virtual ~PHY_RolePion_NBC();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    void Update( bool bIsDead );
    void Clean();
    //@}

    //! @name Main
    //@{
    virtual void Poison( const MIL_ToxicEffectManipulator& nbcAgent );
    virtual void Contaminate( const MIL_ToxicEffectManipulator& nbcAgent );
    virtual void Afflict( float dose, const MIL_DisasterType& type );
    virtual void Decontaminate();
    virtual void StartDecontamination();
    virtual void Decontaminate( double rRatioAgentsWorking );
    void WearNbcProtectionSuit();
    void RemoveNbcProtectionSuit();
    virtual bool IsForcedImmune() const;
    virtual void TemporaryImmunizeAgent( bool bImmunize );
    virtual double GetContaminationQuantity() const;
    virtual void Execute( firing::WeaponReloadingComputer_ABC& algorithm ) const;
    void Execute( moving::SpeedComputer_ABC& algorithm ) const;
    //@}

    //! @name Accessors
    //@{
    virtual bool IsContaminated() const;
    virtual bool IsIntoxicated() const;
    virtual double GetDecontaminationState() const;
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState( client::UnitAttributes& msg ) const;
    virtual void SendFullState( client::UnitAttributes& msg ) const;
    //@}

public:
    //! @name Types
    //@{
    typedef std::set< const MIL_NbcAgentType* > T_NbcAgentTypeSet;
    typedef T_NbcAgentTypeSet::const_iterator CIT_NbcAgentTypeSet;
    //@}

private:
    //! @name Tools
    //@{
    bool HasChanged() const;
    void ContaminateOtherUnits();
    void ApplyWound( const MIL_DisasterType& type );
    std::vector< const MIL_NbcAgentType* > GetContaminating() const;
    //@}

    //! @name Serialization
    //@{
    INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA_HEADER( PHY_RolePion_NBC )
    //@}

private:
    //! @name Data Members
    //@{
    MIL_AgentPion& owner_;
    T_NbcAgentTypeSet nbcAgentTypesContaminating_;
    double rDecontaminationState_;
    double rContaminationQuantity_;
    float dose_;
    bool bNbcProtectionSuitWorn_;
    bool bHasChanged_;
    bool poisoned_;
    bool intoxicated_;
    bool immune_;
    bool contaminated_;
    int currentAttritionThreshold_;
    //@}
};

}

BOOST_CLASS_EXPORT_KEY( nbc::PHY_RolePion_NBC )
namespace nbc
{
    INTERNAL_BOOST_SAVE_LOAD_CONSTRUCT_DATA( PHY_RolePion_NBC, MIL_AgentPion )
}

#endif // __PHY_RolePion_NBC_h_
