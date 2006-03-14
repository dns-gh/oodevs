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

#include "ASN_Types.h"
#include "ParamComboBox.h"

class ActionController;
class Agent_ABC;
class MissionPanel;
class Agent;
class Param_ABC;

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
             MissionInterface_ABC( Agent_ABC& agent, ActionController& controller );
    virtual ~MissionInterface_ABC();
    //@}

protected:
    //! @name Helpers
    //@{
    void CreateOkCancelButtons();
    void CreateNatureAtlas        ( ASN1T_NatureAtlas& asn         , const std::string& strName, bool bOptional );
    void CreateGDH                ( ASN1T_GDH& asn                 , const std::string& strName, bool bOptional );
    void CreateDirection          ( ASN1T_Direction& asn           , const std::string& strName, bool bOptional );
    void CreatePoint              ( ASN1T_Point& asn               , const std::string& strName, bool bOptional );
    void CreatePath               ( ASN1T_Itineraire& asn          , const std::string& strName, bool bOptional );
    void CreatePathList           ( ASN1T_ListItineraire& asn      , const std::string& strName, bool bOptional );
    void CreateAgentList          ( ASN1T_ListAgent& asn           , const std::string& strName, bool bOptional );
    void CreateAgent              ( ASN1T_Agent& asn               , const std::string& strName, bool bOptional );
    void CreateAutomateList       ( ASN1T_ListAutomate& asn        , const std::string& strName, bool bOptional );
    void CreateAutomate           ( ASN1T_Agent& asn               , const std::string& strName, bool bOptional );
    void CreateLocation           ( ASN1T_Localisation& asn        , const std::string& strName, bool bOptional );
    void CreateLocationList       ( ASN1T_ListLocalisation& asn    , const std::string& strName, bool bOptional );
    void CreatePolygonList        ( ASN1T_ListPolygon& asn         , const std::string& strName, bool bOptional );
    void CreatePointList          ( ASN1T_ListPoint& asn           , const std::string& strName, bool bOptional );
    void CreateAgentKnowledge     ( ASN1T_KnowledgeAgent& asn      , const std::string& strName, bool bOptional );
    void CreateAgentKnowledgeList ( ASN1T_ListKnowledgeAgent& asn  , const std::string& strName, bool bOptional );
    void CreateObjectKnowledge    ( ASN1T_KnowledgeObject& asn     , const std::string& strName, bool bOptional );
    void CreateObjectKnowledgeList( ASN1T_ListKnowledgeObject& asn , const std::string& strName, bool bOptional );
    void CreatePopulationKnowledge( ASN1T_KnowledgePopulation& asn      , const std::string& strName, bool bOptional );
    void CreateGenObject          ( ASN1T_MissionGenObject& asn    , const std::string& strName, bool bOptional );
    void CreateGenObjectList      ( ASN1T_ListMissionGenObject& asn, const std::string& strName, bool bOptional );
    void CreateBool               ( ASN1BOOL& asn                  , const std::string& strName, bool bOptional, QWidget* pParent = 0 );
    void CreateNumeric            ( ASN1INT&  asn                  , const std::string& strName, bool bOptional );
    void CreateNumeric            ( ASN1REAL& asn                  , const std::string& strName, bool bOptional );

    void CreateMaintenancePriorities( ASN1T_MaintenancePriorites& asn, const std::string& strName, bool bOptional );
    void CreateMedicalPriorities    ( ASN1T_SantePriorites&       asn, const std::string& strName, bool bOptional );

    template < class T >
    ParamComboBox< T >& CreateVarList( T& value, const std::string& strName, bool bOptional )
    {
        ParamComboBox< T >* pParam = new ParamComboBox<T>( value, strName, this, bOptional );
        paramVector_.push_back( pParam );
        return *pParam;
    }
    //@}

protected slots:
    virtual void OnOk() = 0;

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
    //@}

private:
    //! @name Member data
    //@{
    ActionController& controller_;
    Agent_ABC& agent_;
    T_Parameters  parameters_;
    //@}
};

#endif // __MissionInterface_ABC_h_
