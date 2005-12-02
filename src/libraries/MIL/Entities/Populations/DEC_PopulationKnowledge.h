// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Populations/DEC_PopulationKnowledge.h $
// $Author: Nld $
// $Modtime: 23/06/05 10:29 $
// $Revision: 9 $
// $Workfile: DEC_PopulationKnowledge.h $
//
// *****************************************************************************

#ifndef __DEC_PopulationKnowledge_h_
#define __DEC_PopulationKnowledge_h_

#include "MIL.h"

#include "Knowledge/DEC_Knowledge_Def.h"

class MIL_Agent_ABC;
class MIL_AgentPion;

// =============================================================================
// @class  DEC_PopulationKnowledge
// Created: JVT 2004-08-03
// =============================================================================
class DEC_PopulationKnowledge
{
    MT_COPYNOTALLOWED( DEC_PopulationKnowledge )

public:
    //! @name Types
    //@{
    typedef std::set< const MIL_Agent_ABC* > T_AgentSet;
    typedef T_AgentSet::const_iterator       CIT_AgentSet;
    //@}

public:
     DEC_PopulationKnowledge();
    ~DEC_PopulationKnowledge();

    //! @name Main operations
    //@{
    void Update();
    void Clean ();
    //@}

    //! @name Operations
    //@{
    void NotifyAttackedBy ( const MIL_Agent_ABC& attacker );
    void GetPionsAttacking( T_PopulationKnowledgeAgentDiaIDVector& container ) const;
    void NotifySecuredBy  ( const MIL_Agent_ABC& securer );
    void GetPionsSecuring ( T_PopulationKnowledgeAgentDiaIDVector& container ) const;
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

private:
    T_AgentSet attackers_; 
    T_AgentSet newAttackers_;

    T_AgentSet securers_;
    T_AgentSet newSecurers_;
};

#include "DEC_PopulationKnowledge.inl"

#endif // __DEC_PopulationKnowledge_h_
