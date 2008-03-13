// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentTypes_h_
#define __AgentTypes_h_

#include "Resolver.h"

namespace xml { class xistream; };

namespace kernel
{
    class AgentType;
    class ComponentType;
    class DecisionalModel;
    class SensorType;
    class AutomatType;
    class PopulationType;
    class Mission;
    class MissionType;
    class OrderContext;
    class MissionFactory;
    class FragOrderType;
    class SymbolFactory;
    class KnowledgeGroupType;
    class ExerciseConfig;

// =============================================================================
/** @class  AgentTypes
    @brief  AgentTypes
*/
// Created: AGE 2006-02-14
// =============================================================================
class AgentTypes : public Resolver< AgentType >
                 , public Resolver< AgentType, std::string >
                 , public Resolver< PopulationType >
                 , public Resolver< PopulationType, std::string >
                 , public Resolver< AutomatType >
                 , public Resolver< AutomatType, std::string >
                 , public Resolver< ComponentType >
                 , public Resolver< ComponentType, std::string >
                 , public Resolver< SensorType, std::string >
                 , public Resolver< KnowledgeGroupType, std::string >
                 , public Resolver< MissionType >
                 , public Resolver< FragOrderType >
                 , public Resolver< FragOrderType, std::string >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentTypes();
    virtual ~AgentTypes();
    //@}

    //! @name Operations
    //@{
    void Load( const ExerciseConfig& config );
    void Purge();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentTypes( const AgentTypes& );            //!< Copy constructor
    AgentTypes& operator=( const AgentTypes& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef Mission* (MissionFactory::*T_Resolver)( const std::string& );
    typedef Resolver< MissionType, std::string > T_MissionResolver;
    //@}

    //! @name Helpers
    //@{
    void ReadComponents( const std::string& components );
    void ReadSensors( const std::string& sensors );
    void ReadAgents( const std::string& agents );
    void ReadAutomats( const std::string& automats );
    void ReadOrderTypes( const std::string& missions );
    void ReadModels( const std::string& models );
    void ReadPopulations( const std::string& populations );
    void ReadKnowledgeGroups( const std::string& groups );

    void ReadAgentType( xml::xistream& xis );
    void ReadAutomatType( xml::xistream& xis );
    void ReadPopulationType( xml::xistream& xis );
    void ReadKnowledgeGroupType( xml::xistream& xis );
    void ReadComponent( xml::xistream& xis );
    void ReallyReadSensor( xml::xistream& xis, const std::string& sensor );
    void ReadSensor( xml::xistream& xis );
    void ReadModel( xml::xistream& xis, const T_Resolver& missionResolver, Resolver< DecisionalModel, std::string >& models );
    void ReadMissions( xml::xistream& xis, const std::string& name, T_MissionResolver& missions );
    void ReadMissionType( xml::xistream& xis, T_MissionResolver& missions, const OrderContext& context );
    void ReadFragOrderType( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    T_MissionResolver unitMissions_;
    T_MissionResolver automatMissions_;
    T_MissionResolver populationMissions_;
    SymbolFactory* symbolFactory_;
    //@}

public:
    //! @name Member data
    //@{
    Resolver< DecisionalModel, std::string > unitModels_;
    Resolver< DecisionalModel, std::string > automatModels_;
    Resolver< DecisionalModel, std::string > populationModels_;
    //@}
};

}

#endif // __AgentTypes_h_
