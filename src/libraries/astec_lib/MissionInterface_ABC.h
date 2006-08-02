// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionInterface_ABC_h_
#define __MissionInterface_ABC_h_

#include "ASN_Types.h"
#include "ParamComboBox.h"
#include "ParamRadioBtnGroup.h"

class ActionController;
class Agent_ABC;
class Agent;
class Param_ABC;
class ParametersLayer;
class CoordinateConverter_ABC;
class GlTools_ABC;
class AgentKnowledgeConverter_ABC;
class ObjectTypes;
class OptionalParamFunctor_ABC;

// =============================================================================
/** @class  MissionInterface_ABC
    @brief  MissionInterface_ABC
*/
// Created: APE 2004-04-20
// =============================================================================
class MissionInterface_ABC : public QVBox
{
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             MissionInterface_ABC( QWidget* parent, Agent_ABC& agent, ActionController& controller, ParametersLayer& layer, 
                                   const CoordinateConverter_ABC& converter, AgentKnowledgeConverter_ABC& knowledgeConverter,
                                   const ObjectTypes& objectTypes );
    virtual ~MissionInterface_ABC();
    //@}

    //! @name Operations
    //@{
    bool CheckValidity();
    void Draw( const GlTools_ABC& tools, const geometry::Rectangle2f& extent ) const;
    bool IsEmpty() const;
    //@}

public slots:
    //! @name Slots
    //@{
    virtual void OnOk() = 0;
    //@}

protected:
    //! @name Helpers
    //@{
    void CreateOkCancelButtons();
    void CreateNatureAtlas        ( ASN1T_NatureAtlas& asn         , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateGDH                ( ASN1T_GDH& asn                 , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateDirection          ( ASN1T_Direction& asn           , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreatePoint              ( ASN1T_Point& asn               , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreatePath               ( ASN1T_Itineraire& asn          , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreatePathList           ( ASN1T_ListItineraire& asn      , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateAgentList          ( ASN1T_ListAgent& asn           , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateAgent              ( ASN1T_Agent& asn               , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateAutomateList       ( ASN1T_ListAutomate& asn        , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateAutomate           ( ASN1T_Agent& asn               , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateLocation           ( ASN1T_Localisation& asn        , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateLocationList       ( ASN1T_ListLocalisation& asn    , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreatePolygonList        ( ASN1T_ListPolygon& asn         , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreatePointList          ( ASN1T_ListPoint& asn           , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateAgentKnowledge     ( ASN1T_KnowledgeAgent& asn      , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateAgentKnowledgeList ( ASN1T_ListKnowledgeAgent& asn  , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateObjectKnowledge    ( ASN1T_KnowledgeObject& asn     , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateObjectKnowledgeList( ASN1T_ListKnowledgeObject& asn , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreatePopulationKnowledge( ASN1T_KnowledgePopulation& asn , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateGenObject          ( ASN1T_MissionGenObject& asn    , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateGenObjectList      ( ASN1T_ListMissionGenObject& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateBool               ( ASN1BOOL& asn                  , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0, QWidget* pParent = 0 );
    void CreateNumeric            ( ASN1INT&  asn                  , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateNumeric            ( ASN1REAL& asn                  , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateLimaList           ( ASN1T_ListOID& asn             , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateLimits             ( ASN1T_OID& id1, ASN1T_OID& id2 , const std::string& name1, const std::string& name2, OptionalParamFunctor_ABC* pOptional = 0 );

    void CreateMaintenancePriorities( ASN1T_MaintenancePriorites& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateMedicalPriorities    ( ASN1T_SantePriorites&       asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );

    template < class T >
    ParamComboBox< T >& CreateVarList( T& value, const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 )
    {
        ParamComboBox< T >* pParam = new ParamComboBox<T>( this, value, strName );
        AddParameter( *pParam, pOptional  );
        return *pParam;
    }

    template < class T >
    ParamRadioBtnGroup< T >& CreateRadioButtonGroup( T& value, const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 )
    {
        ParamRadioBtnGroup< T >* pParam = new ParamRadioBtnGroup<T>( this, value, strName );
        AddParameter( *pParam, pOptional );
        return *pParam;
    }

    template< typename T, typename U > T* BuildOptionalParamFunctor( U& asnMission )
    {
        T* result = new T( asnMission );
        optionalFunctors_.push_back( result );
        return result;
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionInterface_ABC( const MissionInterface_ABC& );
    MissionInterface_ABC& operator=( const MissionInterface_ABC& );
    //@}

    //! @name Types
    //@{
    typedef std::vector< Param_ABC* >      T_Parameters;
    typedef T_Parameters::const_iterator CIT_Parameters;

    typedef std::vector< OptionalParamFunctor_ABC* >  T_OptionalFunctors;
    typedef T_OptionalFunctors::const_iterator      CIT_OptionalFunctors;
    //@}

protected:
    //! @name Helpers
    //@{
    void AddParameter( Param_ABC& parameter, OptionalParamFunctor_ABC* pOptional );
    void Commit();
    //@}

private:
    //! @name Member data
    //@{
    ActionController& controller_;
    Agent_ABC& agent_;
    ParametersLayer& layer_;
    const CoordinateConverter_ABC& converter_;
    AgentKnowledgeConverter_ABC& knowledgeConverter_;
    const ObjectTypes& objectTypes_;

    T_Parameters  parameters_;
    T_OptionalFunctors optionalFunctors_;
};

#endif // __MissionInterface_ABC_h_
