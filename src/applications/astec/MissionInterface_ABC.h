// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-04-20 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/MissionInterface_ABC.h $
// $Author: Nld $
// $Modtime: 27/04/05 18:36 $
// $Revision: 9 $
// $Workfile: MissionInterface_ABC.h $
//
// *****************************************************************************

#ifndef __MissionInterface_ABC_h_
#define __MissionInterface_ABC_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Param_ABC.h"
#include "ASN_Types.h"
#include "ParamComboBox.h"

class ActionContext;
class Agent_ABC;
class MissionPanel;
class Agent;
class OptionalParamFunctor_ABC;

// =============================================================================
/** @class  MissionInterface_ABC
    @brief  MissionInterface_ABC
    @par    Using example
    @code
    MissionInterface_ABC;
    @endcode
*/
// Created: APE 2004-04-20
// =============================================================================
class MissionInterface_ABC : public QVBox
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MissionInterface_ABC );
    friend class GLTool;

public:
    //! @name Constructors/Destructor
    //@{
             MissionInterface_ABC( Agent_ABC& agent, MissionPanel& parentPanel );
    virtual ~MissionInterface_ABC();
    //@}

    //! @name Operations
    //@{
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context );
    bool CheckValidity();
    //@}

protected:
    //! @name Helpers
    //@{
    void CreateOkCancelButtons();

    void CreateNatureAtlas              ( ASN1T_NatureAtlas& asn         , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateGDH                      ( ASN1T_GDH& asn                 , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateDirection                ( ASN1T_Direction& asn           , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreatePoint                    ( ASN1T_Point& asn               , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreatePath                     ( ASN1T_Itineraire& asn          , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreatePathList                 ( ASN1T_ListItineraire& asn      , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateAgentList                ( ASN1T_ListAgent& asn           , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateAgent                    ( ASN1T_Agent& asn               , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateAutomateList             ( ASN1T_ListAutomate& asn        , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateAutomate                 ( ASN1T_Agent& asn               , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateLocation                 ( ASN1T_Localisation& asn        , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateLocationList             ( ASN1T_ListLocalisation& asn    , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreatePolygonList              ( ASN1T_ListPolygon& asn         , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreatePointList                ( ASN1T_ListPoint& asn           , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateAgentKnowledge           ( ASN1T_KnowledgeAgent& asn      , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateAgentKnowledgeList       ( ASN1T_ListKnowledgeAgent& asn  , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateObjectKnowledge          ( ASN1T_KnowledgeObject& asn     , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateObjectKnowledgeList      ( ASN1T_ListKnowledgeObject& asn , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreatePopulationKnowledge      ( ASN1T_KnowledgePopulation& asn , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateGenObject                ( ASN1T_MissionGenObject& asn    , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateGenObjectList            ( ASN1T_ListMissionGenObject& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateBool                     ( ASN1BOOL& asn                  , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 , QWidget* pParent = 0 );
    void CreateNumeric                  ( ASN1INT&  asn                  , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateNumeric                  ( ASN1REAL& asn                  , const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );

    void CreateMaintenancePriorities( ASN1T_MaintenancePriorites& asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );
    void CreateMedicalPriorities    ( ASN1T_SantePriorites&       asn, const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 );

    template < class T >
    ParamComboBox< T >& CreateVarList( T& value, const std::string& strName, OptionalParamFunctor_ABC* pOptional = 0 )
    {
        ParamComboBox< T >* pParam = new ParamComboBox<T>( value, strName, this, pOptional );
        paramVector_.push_back( pParam );
        return *pParam;
    }

    template< typename T, typename U > T* BuildOptionalParamFunctor( U& asnMission )
    {
        T* pTmp = new T( asnMission );
        optionalParamFunctors_.push_back( pTmp );
        return pTmp;
    }
    //@}

protected slots:
    virtual void OnOk() = 0;

protected:
    //! @name Member data
    //@{
    Agent_ABC& agent_;
    MissionPanel& parentPanel_;
    T_ParamVector paramVector_;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< OptionalParamFunctor_ABC* >     T_OptionalParamFunctorVector;
    typedef T_OptionalParamFunctorVector::const_iterator CIT_OptionalParamFunctorVector;
    //@}

private:
    T_OptionalParamFunctorVector    optionalParamFunctors_;
};



#endif // __MissionInterface_ABC_h_
