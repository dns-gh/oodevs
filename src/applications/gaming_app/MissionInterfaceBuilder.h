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

#include "actions_gui/ParamComboBox.h"
#include "actions_gui/InterfaceBuilder_ABC.h"

namespace kernel
{
    class AgentKnowledgeConverter_ABC;
    class Controllers;
    class CoordinateConverter_ABC;
    class Entity_ABC;
    class ObjectKnowledgeConverter_ABC;
    class OrderParameter;
    class OrderType;
    class Time_ABC;
}

namespace gui
{
    class ParametersLayer;
}

namespace actions
{
    namespace gui
    {
        class MissionInterface_ABC;
        class Param_ABC;
    }
}

class StaticModel;

// =============================================================================
/** @class  MissionInterfaceBuilder
    @brief  Mission interface builder
*/
// Created: SBO 2006-11-22
// =============================================================================
class MissionInterfaceBuilder : public actions::gui::InterfaceBuilder_ABC
{
private:
    //! @name Types
    //@{
    typedef std::vector< std::pair< int, std::string > > T_Values;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             MissionInterfaceBuilder( kernel::Controllers& controllers, gui::ParametersLayer& layer
                                    , kernel::AgentKnowledgeConverter_ABC& knowledgeConverter, kernel::ObjectKnowledgeConverter_ABC& objectKnowledgeConverter
                                    , const StaticModel& staticModel, const kernel::Time_ABC& simulation );
    virtual ~MissionInterfaceBuilder();
    //@}

    //! @name Operations
    //@{
    void Build( actions::gui::MissionInterface_ABC& missionInterface, kernel::Entity_ABC& entity, const kernel::OrderType& order );
    virtual actions::gui::Param_ABC& Build( const kernel::OrderParameter& parameter, bool isRegistered = true ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionInterfaceBuilder( const MissionInterfaceBuilder& );            //!< Copy constructor
    MissionInterfaceBuilder& operator=( const MissionInterfaceBuilder& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    actions::gui::Param_ABC& BuildAgent                ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildAgentList            ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildAutomat              ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildAutomatList          ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildDotation             ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildAtlasNature          ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildBoolean              ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildDirection            ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildNumeric              ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildNumericList          ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildString               ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildParamDateTime        ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildAgentKnowledge       ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildAgentKnowledgeList   ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildObjectKnowledge      ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildObjectKnowledgeList  ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildPopulationKnowledge  ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildPath                 ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildPathList             ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildPoint                ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildPointList            ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildPolygon              ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildPolygonList          ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildLocation             ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildLocationList         ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildLocationComposite    ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildLocationCompositeList( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildGenObject            ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildGenObjectList        ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildMaintenancePriorities( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildMedicalPriorities    ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildEnumeration          ( const kernel::OrderParameter& parameter ) const;

    actions::gui::Param_ABC& BuildLimit                ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildLima                 ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildLimaList             ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildDotationTypeList     ( const kernel::OrderParameter& parameter ) const;

    actions::gui::Param_ABC& BuildUrbanBlock           ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildResourceNetwork      ( const kernel::OrderParameter& parameter ) const;
    actions::gui::Param_ABC& BuildResourceNetworkList  ( const kernel::OrderParameter& parameter ) const;

    template < class T >
    actions::gui::ParamComboBox<T>& BuildVarList( const kernel::OrderParameter& parameter ) const
    {
        return *new actions::gui::ParamComboBox<T>( parameter );
    }
    //@}

    //! @name Types
    //@{
    typedef actions::gui::Param_ABC& (MissionInterfaceBuilder::*T_BuilderFunctor)( const kernel::OrderParameter& ) const;
    typedef std::map< QString, T_BuilderFunctor > T_BuilderFunctors;
    typedef T_BuilderFunctors::const_iterator   CIT_BuilderFunctors;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers&                   controllers_;
    gui::ParametersLayer&                  layer_;
    const kernel::CoordinateConverter_ABC& converter_;
    kernel::AgentKnowledgeConverter_ABC&   knowledgeConverter_;
    kernel::ObjectKnowledgeConverter_ABC&  objectKnowledgeConverter_;
    const StaticModel&                     staticModel_;
    const kernel::Time_ABC&                simulation_;
    actions::gui::MissionInterface_ABC*    missionInterface_;
    kernel::Entity_ABC*                    entity_;
    T_BuilderFunctors                      builderFunctors_;
    //@}
};

#endif // __MissionInterfaceBuilder_h_
