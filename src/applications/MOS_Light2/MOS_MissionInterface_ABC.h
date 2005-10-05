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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_MissionInterface_ABC.h $
// $Author: Nld $
// $Modtime: 27/04/05 18:36 $
// $Revision: 9 $
// $Workfile: MOS_MissionInterface_ABC.h $
//
// *****************************************************************************

#ifndef __MOS_MissionInterface_ABC_h_
#define __MOS_MissionInterface_ABC_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_Param_ABC.h"
#include "MOS_ASN_Types.h"
#include "MOS_ParamComboBox.h"

class MOS_ActionContext;
class MOS_Agent_ABC;
class MOS_MissionPanel;
class MOS_Agent;



// =============================================================================
/** @class  MOS_MissionInterface_ABC
    @brief  MOS_MissionInterface_ABC
    @par    Using example
    @code
    MOS_MissionInterface_ABC;
    @endcode
*/
// Created: APE 2004-04-20
// =============================================================================
class MOS_MissionInterface_ABC : public QVBox
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_MissionInterface_ABC );
    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor
    //@{
             MOS_MissionInterface_ABC( MOS_Agent_ABC& agent, MOS_MissionPanel& parentPanel );
    virtual ~MOS_MissionInterface_ABC();
    //@}

    //! @name Operations
    //@{
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context );
    bool CheckValidity();
    //@}

protected:
    //! @name Helpers
    //@{
    void CreateOkCancelButtons();

    void CreateNatureAtlas        ( ASN1T_NatureAtlas& asn         , const std::string& strName );
    void CreateGDH                ( ASN1T_GDH& asn                 , const std::string& strName );
    void CreateDirection          ( ASN1T_Direction& asn           , const std::string& strName );
    void CreatePoint              ( ASN1T_Point& asn               , const std::string& strName );
    void CreatePath               ( ASN1T_Itineraire& asn          , const std::string& strName );
    void CreatePathList           ( ASN1T_ListItineraire& asn      , const std::string& strName );
    void CreateAgentList          ( ASN1T_ListAgent& asn           , const std::string& strName );
    void CreateAgent              ( ASN1T_Agent& asn               , const std::string& strName );
    void CreateAutomateList       ( ASN1T_ListAutomate& asn        , const std::string& strName );
    void CreateAutomate           ( ASN1T_Agent& asn               , const std::string& strName );
    void CreateLocation           ( ASN1T_Localisation& asn        , const std::string& strName );
    void CreateLocationList       ( ASN1T_ListLocalisation& asn    , const std::string& strName );
    void CreatePolygonList        ( ASN1T_ListPolygon& asn         , const std::string& strName );
    void CreatePointList          ( ASN1T_ListPoint& asn           , const std::string& strName );
    void CreateAgentKnowledge     ( ASN1T_KnowledgeAgent& asn      , const std::string& strName );
    void CreateAgentKnowledgeList ( ASN1T_ListKnowledgeAgent& asn  , const std::string& strName );
    void CreateObjectKnowledge    ( ASN1T_KnowledgeObject& asn     , const std::string& strName );
    void CreateObjectKnowledgeList( ASN1T_ListKnowledgeObject& asn , const std::string& strName );
    void CreateGenObject          ( ASN1T_MissionGenObject& asn    , const std::string& strName );
    void CreateGenObjectList      ( ASN1T_ListMissionGenObject& asn, const std::string& strName );
    void CreateBool               ( ASN1BOOL& asn                  , const std::string& strName, QWidget* pParent = 0 );
    void CreateNumeric            ( ASN1INT&  asn                  , const std::string& strName );
    void CreateNumeric            ( ASN1REAL& asn                  , const std::string& strName );

    void CreateMaintenancePriorities( ASN1T_MaintenancePriorites& asn, const std::string& strName );
    void CreateMedicalPriorities    ( ASN1T_SantePriorites&       asn, const std::string& strName );

    template < class T >
    MOS_ParamComboBox< T >& CreateVarList( T& value, const std::string& strName )
    {
        MOS_ParamComboBox< T >* pParam = new MOS_ParamComboBox<T>( value, strName, this );
        paramVector_.push_back( pParam );
        return *pParam;
    }
    //@}

protected slots:
    virtual void OnOk() = 0;

protected:
    //! @name Member data
    //@{
    MOS_Agent_ABC& agent_;
    MOS_MissionPanel& parentPanel_;
    T_ParamVector paramVector_;
    //@}
};

#   include "MOS_MissionInterface_ABC.inl"

#endif // __MOS_MissionInterface_ABC_h_
