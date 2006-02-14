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
class AgentType;
class ComponentType;
class DecisionalModel;
class SensorType;
class AutomatType;

// =============================================================================
/** @class  AgentTypes
    @brief  AgentTypes
*/
// Created: AGE 2006-02-14
// =============================================================================
class AgentTypes : public Resolver< AgentType >
                 , public Resolver< AgentType, std::string >
                 , public Resolver< AutomatType >
                 , public Resolver< ComponentType, std::string >
                 , public Resolver< SensorType, std::string >
                 , public Resolver< DecisionalModel, std::string >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentTypes( xml::xistream& xis );
    virtual ~AgentTypes();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentTypes( const AgentTypes& );            //!< Copy constructor
    AgentTypes& operator=( const AgentTypes& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef unsigned long (*T_Resolver)( const std::string& );
    //@}

    //! @name Helpers
    //@{
    void ReadComponents( const std::string& components );
    void ReadDecisional( const std::string& decisional );
    void ReadSensors( const std::string& sensors );
    void ReadAgents( const std::string& agents );
    void ReadAutomats( const std::string& automats );
    void ReadModels( const std::string& models );

    void ReadAgentType( xml::xistream& xis );
    void ReadAutomatType( xml::xistream& xis );
    void ReadComponent( xml::xistream& xis );
    void ReadSensor( xml::xistream& xis );
    void ReadModel( xml::xistream& xis, const T_Resolver& missionResolver );
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __AgentTypes_h_
