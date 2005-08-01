//****************************************************************************
// 
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Mission_ABC.h $
// $Author: Nld $
// $Modtime: 14/02/05 14:46 $
// $Revision: 72 $
// $Workfile: MOS_Mission_ABC.h $
// 
//*****************************************************************************

#ifndef __MOS_Mission_ABC_h_
#define __MOS_Mission_ABC_h_

#include "MOS_Types.h"
#include "MOS_Order_Def.h"
#include "MOS_GLMouse_ABC.h"
#include "MOS_ValueButtonList.h"
#include "MOS_ASN_Types.h"
#include "MOS_Tools.h"

#include <QWidget.h>
#include <QTabWidget.h>
#include <QLabel.h>

class MOS_Agent;
class MOS_LimaMission;
class MOS_LimitMission;
class MOS_MTH_Itineraire1Pos;
class MOS_MTH_ItinerairePos;
class MOS_AgentListView;
class MOS_AgentMission;
class MOS_Value;
class MOS_ValueList;
class MOS_MTH_SegmentPos;
class MOS_MTH_DirectionPos;
class MOS_MTH_GDH;
class MOS_MTH_AgentKnowledgeList;
class MOS_MTH_ObjectKnowledgeList;
class MOS_MTH_Localisation;
class MOS_MTH_ListLocalisation_ListView;
class MOS_MTH_PointList_ListView;
class MOS_MTH_Obstacle_ListView;
class MOS_MTH_Building_ListView;
class MOS_MTH_NatureAtlas;
class MOS_MTH_Obstacle;
class MOS_MTH_ListPoint;
class MOS_MTH_MaintenancePriorities;
class MOS_MTH_MedicalPriorities;
class MOS_MissionAction;

class QPushButton;
class QVBoxLayout;

class MOS_MissionTool_ABC;

//=============================================================================
// Created:  NLD 2002-01-03
//=============================================================================

class MOS_Mission_ABC : public QWidget
                      , public MOS_GLMouse_ABC
{
    Q_OBJECT

    MT_COPYNOTALLOWED( MOS_Mission_ABC );

public:
    typedef std::vector< MOS_MissionTool_ABC* >             T_MissionToolPtrVector;
    typedef const T_MissionToolPtrVector                   CT_MissionToolPtrVector;
    typedef T_MissionToolPtrVector::iterator               IT_MissionToolPtrVector;
    typedef T_MissionToolPtrVector::const_iterator         CIT_MissionToolPtrVector;
    typedef T_MissionToolPtrVector::reverse_iterator       RIT_MissionToolPtrVector;

public:
             MOS_Mission_ABC( E_MissionID nTypeMission, QWidget* pParent = 0 );
    virtual ~MOS_Mission_ABC();

    //-------------------------------------------------------------------------
    /** @name Init */
    //-------------------------------------------------------------------------
    //@{
    virtual void Initialize();
    virtual void Terminate ();
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors*/
    //-------------------------------------------------------------------------
    //@{
    virtual void         SetAgent      ( MOS_Agent* pAgent );
            MOS_Agent*   GetAgent      () const;
            E_MissionID  GetTypeMission() const;
    virtual std::string  GetName       () const;
    virtual bool         IsAutomateMission() const = 0;

    MOS_MissionAction* GetOrCreateAction( MOS_AgentListView& listView );
    //@}

    //-------------------------------------------------------------------------
    /** @name GFX*/
    //-------------------------------------------------------------------------
    //@{
    void Draw();
    bool IsActive( bool bMove );
    bool IsActiveTab( QWidget* pWidget );
    bool RMBPressEvent( const MT_Vector2D& vPos );
    bool LMBPressEvent( const MT_Vector2D& vPos );
    void MouseMoveEvent( const MT_Vector2D& vPos );
    void RMBReleaseEvent( const MT_Vector2D& vPos );
    void LMBReleaseEvent( const MT_Vector2D& vPos );
    //@}

    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    void Validate();
    void Cancel ();
    //@}

    virtual void FillRandomParameters() = 0;
    virtual void ReloadParameters( MT_InputArchive_ABC& archive ) = 0;
    virtual void ResaveParameters( MT_OutputArchive_ABC& archive ) = 0;

protected slots:
    //-------------------------------------------------------------------------
    /** @name Slots */
    //-------------------------------------------------------------------------
    //@{
    void SlotValidate();
    void SlotCancel  ();
    //@}

protected:
    //-------------------------------------------------------------------------
    /** @name MissionTools*/
    //-------------------------------------------------------------------------
    //@{
    void RegisterMissionTool( MOS_MissionTool_ABC& missionTool );
    void UnregisterMissionTool( MOS_MissionTool_ABC& missionTool );
    //@}

    //-------------------------------------------------------------------------
    /** @name Tools */
    //-------------------------------------------------------------------------
    //@{
    MOS_LimitMission&                  CreateLimitMission       ( ASN1T_OID& asnLeftLimitID, ASN1T_OID& asnRightLimitID, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName );
    MOS_LimaMission&                   CreateLimaMission        ( ASN1T_ListOID& asnLimas, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName );

    MOS_MTH_NatureAtlas&               CreateNatureAtlas        ( ASN1T_NatureAtlas& asnNatureAtlas          , QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName );
    MOS_MTH_GDH&                       CreateGDH                ( ASN1T_GDH& asnGDH                          , QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName, int nTools );
    MOS_MTH_DirectionPos&              CreateDirection       ( ASN1T_Direction& asnDirection              , QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName, bool bFollow = true );
    MOS_MTH_SegmentPos&                CreateSegmentPos         ( ASN1T_Segment& asnSegment                  , QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName );
    MOS_MTH_Itineraire1Pos&            CreatePoint     ( ASN1T_Point& asnPoint                      , QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName );
    MOS_MTH_ItinerairePos&             CreatePath         ( ASN1T_Itineraire& asnListPoint             , QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName );
    MOS_MTH_ListPoint&                 CreatePointList          ( ASN1T_ListPoint& asnListPoint              , QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName );
    MOS_AgentMission&                  CreateAgentList          ( ASN1T_ListAgent& asnListAgent              , QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName );
    MOS_AgentMission&                  CreateAutomateList          ( ASN1T_ListAgent& asnListAgent              , QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName );
    MOS_AgentMission&                  CreateAgent              ( ASN1T_Agent& asnAgent                      , QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName );
    MOS_ValueList&                     CreateValueList          (                                              QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName, int nWidth, int nHeight );
    MOS_MTH_Localisation&              CreateLocation       ( ASN1T_Localisation& asnLocalisation        , QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName );
    MOS_MTH_AgentKnowledgeList&        CreateAgentKnowledgeList ( ASN1T_ListKnowledgeAgent& asnListAgent     , QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName );
    MOS_MTH_AgentKnowledgeList&        CreateAgentKnowledge     ( ASN1T_KnowledgeAgent& asnListAgent         , QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName );
    MOS_MTH_ObjectKnowledgeList&       CreateObjectKnowledgeList( ASN1T_ListKnowledgeObject& asnListObject    , QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName );
    MOS_MTH_ObjectKnowledgeList&       CreateObjectKnowledge    ( ASN1T_KnowledgeObject& asnListObject        , QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName );
    MOS_MTH_ListLocalisation_ListView& CreateLocationList   ( ASN1T_ListLocalisation& asnListLocalisation, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName );
    MOS_MTH_ListLocalisation_ListView& CreatePolygonList   ( ASN1T_ListPolygon& asnListLocalisation, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName );
    MOS_MTH_PointList_ListView&        CreatePathList     ( ASN1T_ListItineraire& asnListLocalisation , QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName );
    MOS_MTH_Obstacle_ListView&         CreateGenObjectList       ( ASN1T_ListMissionGenObject& asnListObstacle, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName );
    MOS_MTH_Obstacle&                  CreateGenObject          ( ASN1T_MissionGenObject& asn, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName );
    MOS_MTH_MaintenancePriorities&     CreateMaintenancePriorities( ASN1T_MaintenancePriorites& asn, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName );
    MOS_MTH_MedicalPriorities&         CreateMedicalPriorities( ASN1T_SantePriorites& asn, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName );
    void                               CreateID                 ( ASN1T_OID& /*asn*/, const std::string ) {} 

    template < class T >
    MOS_ValueButtonList< T >& CreateVarList( T& value, QWidget* pParentWidget, QBoxLayout* pParentLayout, const std::string& sName, int nWidth, int nHeight, bool bExclusive = true )
    {
        MOS_ValueButtonList<T>* pMissionTool = new MOS_ValueButtonList< T >( value, pParentWidget, sName, nWidth, nHeight, bExclusive );
        pMissionTool->SetWidgetTab( pParentWidget );
        pMissionTool->SetMission( this );
        if( pParentLayout )
            pParentLayout->addWidget( pMissionTool );
        RegisterMissionTool( *pMissionTool );
        return *pMissionTool;
    }

    // Helpers
    MOS_MTH_NatureAtlas&               CreateNatureAtlas        ( ASN1T_NatureAtlas& asn         , const std::string& sName )                          { return CreateNatureAtlas          ( asn, pOptionWidget_, pOptionLayout_, sName );                    }
    MOS_MTH_GDH&                       CreateGDH                ( ASN1T_GDH& asn                 , const std::string& sName, int nTools )               { return CreateGDH                  ( asn, pOptionWidget_, pOptionLayout_, sName, nTools );            }
    MOS_MTH_DirectionPos&              CreateDirection       ( ASN1T_Direction& asn           , const std::string& sName, bool bFollow = true )      { return CreateDirection         ( asn, pOptionWidget_, pOptionLayout_, sName, bFollow );           }
    MOS_MTH_SegmentPos&                CreateSegmentPos         ( ASN1T_Segment& asn             , const std::string& sName )                           { return CreateSegmentPos           ( asn, pOptionWidget_, pOptionLayout_, sName );                    }
    MOS_MTH_Itineraire1Pos&            CreatePoint     ( ASN1T_Point& asn               , const std::string& sName )                           { return CreatePoint       ( asn, pOptionWidget_, pOptionLayout_, sName );                    }
    MOS_MTH_ItinerairePos&             CreatePath         ( ASN1T_Itineraire& asn          , const std::string& sName )                           { return CreatePath           ( asn, pOptionWidget_, pOptionLayout_, sName );                    }
    MOS_MTH_ListPoint&                 CreatePointList          ( ASN1T_ListPoint& asn           , const std::string& sName )                           { return CreatePointList            ( asn, pOptionWidget_, pOptionLayout_, sName );                    }
    MOS_AgentMission&                  CreateAgentList          ( ASN1T_ListAgent& asn           , const std::string& sName )          { return CreateAgentList            ( asn, pOptionWidget_, pOptionLayout_, sName );       }
    MOS_AgentMission&                  CreateAutomateList          ( ASN1T_ListAgent& asn           , const std::string& sName )          { return CreateAutomateList            ( asn, pOptionWidget_, pOptionLayout_, sName );       }
    MOS_AgentMission&                  CreateAutomateList          ( ASN1T_ListAutomate& asn        , const std::string& sName )          { return CreateAutomateList            ( (ASN1T_ListAgent&)asn, pOptionWidget_, pOptionLayout_, sName );       }
    MOS_AgentMission&                  CreateAgent              ( ASN1T_Agent& asn               , const std::string& sName )                           { return CreateAgent                ( asn, pOptionWidget_, pOptionLayout_, sName );                    }
    MOS_ValueList&                     CreateValueList          (                                  const std::string& sName, int nWidth, int nHeight )  { return CreateValueList            (      pOptionWidget_, pOptionLayout_, sName, nWidth, nHeight );   }
    MOS_MTH_Localisation&              CreateLocation       ( ASN1T_Localisation& asn        , const std::string& sName )                           { return CreateLocation         ( asn, pOptionWidget_, pOptionLayout_, sName );                    }
    MOS_MTH_AgentKnowledgeList&        CreateAgentKnowledge     ( ASN1T_KnowledgeAgent& asn      , const std::string& sName )                           { return CreateAgentKnowledge       ( asn, pOptionWidget_, pOptionLayout_, sName );                    }
    MOS_MTH_AgentKnowledgeList&        CreateAgentKnowledgeList ( ASN1T_ListKnowledgeAgent& asn  , const std::string& sName )                           { return CreateAgentKnowledgeList   ( asn, pOptionWidget_, pOptionLayout_, sName );                    }
    MOS_MTH_ObjectKnowledgeList&       CreateObjectKnowledgeList( ASN1T_ListKnowledgeObject& asn  , const std::string& sName )                           { return CreateObjectKnowledgeList  ( asn, pOptionWidget_, pOptionLayout_, sName );                    }
    MOS_MTH_ObjectKnowledgeList&       CreateObjectKnowledge    ( ASN1T_KnowledgeObject& asn      , const std::string& sName )                           { return CreateObjectKnowledge      ( asn, pOptionWidget_, pOptionLayout_, sName );                    }
    MOS_MTH_ListLocalisation_ListView& CreateLocationList   ( ASN1T_ListLocalisation& asn    , const std::string& sName )                           { return CreateLocationList     ( asn, pOptionWidget_, pOptionLayout_, sName );                    }
    MOS_MTH_ListLocalisation_ListView& CreatePolygonList   ( ASN1T_ListPolygon& asn    , const std::string& sName )                           { return CreatePolygonList     ( asn, pOptionWidget_, pOptionLayout_, sName );                    }
    MOS_MTH_PointList_ListView&        CreatePathList     ( ASN1T_ListItineraire& asn     , const std::string& sName )                           { return CreatePathList       ( asn, pOptionWidget_, pOptionLayout_, sName );                    }
    MOS_MTH_Obstacle_ListView&         CreateGenObjectList       ( ASN1T_ListMissionGenObject& asn, const std::string& sName )                       { return CreateGenObjectList         ( asn, pOptionWidget_, pOptionLayout_, sName );                    }
    MOS_MTH_Obstacle&                  CreateGenObject ( ASN1T_MissionGenObject& asn, const std::string& sName ) { return CreateGenObject         ( asn, pOptionWidget_, pOptionLayout_, sName );                    }
    MOS_MTH_MaintenancePriorities&     CreateMaintenancePriorities( ASN1T_MaintenancePriorites& asn, const std::string& sName ) { return CreateMaintenancePriorities( asn, pOptionWidget_, pOptionLayout_, sName );                    }
    MOS_MTH_MedicalPriorities&         CreateMedicalPriorities( ASN1T_SantePriorites& asn, const std::string& sName ) { return CreateMedicalPriorities( asn, pOptionWidget_, pOptionLayout_, sName );                    }

    template < class T >
    MOS_ValueButtonList< T >& CreateVarList( T& value, const std::string& sName, int nWidth, int nHeight, bool bExclusive = true )
    {
        return CreateVarList( value, pOptionWidget_, pOptionLayout_, sName, nWidth, nHeight, bExclusive );
    }
    //@}

    //-------------------------------------------------------------------------
    /** @name Network */
    //-------------------------------------------------------------------------
    //@{
    virtual void SendMsg( std::string& sParam ) = 0;
    //@}

protected:
    QVBoxLayout*            pMainLayout_;
    QTabWidget*             pTabWidget_;
    QTabWidget*             pOptionWidget_;
    QVBoxLayout*            pOptionLayout_;
    E_MissionID             nTypeMission_;

    T_MissionToolPtrVector  missionToolVector_;

private:
    QHBoxLayout*            pLabelLayout_;
    QLabel*                 pLabel_;

    QButtonGroup*           pButtonGroup_;
    QPushButton*            pOKButton_;
    QPushButton*            pCancelButton_;

    MOS_Agent*              pAgent_;
    
    int                     nNbLinkGLWidget_;

    MOS_MissionAction*      pAction_;
};

#ifdef MOS_USE_INLINE
#   include "MOS_Mission_ABC.inl"
#endif

#endif // __MOS_Mission_ABC_h_
