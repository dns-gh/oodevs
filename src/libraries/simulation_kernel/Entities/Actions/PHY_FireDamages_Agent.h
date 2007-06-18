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

#include "MIL.h"

class MIL_Agent_ABC;
class PHY_Human;
class PHY_HumanWound;
class PHY_Composante_ABC;
class PHY_ComposanteType_ABC;
class PHY_ComposanteState;

struct ASN1T_FireResult;

// =============================================================================
// @class  PHY_FireDamages_Agent
// Created: JVT 2004-08-03
// =============================================================================
class PHY_FireDamages_Agent
{
public:
     PHY_FireDamages_Agent();
    ~PHY_FireDamages_Agent();

    //! @name Notifications
    //@{
    void NotifyHumanWoundChanged     ( const PHY_Human& human, const PHY_HumanWound& oldWound );
    void NotifyComposanteStateChanged( const PHY_Composante_ABC& composante, const PHY_ComposanteState& oldState, const PHY_ComposanteState& newState );
    //@}

    //! @name Network
    //@{
           void Serialize              ( const MIL_Agent_ABC& target, ASN1T_UnitFireDamages& asn ) const;
    static void CleanAfterSerialization( ASN1T_UnitFireDamages& asn );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< int >                                            T_ComposanteStates;
    typedef std::map< const PHY_ComposanteType_ABC*, T_ComposanteStates > T_ComposanteResults;
    typedef T_ComposanteResults::const_iterator                           CIT_ComposanteResults;

    typedef std::vector< int >                     T_HumansPerWoundVector;
    typedef std::vector< T_HumansPerWoundVector >  T_HumansPerRankVector;
    typedef T_HumansPerRankVector::iterator        IT_HumansPerRankVector;
    typedef T_HumansPerRankVector::const_iterator  CIT_HumansPerRankVector;
    //@}

private:
    T_ComposanteResults   composanteResults_;
    T_HumansPerRankVector humanResults_;
};

#include "PHY_FireDamages_Agent.inl"

#endif // __PHY_FireDamages_Agent_h_
