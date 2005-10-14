// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_PopulationPerception.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 4 $
// $Workfile: DEC_Knowledge_PopulationPerception.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_PopulationPerception_h_
#define __DEC_Knowledge_PopulationPerception_h_

#include "MIL.h"

#include "DEC_Knowledge_ABC.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"

class MIL_Population;
class MIL_PopulationConcentration;
class MIL_PopulationFlow;
class MIL_AgentPion;
class DEC_Knowledge_PopulationConcentrationPerception;
class DEC_Knowledge_PopulationFlowPerception;
class DEC_Knowledge_Population;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_PopulationPerception : public DEC_Knowledge_ABC
{
    MT_COPYNOTALLOWED( DEC_Knowledge_PopulationPerception ) 

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_PopulationPerception( const MIL_AgentPion& agentPerceiving, MIL_Population& populationPerceived );
             DEC_Knowledge_PopulationPerception();
    virtual ~DEC_Knowledge_PopulationPerception();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}
    
    //! @name Operations
    //@{
    void Prepare();
    void Update ( MIL_PopulationConcentration& concentrationPerceived, const PHY_PerceptionLevel& level );
    void Update ( MIL_PopulationFlow&          flowPerceived         , const PHY_PerceptionLevel& level, const T_PointVector& shape );
    bool Clean  ();

    void PublishKnowledges( DEC_Knowledge_Population& knowledge ) const;
    bool IsIdentified     ( const MIL_PopulationConcentration& concentration );
    //@}

    //! @name Network operations
    //@{
    void UpdateOnNetwork     () const;
    void SendStateToNewClient() const;
    //@}

    //! @name Accessors
    //@{
          MIL_Population& GetPopulationPerceived() const;
    const MIL_AgentPion&  GetAgentPerceiving    () const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< const MIL_PopulationConcentration*, DEC_Knowledge_PopulationConcentrationPerception* > T_ConcentrationMap;
    typedef T_ConcentrationMap::iterator                                                                     IT_ConcentrationMap;
    typedef T_ConcentrationMap::const_iterator                                                               CIT_ConcentrationMap;

    typedef std::map< const MIL_PopulationFlow*, DEC_Knowledge_PopulationFlowPerception* > T_FlowMap;
    typedef T_FlowMap::iterator                                                            IT_FlowMap;
    typedef T_FlowMap::const_iterator                                                      CIT_FlowMap;
    //@}

private:
    const MIL_AgentPion*   pAgentPerceiving_;
          MIL_Population*  pPopulationPerceived_;

    T_ConcentrationMap concentrations_;
    T_FlowMap          flows_;
};

#include "DEC_Knowledge_PopulationPerception.inl"

#endif // __DEC_Knowledge_PopulationPerception_h_
