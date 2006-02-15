//*****************************************************************************
//
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/PopulationKnowledge.h $
// $Author: Age $
// $Modtime: 31/03/05 17:36 $
// $Revision: 5 $
// $Workfile: PopulationKnowledge.h $
//
//*****************************************************************************

#ifndef __PopulationKnowledge_h_
#define __PopulationKnowledge_h_

#include "ASN_Types.h"
#include "IDManager.h"
#include "Resolver.h"

class Controller;
class KnowledgeGroup;
class Team;
class Population;
class PopulationConcentrationKnowledge;
class PopulationFlowKnowledge;
class TypePopulation;

// =============================================================================
// Created: APE 2004-03-10
// =============================================================================
class PopulationKnowledge : public Resolver< PopulationConcentrationKnowledge >
                          , public Resolver< PopulationFlowKnowledge >
{
public:
    //! @name Constructor/Destructor
    //@{
             PopulationKnowledge( Controller& controller, const ASN1T_MsgPopulationKnowledgeCreation& asnMsg );
    virtual ~PopulationKnowledge();
    //@}

    //! @name Accessors
    //@{
    unsigned long GetId() const;
//    const Team*                    GetTeam          () const;
//    const Population&              GetPopulation    () const;
//          KnowledgeGroup*                    GetKnowledgeGroup          () const;
//    const TypePopulation&          GetType          () const;
    //@}

    //! @name Network
    //@{
    void Update( const ASN1T_MsgPopulationKnowledgeUpdate& asnMsg );

    void Update( const ASN1T_MsgPopulationConcentrationKnowledgeCreation&    asnMsg );
    void Update( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate&      asnMsg );
    void Update( const ASN1T_MsgPopulationConcentrationKnowledgeDestruction& asnMsg );
    void Update( const ASN1T_MsgPopulationFluxKnowledgeCreation&             asnMsg );
    void Update( const ASN1T_MsgPopulationFluxKnowledgeUpdate&               asnMsg );
    void Update( const ASN1T_MsgPopulationFluxKnowledgeDestruction&          asnMsg );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationKnowledge( const PopulationKnowledge& );
    PopulationKnowledge& operator=( const PopulationKnowledge& );
    //@}

private:
    Controller& controller_;
    unsigned long nID_;
};

#endif // __PopulationKnowledge_h_