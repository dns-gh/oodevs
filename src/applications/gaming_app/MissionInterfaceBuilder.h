// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionInterfaceBuilder_h_
#define __MissionInterfaceBuilder_h_

#include "game_asn/Simulation.h"
#include "ParamComboBox.h"

namespace kernel
{
    class ActionController;
    class Entity_ABC;
    class CoordinateConverter_ABC;
    class OrderType;
    class OrderParameter;
}

namespace gui
{
    class ParametersLayer;
}

class StaticModel;
class MissionInterface_ABC;
class AgentKnowledgeConverter_ABC;
class ObjectKnowledgeConverter_ABC;
class Param_ABC;
class Simulation;

// =============================================================================
/** @class  MissionInterfaceBuilder
    @brief  Mission interface builder
*/
// Created: SBO 2006-11-22
// =============================================================================
class MissionInterfaceBuilder
{
private:
    //! @name Types
    //@{
    typedef std::vector< std::pair< int, std::string > > T_Values;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             MissionInterfaceBuilder( kernel::ActionController& controller, gui::ParametersLayer& layer
                                    , AgentKnowledgeConverter_ABC& knowledgeConverter, ObjectKnowledgeConverter_ABC& objectKnowledgeConverter
                                    , const StaticModel& staticModel, const Simulation& simulation );
    virtual ~MissionInterfaceBuilder();
    //@}

    //! @name Operations
    //@{
    void Build( MissionInterface_ABC& missionInterface, kernel::Entity_ABC& entity, const kernel::OrderType& order );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MissionInterfaceBuilder( const MissionInterfaceBuilder& );            //!< Copy constructor
    MissionInterfaceBuilder& operator=( const MissionInterfaceBuilder& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void Build( const kernel::OrderParameter& parameter );

    Param_ABC* BuildAgent                ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildAgentList            ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildAutomat              ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildAutomatList          ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildDotation             ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildAtlasNature          ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildBoolean              ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildDirection            ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildNumeric              ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildAgentKnowledge       ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildAgentKnowledgeList   ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildObjectKnowledge      ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildObjectKnowledgeList  ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildPopulationKnowledge  ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildPath                 ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildPathList             ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildPoint                ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildPointList            ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildPolygon              ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildPolygonList          ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildLocation             ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildLocationList         ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildGenObject            ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildGenObjectList        ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildMissionObjective     ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildMissionObjectiveList ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildMaintenancePriorities( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildMedicalPriorities    ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildEnumeration          ( const kernel::OrderParameter& parameter ) const;

    Param_ABC* BuildLimits               ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildLimaList             ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildDangerousDirection   ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildDotationTypeList     ( const kernel::OrderParameter& parameter ) const;
    Param_ABC* BuildIntelligenceList     ( const kernel::OrderParameter& parameter ) const;

    template < class T >
    ParamComboBox<T>& BuildVarList( const kernel::OrderParameter& parameter ) const
    {
        return *new ParamComboBox<T>( parameter );
    }
    //@}

    //! @name Types
    //@{
    typedef Param_ABC* (MissionInterfaceBuilder::*T_BuilderFunctor)( const kernel::OrderParameter& ) const;
    typedef std::map< QString, T_BuilderFunctor > T_BuilderFunctors;
    typedef T_BuilderFunctors::const_iterator   CIT_BuilderFunctors;
    //@}

private:
    //! @name Member data
    //@{
    kernel::ActionController&              controller_;
    gui::ParametersLayer&                  layer_;
    const kernel::CoordinateConverter_ABC& converter_;
    AgentKnowledgeConverter_ABC&           knowledgeConverter_;
    ObjectKnowledgeConverter_ABC&          objectKnowledgeConverter_;
    const StaticModel&                     staticModel_;
    const Simulation&                      simulation_;
    MissionInterface_ABC*                  missionInterface_;
    kernel::Entity_ABC*                    entity_;
    T_BuilderFunctors                      builderFunctors_;
    //@}
};

#endif // __MissionInterfaceBuilder_h_
