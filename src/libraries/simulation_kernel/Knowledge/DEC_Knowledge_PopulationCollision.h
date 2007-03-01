// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_PopulationCollision.h $
// $Author: Jvt $
// $Modtime: 14/04/05 12:33 $
// $Revision: 6 $
// $Workfile: DEC_Knowledge_PopulationCollision.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_PopulationCollision_h_
#define __DEC_Knowledge_PopulationCollision_h_

#include "MIL.h"

#include "DEC_Knowledge_ABC.h"

class MIL_Population;
class MIL_PopulationFlow;
class MIL_PopulationConcentration;
class MIL_AgentPion;
class DEC_Knowledge_Population;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_PopulationCollision : public DEC_Knowledge_ABC
{
    MT_COPYNOTALLOWED( DEC_Knowledge_PopulationCollision ) 

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_PopulationCollision( const MIL_AgentPion& agentColliding, MIL_Population& population );
             DEC_Knowledge_PopulationCollision ();
    virtual ~DEC_Knowledge_PopulationCollision();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    void Prepare();
    void Update ( MIL_PopulationFlow&          flow          );
    void Update ( MIL_PopulationConcentration& concentration );
    bool Clean  ();

    void PublishKnowledges( DEC_Knowledge_Population& knowledge ) const;
    //@}

    //! @name Operations
    //@{
          MT_Float     GetMaxPopulationDensity   () const;
          MT_Float     GetPionMaxSpeed           () const;
          MT_Float     GetPionReloadingTimeFactor() const;
    const MT_Vector2D& GetPosition               () const;
    //@}

    //! @name Accessors
    //@{
          MIL_Population&  GetPopulation    () const;
    const MIL_AgentPion&   GetAgentColliding() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< MIL_PopulationFlow* >     T_PopulationFlowSet;
    typedef T_PopulationFlowSet::const_iterator CIT_PopulationFlowSet;

    typedef std::set< MIL_PopulationConcentration* >     T_PopulationConcentrationSet;
    typedef T_PopulationConcentrationSet::const_iterator CIT_PopulationConcentrationSet;
    //@}

private:
    const MIL_AgentPion*                pAgentColliding_;
          MIL_Population*               pPopulation_;

          T_PopulationFlowSet           flows_;
          T_PopulationConcentrationSet  concentrations_;

          T_PopulationFlowSet           previousFlows_;
          T_PopulationConcentrationSet  previousConcentrations_;
};

#include "DEC_Knowledge_PopulationCollision.inl"

#endif // __DEC_Knowledge_PopulationCollision_h_
