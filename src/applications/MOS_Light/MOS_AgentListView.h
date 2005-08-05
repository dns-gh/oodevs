//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentListView.h $
// $Author: Nld $
// $Modtime: 15/03/05 19:01 $
// $Revision: 51 $
// $Workfile: MOS_AgentListView.h $
//
//*****************************************************************************

#ifndef __MOS_AgentListView_h_
#define __MOS_AgentListView_h_


#include "MOS_Types.h"
#include "MOS_Order_Def.h"

#include <QListView.h>

#include "MOS_Agent.h"
#include "MOS_GLMouse_ABC.h"
#include "MOS_ListViewItem_ABC.h"
#include "MOS_ArmeListViewItem.h"

#include "MOS_AgentManager.h"

#include "MT/MT_Tools/MT_IdentifierManager.h"
#include "MT/MT_Tools/MT_Triple.h"

class QWidget;
class QPopupMenu;
class QListViewItem;
class MOS_Agent;
class MOS_AgentListViewItem_ABC;
class QActionGroup;
class MOS_MissionAction;
class QAction;
class MOS_OrderDialog_ABC;
class MOS_Mission_ABC;
class MOS_AgentTest;

//*****************************************************************************
// Created: FBD 02-12-17
// Last modified: JVT 03-04-08
//*****************************************************************************
class MOS_AgentListView : public QListView
                        , public MOS_GLMouse_ABC
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_AgentListView );

private: 
    typedef std::map< E_TypeArme, MOS_ArmeListViewItem* >   T_ArmePtrMap;    
    typedef const T_ArmePtrMap                              CT_ArmePtrMap;
    typedef T_ArmePtrMap::iterator                          IT_ArmePtrMap;
    typedef T_ArmePtrMap::const_iterator                    CIT_ArmePtrMap;
    typedef T_ArmePtrMap::reverse_iterator                  RIT_ArmePtrMap;

    typedef MT_Triple< QPopupMenu*, QPopupMenu*, QPopupMenu*>   T_MissionPopupTriple;
    typedef std::map< E_TypeArme, T_MissionPopupTriple >        T_MissionPopupPtrMap;
    typedef const T_MissionPopupPtrMap                          CT_MissionPopupPtrMap;
    typedef T_MissionPopupPtrMap::iterator                      IT_MissionPopupPtrMap;
    typedef T_MissionPopupPtrMap::const_iterator                CIT_MissionPopupPtrMap;
    typedef T_MissionPopupPtrMap::reverse_iterator              RIT_MissionPopupPtrMap;

    typedef std::vector< MOS_Mission_ABC* >           T_MissionDialogPtr_Vector;
    typedef T_MissionDialogPtr_Vector::iterator       IT_MissionDialogPtr_Vector;
    typedef T_MissionDialogPtr_Vector::const_iterator CIT_MissionDialogPtr_Vector;

    typedef std::map< E_MissionID, MOS_Mission_ABC* > T_MissionDialogPtrMap;
    typedef const T_MissionDialogPtrMap               CT_MissionDialogPtrMap;
    typedef T_MissionDialogPtrMap::iterator           IT_MissionDialogPtrMap;
    typedef T_MissionDialogPtrMap::const_iterator     CIT_MissionDialogPtrMap;
    typedef T_MissionDialogPtrMap::reverse_iterator   RIT_MissionDialogPtrMap;
    
    typedef std::map< E_FragOrder, MOS_Mission_ABC* >  T_OCDialogAgentTypePtrMap;
    typedef const T_OCDialogAgentTypePtrMap             CT_OCDialogAgentTypePtrMap;
    typedef T_OCDialogAgentTypePtrMap::iterator         IT_OCDialogAgentTypePtrMap;
    typedef T_OCDialogAgentTypePtrMap::const_iterator   CIT_OCDialogAgentTypePtrMap;
    typedef T_OCDialogAgentTypePtrMap::reverse_iterator RIT_OCDialogAgentTypePtrMap;

    typedef std::vector< QListViewItem* >           T_AgentPtrVector;
    typedef const T_AgentPtrVector                   CT_AgentPtrVector;
    typedef T_AgentPtrVector::iterator               IT_AgentPtrVector;
    typedef T_AgentPtrVector::const_iterator         CIT_AgentPtrVector;
    typedef T_AgentPtrVector::reverse_iterator       RIT_AgentPtrVector;

    typedef std::pair< MOS_PionListViewItem*, MOS_AutomateListViewItem* >   T_AgentChangeAutomatePair;
    typedef std::map< uint, T_AgentChangeAutomatePair >                     T_AgentChangeAutomateMap;
    typedef T_AgentChangeAutomateMap::iterator                              IT_AgentChangeAutomateMap;

    typedef std::pair< MOS_AutomateListViewItem*, MOS_KnowledgeGroupListViewItem* > T_AutomateChangeKnowledgeGroupPair;
    typedef std::map< uint, T_AutomateChangeKnowledgeGroupPair >                    T_AutomateChangeKnowledgeGroupMap;
    typedef T_AutomateChangeKnowledgeGroupMap::iterator                             IT_AutomateChangeKnowledgeGroupMap;

public:
    //-------------------------------------------------------------------------
    /** @name Constructor/Destructor*/
    //-------------------------------------------------------------------------
    //@{
    explicit MOS_AgentListView( QWidget* pParent, E_ListViewType nType );
    virtual ~MOS_AgentListView();
    //@}

    //-------------------------------------------------------------------------
    /** @name Init*/
    //-------------------------------------------------------------------------
    //@{
    void Initialize();
    void Terminate();
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors*/
    //-------------------------------------------------------------------------
    //@{
    MOS_ArmeListViewItem*       CreateArme              ( E_TypeArme nTypeArme );
    void                        DeleteArme              ( E_TypeArme nTypeArme );
    MOS_ArmeListViewItem&       GetArme                 ( E_TypeArme nTypeArme );
    QListViewItem*              CreateAgent             ( MOS_Agent& Agent );
    void                        DeleteAgent             ( QListViewItem& Agent );
    void                        RegisterAgent           ( QListViewItem& Agent );
    void                        UnregisterAgent         ( QListViewItem& Agent );
    MOS_Agent*                  GetSelectedAgent        () const;
    void                        LocOnAgent              ( QListViewItem* pAgent, bool bSelectList = true );
    E_ListViewType              GetType                 () const;
    void                        CreateAgentFromSelector ( MOS_AgentListView* pSelector );
    void                        GetTaggedAgents         ( MOS_AgentManager::T_AgentPtrVector& agentVector );
    void                        SetAgent                ( MOS_Agent* pAgent );
    bool                        IsOptiDraw              ()  const;
    bool                        IsDrawAltitude          ()  const;
    bool                        IsDrawItineraire        ()  const;
    bool                        IsDrawAllItineraire     ()  const;
    bool                        IsDrawAllLima           ()  const;
    bool                        IsDrawAllLimit          ()  const;
    bool                        IsDrawAllVision         ()  const;
    bool                        IsDrawAllCone           ()  const;
    bool                        IsDrawOldPath           ()  const;
    bool                        IsDrawText              ()  const;
    bool                        IsScrollTrace           ()  const;
    bool                        MustSaveCoordinates     ()  const;
    MOS_Agent*                  GetInitAgent            ( const MOS_Agent& agent );
    T_AgentPtrVector&           GetListAgent            ();
    bool                        IsEmpty                 () const;

    void SetDrawVisionCones( bool bEnable );
    
    //@}

    //-------------------------------------------------------------------------
    /** @name GFX*/
    //-------------------------------------------------------------------------
    //@{
    void Draw();
    bool IsActive( bool bMove );
    bool RMBPressEvent( const MT_Vector2D& vPos );
    bool LMBPressEvent( const MT_Vector2D& vPos );
    void MouseMoveEvent( const MT_Vector2D& vPos );
    void RMBReleaseEvent( const MT_Vector2D& vPos );
    void LMBReleaseEvent( const MT_Vector2D& vPos );
    //@}

    //-------------------------------------------------------------------------
    /** @name Mission*/
    //-------------------------------------------------------------------------
    //@{
    void        OpenPopupMission       ( QListViewItem* pItem );
    std::string BuildNameArme          ( E_TypeArme nTypeArme );
    QPopupMenu* GetPopupMenu           ( const MOS_Agent& agent );
    void        RegisterConduiteOrder  ( E_FragOrder nTypeMission );
    MOS_Mission_ABC* GetConduiteOrder  ( E_FragOrder nTypeMission );
    void        NotifyDeleteAgent      ( MOS_Agent& agent );
    void        NotifyAgentHasMoved    ( MOS_Agent& agent, const MT_Vector2D& vPos );
    void        NotifyAgentMissionAck  ( ASN1T_EnumOrderErrorCode nErrorCode );
    void        NotifyDisconnected     ();
    //@}

    //-------------------------------------------------------------------------
    /** @name DIN*/
    //-------------------------------------------------------------------------
    //@{
    void WriteMsg( ASN1T_ListAgent& asnListAgent,std::string& sParam );
    void WriteMsg( ASN1T_Agent& asnAgent, std::string& sParam );
    void FillRandomParameters();
    void ReloadParameters( MT_InputArchive_ABC& archive );
    void ResaveParameters( MT_OutputArchive_ABC& archive );

    
    //@}

    //-------------------------------------------------------------------------
    /** @name Changement de supérieur */
    //-------------------------------------------------------------------------
    //@{
    void AcceptAgentChangeAutomate( uint nID );
    void CancelAgentChangeAutomate( uint nID );

    void AcceptAutomateChangeKnowledgeGroup( uint nID );
    void CancelAutomateChangeKnowledgeGroup( uint nID );
    //@}

    //-------------------------------------------------------------------------
    /** @name Test */
    //-------------------------------------------------------------------------
    //@{
//    void GiveAMission( MOS_AgentTest& agentTest, MT_OutputArchive_ABC& archive );
    //    void GiveThisMission( MOS_AgentTest& agentTest, MT_OutputArchive_ABC& archive, MOS_Mission_ABC& mission );

//    void ReplayAMission( MOS_AgentTest& agentTest, MT_InputArchive_ABC& archive );
    //@}

protected slots:
    //-------------------------------------------------------------------------
    /** @name Slots */
    //-------------------------------------------------------------------------
    //@{
    void SlotContextMenuRequested   ( QListViewItem* pItem, const QPoint& point );
    void SlotItemPressed            ( QListViewItem* pItem );
    void SlotSetFollow              ( bool bFollow  );
    void SlotSetItineraire          ( bool bItineraire );
    void SlotSetAllItineraire       ( bool bAllItineraire );
    void SlotSetOptiDraw            ( bool bOptiDraw );
    void SlotSetDrawAltitude        ( bool bDrawAltitude );
    void SlotSetDrawLimit            ( bool bAllLimit );
    void SlotSetDrawLima             ( bool bAllLima );
    void SlotSetAllVision           ( bool bAllVision );
    void SlotSetAllCone             ( bool bAllCone );
    void SlotSetPointEdition        ( bool bValue );
    void SlotSetDrawOldPath         ( bool bDrawOldPath );
    void SlotSetDrawText            ( bool bDrawText );
    void SlotSetScrollTrace         ( bool bScrollTrace );
    void SlotEnableProfiling        ( bool bEnabled );
//    void SlotTestMission            ();
    //@}

protected:
    //-------------------------------------------------------------------------
    /** @name QT */
    //-------------------------------------------------------------------------
    //@{
    QDragObject* dragObject    ();
    void dragEnterEvent( QDragEnterEvent* pEvent );
    void dropEvent     ( QDropEvent* pEvent );
    //@}

private:
    //! @name Types
    //@{
    enum E_TypeAgent
    {
        eTypeAgentPion           ,
        eTypeAgentAutomateEmbraye,
        eTypeAgentAutomateDebraye,
    };
    //@}

private:
    void RegisterMission( E_MissionID nTypeMission, E_TypeAgent nTypeAgent );
    
private:
    QListViewItem*              pSelectedAgent_;
    bool                        bFollow_;
    bool                        bNotLock_;
    bool                        bItineraire_;
    bool                        bAllItineraire_;
    bool                        bDrawAltitude_;
    bool                        bAllVision_;
    bool                        bAllLima_;
    bool                        bAllLimit_;
    bool                        bAllCone_;
    bool                        bOptiDraw_;
    bool                        bPointEdition_; // To retrieve coordinates in a file
    bool                        bDrawOldPath_;
    bool                        bDrawText_;
    bool                        bScrollTrace_;

    E_ListViewType              nType_;
    T_AgentPtrVector            agentVector_;
    
    T_ArmePtrMap                armePtrMap_;
    T_MissionPopupPtrMap        missionPopupPtrMap_;
    T_MissionDialogPtrMap       missionDialogPtrMap_;
    T_OCDialogAgentTypePtrMap   orderConduiteDialogPtrMap_;



    QActionGroup*               pMissions_;

    
    MOS_Agent*                  pAgent_;
    
    
    MOS_Mission_ABC*            pMissionTest_;
    int                         nStateTest_;

    T_AgentChangeAutomateMap            agentChangeAutomateMap_;
    T_AutomateChangeKnowledgeGroupMap   automateChangeKnowledgeGroupMap_;


    MT_IdentifierManager        agentChangeAutomateIDManager_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_AgentListView.inl"
#endif


#endif // __MOS_AgentListView_h_