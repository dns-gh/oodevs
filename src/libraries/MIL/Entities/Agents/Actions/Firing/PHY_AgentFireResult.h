// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/PHY_AgentFireResult.h $
// $Author: Nld $
// $Modtime: 12/04/05 17:19 $
// $Revision: 5 $
// $Workfile: PHY_AgentFireResult.h $
//
// *****************************************************************************

#ifndef __PHY_AgentFireResult_h_
#define __PHY_AgentFireResult_h_

#include "MIL.h"

#include "Tools/MIL_MOSIDManager.h"

class MIL_Agent_ABC;
class PHY_Human;
class PHY_HumanWound;
class PHY_Composante_ABC;
class PHY_ComposanteType_ABC;
class PHY_ComposanteState;

struct ASN1T_FireResult;

// =============================================================================
// @class  PHY_AgentFireResult
// Created: JVT 2004-08-03
// =============================================================================
class PHY_AgentFireResult
{
public:
     PHY_AgentFireResult();
    ~PHY_AgentFireResult();

    //! @name Notifications
    //@{
    void NotifyHumanWoundChanged     ( const PHY_Human& human, const PHY_HumanWound& oldWound );
    void NotifyComposanteStateChanged( const PHY_Composante_ABC& composante, const PHY_ComposanteState& oldState, const PHY_ComposanteState& newState );
    //@}

    //! @name Network
    //@{
           void Serialize              ( const MIL_Agent_ABC& target, ASN1T_FireResult& asn ) const;
    static void CleanAfterSerialization( ASN1T_FireResult& asn );
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

#include "PHY_AgentFireResult.inl"

#endif // __PHY_AgentFireResult_h_
