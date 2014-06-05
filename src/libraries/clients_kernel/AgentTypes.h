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

#include "ENT/ENT_Enums.h"
#include <tools/Resolver.h>
#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

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
    class InhabitantType;
    class Mission;
    class MissionType;
    class MissionFactory;
    class FragOrderType;
    class MagicOrderType;
    class SymbolFactory;
    class KnowledgeGroupType;
    class MagicActionType;
    class XmlTranslations;

// =============================================================================
/** @class  AgentTypes
    @brief  AgentTypes
*/
// Created: AGE 2006-02-14
// =============================================================================
class AgentTypes : public tools::Resolver< AgentType >
                 , public tools::StringResolver< AgentType >
                 , public tools::Resolver< PopulationType >
                 , public tools::StringResolver< PopulationType >
                 , public tools::Resolver< InhabitantType >
                 , public tools::StringResolver< InhabitantType >
                 , public tools::Resolver< AutomatType >
                 , public tools::StringResolver< AutomatType >
                 , public tools::Resolver< ComponentType >
                 , public tools::StringResolver< ComponentType >
                 , public tools::StringResolver< SensorType >
                 , public tools::StringResolver< KnowledgeGroupType >
                 , public tools::Resolver< MissionType >
                 , public tools::Resolver< FragOrderType >
                 , public tools::StringResolver< FragOrderType >
                 , public tools::StringResolver< MagicActionType >
                 , private boost::noncopyable
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
    void ReadOrderTypes( xml::xistream& xis );
    void CreateMagicActionTypes();
    //@}

private:
    //! @name Types
    //@{
    typedef Mission* ( MissionFactory::*T_Resolver )( const std::string& ) const;
    typedef tools::StringResolver< MissionType > T_MissionResolver;
    //@}

    //! @name Helpers
    //@{
    void ReadComponents( xml::xistream& xis );
    void ReadSensors( xml::xistream& xis );
    void ReadAgents( xml::xistream& xis );
    void ReadAutomats( xml::xistream& xis );
    void ReadMagicOrderTypes( xml::xistream& xis );
    void ReadModels( xml::xistream& xis );
    void ReadPopulations( xml::xistream& xis );
    void ReadInhabitants( xml::xistream& xis );
    void ReadKnowledgeGroups( xml::xistream& xis );

    void ReadAgentType( xml::xistream& xis );
    void ReadAutomatType( xml::xistream& xis );
    void ReadPopulationType( xml::xistream& xis );
    void ReadInhabitantType( xml::xistream& xis );
    void ReadKnowledgeGroupType( xml::xistream& xis );
    void ReadComponent( xml::xistream& xis );
    void ReallyReadSensor( xml::xistream& xis, const std::string& sensor );
    void ReadSensor( xml::xistream& xis );
    void ReadModel( xml::xistream& xis, const T_Resolver& missionResolver,  tools::Resolver< DecisionalModel, std::string >& models );
    void ReadMissions( xml::xistream& xis, const std::string& name, E_MissionType type );
    void ReadMissionType( xml::xistream& xis, E_MissionType type );
    void ReadFragOrderType( xml::xistream& xis );

    void RegisterActionType( MagicActionType& actionType );
    //@}

private:
    //! @name Member data
    //@{
    T_MissionResolver missions_[ 3 ];
    std::unique_ptr< SymbolFactory > symbolFactory_;
    std::unique_ptr< kernel::XmlTranslations > agentTypesTranslations_;
    //@}

public:
    //! @name Member data
    //@{
     tools::StringResolver< DecisionalModel > unitModels_;
     tools::StringResolver< DecisionalModel > automatModels_;
     tools::StringResolver< DecisionalModel > populationModels_;
    //@}
};

}

#endif // __AgentTypes_h_
