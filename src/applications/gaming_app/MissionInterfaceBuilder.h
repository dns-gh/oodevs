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

#include "gaming/ASN_Types.h"
#include "ParamComboBox.h"
#include "ParamRadioBtnGroup.h"

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

    void AddParameter( ASN1T_MissionParameter& asnParameter, const QString& type, const QString& name, bool optional, const T_Values& values );
    void AddParameter( ASN1T_OrderContext& context, bool optional );
    void AddParameter( ASN1T_EnumAutomateOrderFormation& formation, bool optional );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MissionInterfaceBuilder( const MissionInterfaceBuilder& );            //!< Copy constructor
    MissionInterfaceBuilder& operator=( const MissionInterfaceBuilder& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    Param_ABC* BuildAgent              ( ASN1T_MissionParameter& asnParameter, const QString& name ) const;
    Param_ABC* BuildAgentList          ( ASN1T_MissionParameter& asnParameter, const QString& name ) const;
    Param_ABC* BuildAutomat            ( ASN1T_MissionParameter& asnParameter, const QString& name ) const;
    Param_ABC* BuildAutomatList        ( ASN1T_MissionParameter& asnParameter, const QString& name ) const;
    Param_ABC* BuildDotation           ( ASN1T_MissionParameter& asnParameter, const QString& name ) const;
    Param_ABC* BuildAtlasNature        ( ASN1T_MissionParameter& asnParameter, const QString& name ) const;
    Param_ABC* BuildBoolean            ( ASN1T_MissionParameter& asnParameter, const QString& name ) const;
    Param_ABC* BuildDirection          ( ASN1T_MissionParameter& asnParameter, const QString& name ) const;
    Param_ABC* BuildGDH                ( ASN1T_MissionParameter& asnParameter, const QString& name ) const;
    Param_ABC* BuildNumeric            ( ASN1T_MissionParameter& asnParameter, const QString& name ) const;
    Param_ABC* BuildAgentKnowledge     ( ASN1T_MissionParameter& asnParameter, const QString& name ) const;
    Param_ABC* BuildAgentKnowledgeList ( ASN1T_MissionParameter& asnParameter, const QString& name ) const;
    Param_ABC* BuildObjectKnowledge    ( ASN1T_MissionParameter& asnParameter, const QString& name ) const;
    Param_ABC* BuildObjectKnowledgeList( ASN1T_MissionParameter& asnParameter, const QString& name ) const;
    Param_ABC* BuildPopulationKnowledge( ASN1T_MissionParameter& asnParameter, const QString& name ) const;
    Param_ABC* BuildPath               ( ASN1T_MissionParameter& asnParameter, const QString& name ) const;
    Param_ABC* BuildPathList           ( ASN1T_MissionParameter& asnParameter, const QString& name ) const;
    Param_ABC* BuildPoint              ( ASN1T_MissionParameter& asnParameter, const QString& name ) const;
    Param_ABC* BuildPointList          ( ASN1T_MissionParameter& asnParameter, const QString& name ) const;
    Param_ABC* BuildPolygon            ( ASN1T_MissionParameter& asnParameter, const QString& name ) const;
    Param_ABC* BuildPolygonList        ( ASN1T_MissionParameter& asnParameter, const QString& name ) const;
    Param_ABC* BuildLocation           ( ASN1T_MissionParameter& asnParameter, const QString& name ) const;
    Param_ABC* BuildLocationList       ( ASN1T_MissionParameter& asnParameter, const QString& name ) const;
    Param_ABC* BuildGenObject          ( ASN1T_MissionParameter& asnParameter, const QString& name ) const;
    Param_ABC* BuildGenObjectList      ( ASN1T_MissionParameter& asnParameter, const QString& name ) const;
    Param_ABC* BuildMaintenancePriorities( ASN1T_MissionParameter& asnParameter, const QString& name ) const;
    Param_ABC* BuildMedicalPriorities    ( ASN1T_MissionParameter& asnParameter, const QString& name ) const;

    Param_ABC* BuildLimits             ( ASN1T_Line& limit1, ASN1T_Line& limit2, const QString& name1, const QString& name2 ) const;
    Param_ABC* BuildLimaList           ( ASN1T_LimasOrder& asn, const QString& name ) const;
    Param_ABC* BuildDirection          ( ASN1T_Direction&  asn, const QString& name ) const;
    Param_ABC* BuildEnumeration        ( ASN1T_MissionParameter& asnParameter, const QString& name, const T_Values& values );


    template < class T >
    ParamComboBox<T>& BuildVarList( T& value, const QString& name )
    {
        return *new ParamComboBox<T>( missionInterface_, value, name );
    }

    template < class T >
    ParamRadioBtnGroup<T>& BuildRadioButtonGroup( T& value, const QString& name )
    {
        return *new ParamRadioBtnGroup<T>( missionInterface_, value, name );
    }
    //@}

    //! @name Types
    //@{
    typedef Param_ABC* (MissionInterfaceBuilder::*T_BuilderFunctor)( ASN1T_MissionParameter&, const QString& ) const;
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
