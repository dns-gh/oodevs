// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
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
    class MissionFactory;
    class Mission;
    class FragOrder;
    class SymbolFactory;
    class KnowledgeGroupType;

// =============================================================================
/** @class  AgentTypes
    @brief  AgentTypes
*/
// Created: AGE 2006-02-14
// =============================================================================
class AgentTypes : public Resolver< AgentType >
                 , public Resolver< AgentType, QString >
                 , public Resolver< PopulationType >
                 , public Resolver< PopulationType, QString >
                 , public Resolver< AutomatType >
                 , public Resolver< AutomatType, QString >
                 , public Resolver< ComponentType >
                 , public Resolver< ComponentType, QString >
                 , public Resolver< SensorType, QString >
                 , public Resolver< KnowledgeGroupType, QString >
                 , public Resolver< Mission >
                 , public Resolver< FragOrder >
                 , public Resolver< FragOrder, QString >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentTypes();
    virtual ~AgentTypes();
    //@}

    //! @name Operations
    //@{
    void Load( const std::string& scipioXml );
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
    typedef Resolver< Mission, QString > T_MissionResolver;
    //@}

    //! @name Helpers
    //@{
    void ReadComponents( const std::string& components );
    void ReadDecisional( const std::string& decisional );
    void ReadSensors( const std::string& sensors );
    void ReadAgents( const std::string& agents );
    void ReadAutomats( const std::string& automats );
    void ReadMissions( const std::string& missions );
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
    void ReadModel( xml::xistream& xis, const T_Resolver& missionResolver, Resolver< DecisionalModel, QString >& models );
    void ReadMission( xml::xistream& xis, T_MissionResolver& missions );
    void ReadFragOrder( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    T_MissionResolver unitMissions_;
    T_MissionResolver automatMissions_;
    T_MissionResolver populationMissions_;

    MissionFactory* missionFactory_;
    SymbolFactory* symbolFactory_;
    //@}

public:
    //! @name Member data
    //@{
    Resolver< DecisionalModel, QString > unitModels_;
    Resolver< DecisionalModel, QString > automatModels_;
    Resolver< DecisionalModel, QString > populationModels_;
    //@}
};

}

#endif // __AgentTypes_h_
