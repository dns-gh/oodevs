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

#include "tools/Resolver.h"

namespace xml { class xistream; };
namespace tools
{
    class ExerciseConfig;
}

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

// =============================================================================
/** @class  AgentTypes
    @brief  AgentTypes
*/
// Created: AGE 2006-02-14
// =============================================================================
class AgentTypes : public  tools::Resolver< AgentType >
                 , public  tools::Resolver< AgentType, std::string >
                 , public  tools::Resolver< PopulationType >
                 , public  tools::Resolver< PopulationType, std::string >
                 , public  tools::Resolver< AutomatType >
                 , public  tools::Resolver< AutomatType, std::string >
                 , public  tools::Resolver< ComponentType >
                 , public  tools::Resolver< ComponentType, std::string >
                 , public  tools::Resolver< SensorType, std::string >
                 , public  tools::Resolver< KnowledgeGroupType, std::string >
                 , public  tools::Resolver< MissionType >
                 , public  tools::Resolver< FragOrderType >
                 , public  tools::Resolver< FragOrderType, std::string >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentTypes();
    explicit AgentTypes( const tools::ExerciseConfig& config );
    virtual ~AgentTypes();
    //@}

    //! @name Operations
    //@{
    void Load( const tools::ExerciseConfig& config );
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
    typedef  tools::Resolver< MissionType, std::string > T_MissionResolver;
    //@}

    //! @name Helpers
    //@{
    void ReadComponents( xml::xistream& xis );
    void ReadSensors( xml::xistream& xis );
    void ReadAgents( xml::xistream& xis );
    void ReadAutomats( xml::xistream& xis );
    void ReadOrderTypes( xml::xistream& xis );
    void ReadModels( xml::xistream& xis );
    void ReadPopulations( xml::xistream& xis );
    void ReadKnowledgeGroups( xml::xistream& xis );

    void ReadAgentType( xml::xistream& xis );
    void ReadAutomatType( xml::xistream& xis );
    void ReadPopulationType( xml::xistream& xis );
    void ReadKnowledgeGroupType( xml::xistream& xis );
    void ReadComponent( xml::xistream& xis );
    void ReallyReadSensor( xml::xistream& xis, const std::string& sensor );
    void ReadSensor( xml::xistream& xis );
    void ReadModel( xml::xistream& xis, const T_Resolver& missionResolver,  tools::Resolver< DecisionalModel, std::string >& models );
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
     tools::Resolver< DecisionalModel, std::string > unitModels_;
     tools::Resolver< DecisionalModel, std::string > automatModels_;
     tools::Resolver< DecisionalModel, std::string > populationModels_;
    //@}
};

}

#endif // __AgentTypes_h_
