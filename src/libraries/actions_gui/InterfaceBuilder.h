// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __InterfaceBuilder_h_
#define __InterfaceBuilder_h_

#include <assert.h>
#include "InterfaceBuilder_ABC.h"

namespace kernel
{
    class AgentKnowledgeConverter_ABC;
    class Controllers;
    class CoordinateConverter_ABC;
    class Entity_ABC;
    class ObjectKnowledgeConverter_ABC;
    class OrderParameter;
    class OrderType;
    class TacticalLine_ABC;
    class Time_ABC;
    class StaticModel;
}

namespace gui
{
    class ParametersLayer;
}

namespace actions
{
    namespace gui
    {

class MissionInterface;
class Param_ABC;

// =============================================================================
/** @class  InterfaceBuilder
    @brief  InterfaceBuilder
*/
// Created: ABR 2012-01-10
// =============================================================================
class InterfaceBuilder : public actions::gui::InterfaceBuilder_ABC
{
private:
    //! @name Types
    //@{
    typedef std::vector< std::pair< int, std::string > > T_Values;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             InterfaceBuilder( kernel::Controllers& controllers,
                               const tools::ExerciseConfig& config,
                               ::gui::ParametersLayer& layer,
                               const kernel::StaticModel& staticModel,
                               kernel::AgentKnowledgeConverter_ABC* knowledgeConverter = 0,
                               kernel::ObjectKnowledgeConverter_ABC* objectKnowledgeConverter = 0,
                               const kernel::Time_ABC* simulation = 0,
                               tools::Resolver< kernel::TacticalLine_ABC >* tacticalLineResolver = 0,
                               tools::Resolver< kernel::Pathfind_ABC >* pathfindResolver = 0 );
    virtual ~InterfaceBuilder();
    //@}

    //! @name Operations
    //@{
    virtual void BuildAll( actions::gui::MissionInterface& missionInterface, const kernel::OrderType& order );
    virtual actions::gui::Param_ABC& BuildOne( const kernel::OrderParameter& parameter, bool isRegistered = true ) const;
    //@}

    //! @name Getters
    //@{
    virtual QObject* GetParentObject() const;
    virtual ParamInterface_ABC& GetParamInterface() const;
    virtual ::gui::ParametersLayer& GetParameterLayer() const;
    virtual kernel::Controllers& GetControllers() const;
    virtual kernel::AgentKnowledgeConverter_ABC* GetAgentKnowledgeConverter() const;
    virtual kernel::ObjectKnowledgeConverter_ABC* GetObjectKnowledgeConverter() const;
    virtual tools::Resolver< kernel::TacticalLine_ABC >* GetTacticalLineResolver() const;
    virtual tools::Resolver< kernel::Pathfind_ABC >& GetPathfindResolver() const;
    virtual const QDateTime GetCurrentDateTime() const;
    virtual const kernel::StaticModel& GetStaticModel() const;
    virtual const tools::ExerciseConfig& GetConfig() const;
    //@}

    //! @name Setters
    //@{
    virtual void SetParentObject( QObject* parent );
    virtual void SetParamInterface( ParamInterface_ABC& paramInterface );
    //@}

private:
    //! @name Helpers
    //@{
    template< typename ParameterType >
    void AddFunctor( const std::string& typeName );

    template< typename Element >
    actions::gui::Param_ABC& BuildElement( const kernel::OrderParameter& parameter ) const;
    //@}

    //! @name Types
    //@{
    typedef actions::gui::Param_ABC& (InterfaceBuilder::*T_BuilderFunctor)( const kernel::OrderParameter& ) const;
    typedef std::map< std::string, T_BuilderFunctor > T_BuilderFunctors;
    typedef T_BuilderFunctors::const_iterator   CIT_BuilderFunctors;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers&                   controllers_;
    const tools::ExerciseConfig&           config_;
    ::gui::ParametersLayer&                layer_;
    const kernel::StaticModel&             staticModel_;
    kernel::AgentKnowledgeConverter_ABC*   knowledgeConverter_;
    kernel::ObjectKnowledgeConverter_ABC*  objectKnowledgeConverter_;
    const kernel::Time_ABC*                simulation_;
    tools::Resolver< kernel::TacticalLine_ABC >* tacticalLineResolver_;
    tools::Resolver< kernel::Pathfind_ABC >* pathfindResolver_;

    actions::gui::MissionInterface*        missionInterface_;
    QObject*                               parentObject_;
    ParamInterface_ABC*                    paramInterface_;
    T_BuilderFunctors                      builderFunctors_;
    //@}
};

    }
}

#endif // __InterfaceBuilder_h_
