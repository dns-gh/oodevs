// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/PHY_FireDamages_Agent.h $
// $Author: Nld $
// $Modtime: 12/04/05 17:19 $
// $Revision: 5 $
// $Workfile: PHY_FireDamages_Agent.h $
//
// *****************************************************************************

#ifndef __PHY_FireDamages_Agent_h_
#define __PHY_FireDamages_Agent_h_

#include <tools/Map.h>

namespace sword
{
    class UnitFireDamages;
    class UnitDamagedByUnitFire;
    class UnitDamagedByCrowdFire;
}

class Human_ABC;
class MIL_Agent_ABC;
class PHY_HumanWound;
class PHY_Composante_ABC;
class PHY_ComposanteType_ABC;
class PHY_ComposanteState;

struct FireResult;

// =============================================================================
// @class  PHY_FireDamages_Agent
// Created: JVT 2004-08-03
// =============================================================================
class PHY_FireDamages_Agent
{
public:
             PHY_FireDamages_Agent();
    virtual ~PHY_FireDamages_Agent();

    //! @name Notifications
    //@{
    void NotifyHumanWoundChanged     ( const Human_ABC& human, const PHY_HumanWound& oldWound );
    void NotifyComposanteStateChanged( const PHY_Composante_ABC& composante, const PHY_ComposanteState& oldState, const PHY_ComposanteState& newState );
    //@}

    //! @name Network
    //@{
    void Serialize( const MIL_Agent_ABC& target, sword::UnitFireDamages& asn ) const;
    void SerializeDamages( sword::UnitDamagedByUnitFire& msg ) const;
    void SerializeDamages( sword::UnitDamagedByCrowdFire& msg ) const;
    //@}

    void VisitComponents( const std::function< void( const std::string& equipment, unsigned int repairableWithEvacuation,
                                                     unsigned int repairableWithoutEvacuation, unsigned int destroyed ) >& visitor ) const;
    unsigned int GetWounded() const;
    unsigned int GetDead() const;

private:
    //! @name Types
    //@{
    typedef std::vector< int > T_ComposanteStates;
    typedef tools::Map< const PHY_ComposanteType_ABC*, T_ComposanteStates > T_ComposanteResults;

    typedef std::vector< int > T_HumansPerWoundVector;
    typedef std::vector< T_HumansPerWoundVector >  T_HumansPerRankVector;
    //@}

private:
    //! @name Helpers
    //@{
    template< typename T >
    void DoSerializeDamages( T& msg ) const;
    //@}

private:
    T_ComposanteResults composanteResults_;
    T_HumansPerRankVector humanResults_;
};

#endif // __PHY_FireDamages_Agent_h_
