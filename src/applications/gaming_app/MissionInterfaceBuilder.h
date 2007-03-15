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

#include "game_asn/Asn.h"
#include "ParamComboBox.h"

namespace kernel
{
    class ActionController;
    class Entity_ABC;
    class CoordinateConverter_ABC;
    class ObjectTypes;
}

namespace gui
{
    class ParametersLayer;
}

class MissionInterface_ABC;
class AgentKnowledgeConverter_ABC;
class ObjectKnowledgeConverter_ABC;
class Param_ABC;

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
                                    , const kernel::CoordinateConverter_ABC& converter, AgentKnowledgeConverter_ABC& knowledgeConverter
                                    , ObjectKnowledgeConverter_ABC& objectKnowledgeConverter, const kernel::ObjectTypes& objectTypes );
    virtual ~MissionInterfaceBuilder();
    //@}

    //! @name Operations
    //@{
    void Begin( MissionInterface_ABC& missionInterface, kernel::Entity_ABC& entity );
    void End();

    void AddParameter( const QString& type, const QString& name, bool optional, const T_Values& values );
    void AddOrderContext( bool optional );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MissionInterfaceBuilder( const MissionInterfaceBuilder& );            //!< Copy constructor
    MissionInterfaceBuilder& operator=( const MissionInterfaceBuilder& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    Param_ABC* BuildAgent                ( const QString& name ) const;
    Param_ABC* BuildAgentList            ( const QString& name ) const;
    Param_ABC* BuildAutomat              ( const QString& name ) const;
    Param_ABC* BuildAutomatList          ( const QString& name ) const;
    Param_ABC* BuildDotation             ( const QString& name ) const;
    Param_ABC* BuildAtlasNature          ( const QString& name ) const;
    Param_ABC* BuildBoolean              ( const QString& name ) const;
    Param_ABC* BuildDirection            ( const QString& name ) const;
    Param_ABC* BuildGDH                  ( const QString& name ) const;
    Param_ABC* BuildNumeric              ( const QString& name ) const;
    Param_ABC* BuildAgentKnowledge       ( const QString& name ) const;
    Param_ABC* BuildAgentKnowledgeList   ( const QString& name ) const;
    Param_ABC* BuildObjectKnowledge      ( const QString& name ) const;
    Param_ABC* BuildObjectKnowledgeList  ( const QString& name ) const;
    Param_ABC* BuildPopulationKnowledge  ( const QString& name ) const;
    Param_ABC* BuildPath                 ( const QString& name ) const;
    Param_ABC* BuildPathList             ( const QString& name ) const;
    Param_ABC* BuildPoint                ( const QString& name ) const;
    Param_ABC* BuildPointList            ( const QString& name ) const;
    Param_ABC* BuildPolygon              ( const QString& name ) const;
    Param_ABC* BuildPolygonList          ( const QString& name ) const;
    Param_ABC* BuildLocation             ( const QString& name ) const;
    Param_ABC* BuildLocationList         ( const QString& name ) const;
    Param_ABC* BuildGenObject            ( const QString& name ) const;
    Param_ABC* BuildGenObjectList        ( const QString& name ) const;
    Param_ABC* BuildMaintenancePriorities( const QString& name ) const;
    Param_ABC* BuildMedicalPriorities    ( const QString& name ) const;

    Param_ABC* BuildLimits             ( const QString& name1, const QString& name2 ) const;
    Param_ABC* BuildLimaList           ( const QString& name ) const;
    Param_ABC* BuildEnumeration        ( const QString& name, const T_Values& values );


    template < class T >
    ParamComboBox<T>& BuildVarList( const QString& name )
    {
        return *new ParamComboBox<T>( name );
    }
    //@}

    //! @name Types
    //@{
    typedef Param_ABC* (MissionInterfaceBuilder::*T_BuilderFunctor)( const QString& ) const;
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
    const kernel::ObjectTypes&             objectTypes_;
    MissionInterface_ABC*                  missionInterface_;
    kernel::Entity_ABC*                    entity_;
    T_BuilderFunctors                      builderFunctors_;
    //@}
};

#endif // __MissionInterfaceBuilder_h_
