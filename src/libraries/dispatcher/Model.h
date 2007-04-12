// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Model_h_
#define __Model_h_

#include "game_asn/Asn.h"
#include "ModelsContainer.h"

namespace DIN
{
    class DIN_Input;
}

namespace dispatcher
{

class ModelVisitor_ABC;
class Side;
class KnowledgeGroup;
class Formation;
class Automat;
class Agent;    
class Object;
class AgentKnowledge;
class ObjectKnowledge;
class LogConsignMaintenance;
class LogConsignSupply;
class LogConsignMedical;
class Population;
class PopulationKnowledge;
class SimulationModel;
class Publisher_ABC;
class Limit;
class Lima;
class Synchronisable;

// =============================================================================
/** @class  Model
    @brief  Model
*/
// Created: NLD 2006-09-19
// =============================================================================
class Model
{
public:
    //! @name Constructors/Destructor
    //@{
             Model();
    virtual ~Model();
    //@}

    //! @name Update
    //@{
    void Reset();

    void Update( const ASN1T_MsgsOutSim& asnMsg );
    void Update( const ASN1T_MsgsInClient& asnMsg );
    void Update( uint nMsgID, DIN::DIN_Input& input );

    void Send  ( Publisher_ABC& publisher ) const;
    //@}

    //! @name Operations
    //@{
    void Accept( ModelVisitor_ABC& visitor ) const;
    void StartSynchronisation( Publisher_ABC& publisher );
    void EndSynchronisation();

    void FlagForDestruction( Synchronisable& synch );
    //@}

    //! @name Accessors
    //@{
          ModelsContainer< Side           >& GetSides          ();
          ModelsContainer< KnowledgeGroup >& GetKnowledgeGroups();
          ModelsContainer< Formation      >& GetFormations     ();
    const ModelsContainer< Formation      >& GetFormations     () const;
          ModelsContainer< Automat        >& GetAutomats       ();
    const ModelsContainer< Automat        >& GetAutomats       () const;
          ModelsContainer< Agent          >& GetAgents         ();
    const ModelsContainer< Agent          >& GetAgents         () const;
          ModelsContainer< Object         >& GetObjects        ();
          ModelsContainer< Population     >& GetPopulations    ();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Model( const Model& );            //!< Copy constructor
    Model& operator=( const Model& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    template< typename T, typename P >
    void CreateUpdate( ModelsContainer< T >& container, unsigned id, const P& parameter );
    //@}

private:
    SimulationModel*    pSimulationModel_;

    ModelsContainer< Side                   > sides_;
    ModelsContainer< KnowledgeGroup         > knowledgeGroups_;
    ModelsContainer< Formation              > formations_;
    ModelsContainer< Automat                > automats_;
    ModelsContainer< Agent                  > agents_;
    ModelsContainer< Object                 > objects_;
    ModelsContainer< AgentKnowledge         > agentKnowledges_;
    ModelsContainer< ObjectKnowledge        > objectKnowledges_;
    ModelsContainer< LogConsignMaintenance  > logConsignsMaintenance_;
    ModelsContainer< LogConsignSupply       > logConsignsSupply_;
    ModelsContainer< LogConsignMedical      > logConsignsMedical_;
    ModelsContainer< Population             > populations_;
    ModelsContainer< PopulationKnowledge    > populationKnowledges_;
    ModelsContainer< Limit                  > limits_;
    ModelsContainer< Lima                   > limas_;

    std::vector< Synchronisable* > toFlush_;
    Publisher_ABC* synchroniser_;
};

}

#include "Model.inl"

#endif // __Model_h_
