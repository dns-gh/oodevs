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

#include "gaming/ASN_Types.h"
#include "ParamComboBox.h"
#include "ParamRadioBtnGroup.h"

namespace kernel
{
    class ActionController;
    class Entity_ABC;
    class Agent_ABC;
    class CoordinateConverter_ABC;
    class GlTools_ABC;
    class ObjectTypes;
}

namespace gui
{
    class ParametersLayer;
}

class Param_ABC;
class AgentKnowledgeConverter_ABC;
class ObjectKnowledgeConverter_ABC;
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
             MissionInterface_ABC( QWidget* parent, kernel::Entity_ABC& agent, kernel::ActionController& controller, gui::ParametersLayer& layer, 
                                   const kernel::CoordinateConverter_ABC& converter, 
                                   AgentKnowledgeConverter_ABC& knowledgeConverter, ObjectKnowledgeConverter_ABC& objectKnowledgeConverter, 
                                   const kernel::ObjectTypes& objectTypes );
    virtual ~MissionInterface_ABC();
    //@}

    //! @name Operations
    //@{
    bool CheckValidity();
    void Draw( const kernel::GlTools_ABC& tools, const geometry::Rectangle2f& extent ) const;
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
    void CreateNatureAtlas        ( ASN1T_NatureAtlas& asn         , const QString& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateGDH                ( ASN1T_GDH& asn                 , const QString& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateDirection          ( ASN1T_Direction& asn           , const QString& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreatePoint              ( ASN1T_Point& asn               , const QString& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreatePath               ( ASN1T_Itineraire& asn          , const QString& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreatePathList           ( ASN1T_ListItineraire& asn      , const QString& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateAgentList          ( ASN1T_ListAgent& asn           , const QString& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateAgent              ( ASN1T_Agent& asn               , const QString& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateAutomateList       ( ASN1T_ListAutomate& asn        , const QString& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateAutomate           ( ASN1T_Agent& asn               , const QString& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateLocation           ( ASN1T_Localisation& asn        , const QString& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateLocationList       ( ASN1T_ListLocalisation& asn    , const QString& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreatePolygonList        ( ASN1T_ListPolygon& asn         , const QString& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreatePointList          ( ASN1T_ListPoint& asn           , const QString& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateAgentKnowledge     ( ASN1T_KnowledgeAgent& asn      , const QString& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateAgentKnowledgeList ( ASN1T_ListKnowledgeAgent& asn  , const QString& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateObjectKnowledge    ( ASN1T_KnowledgeObject& asn     , const QString& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateObjectKnowledgeList( ASN1T_ListKnowledgeObject& asn , const QString& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreatePopulationKnowledge( ASN1T_KnowledgePopulation& asn , const QString& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateGenObject          ( ASN1T_MissionGenObject& asn    , const QString& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateGenObjectList      ( ASN1T_ListMissionGenObject& asn, const QString& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateBool               ( ASN1BOOL& asn                  , const QString& strName, OptionalParamFunctor_ABC* pOptional = 0, QWidget* pParent = 0 );
    void CreateNumeric            ( ASN1INT&  asn                  , const QString& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateNumeric            ( ASN1REAL& asn                  , const QString& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateLimaList           ( ASN1T_LimasOrder& asn          , const QString& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateLimits             ( ASN1T_Line& left, ASN1T_Line& right, const QString& name1, const QString& name2, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateDotationDType      ( ASN1T_TypeDotationTrancheD& asn, const QString& name, OptionalParamFunctor_ABC* pOptional = 0 );

    void CreateMaintenancePriorities( ASN1T_MaintenancePriorites& asn, const QString& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateMedicalPriorities    ( ASN1T_SantePriorites&       asn, const QString& strName, OptionalParamFunctor_ABC* pOptional = 0 );

    template < class T >
    ParamComboBox< T >& CreateVarList( T& value, const QString& strName, OptionalParamFunctor_ABC* pOptional = 0 )
    {
        ParamComboBox< T >* pParam = new ParamComboBox<T>( this, value, strName );
        AddParameter( *pParam, pOptional  );
        return *pParam;
    }

    template < class T >
    ParamRadioBtnGroup< T >& CreateRadioButtonGroup( T& value, const QString& strName, OptionalParamFunctor_ABC* pOptional = 0 )
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
    kernel::ActionController& controller_;
    kernel::Entity_ABC& agent_;
    gui::ParametersLayer& layer_;
    const kernel::CoordinateConverter_ABC& converter_;
    AgentKnowledgeConverter_ABC& knowledgeConverter_;
    ObjectKnowledgeConverter_ABC& objectKnowledgeConverter_;
    const kernel::ObjectTypes& objectTypes_;

    T_Parameters  parameters_;
    T_OptionalFunctors optionalFunctors_;
};

#endif // __MissionInterface_ABC_h_
