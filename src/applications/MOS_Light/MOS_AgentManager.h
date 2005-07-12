//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentManager.h $
// $Author: Nld $
// $Modtime: 14/02/05 16:00 $
// $Revision: 25 $
// $Workfile: MOS_AgentManager.h $
//
//*****************************************************************************

#ifndef __MOS_AgentManager_h_
#define __MOS_AgentManager_h_

#include "MOS_Types.h"
#include "MOS_ASN_Types.h"

class MOS_Agent;
class MOS_AgentListView;
class MOS_ObjectKnowledge;
class MOS_AgentModel;
class MOS_Gtia;
class MOS_Team;
class MOS_LogMaintenanceConsign;
class MOS_LogMedicalConsign;
class MOS_LogSupplyConsign;
class MOS_TypePion;
class MOS_TypeAutomate;

//=============================================================================
// Created: NLD 2002-07-15
//=============================================================================
class MOS_AgentManager
{
    MT_COPYNOTALLOWED( MOS_AgentManager );

public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    typedef std::map< MIL_AgentID, MOS_Agent* > T_AgentMap;
    typedef const T_AgentMap                    CT_AgentMap;
    typedef T_AgentMap::iterator                IT_AgentMap;
    typedef T_AgentMap::const_iterator          CIT_AgentMap;
    
    typedef std::vector< MOS_Agent* >                 T_AgentPtrVector;
    typedef const T_AgentPtrVector                    CT_AgentPtrVector;
    typedef T_AgentPtrVector::iterator                IT_AgentPtrVector;
    typedef T_AgentPtrVector::const_iterator          CIT_AgentPtrVector;

    typedef std::set< MOS_AgentListView* >     T_AgentListViewSet;
    typedef const T_AgentListViewSet           CT_AgentListViewSet;
    typedef T_AgentListViewSet::iterator       IT_AgentListViewSet;
    typedef T_AgentListViewSet::const_iterator CIT_AgentListViewSet; 
    
    typedef std::map< uint, MOS_ObjectKnowledge* > T_ObjectKnowledgeMap;
    typedef T_ObjectKnowledgeMap::iterator         IT_ObjectKnowledgeMap;
    typedef T_ObjectKnowledgeMap::const_iterator   CIT_ObjectKnowledgeMap;

    typedef std::vector< MOS_AgentModel* > T_Model_Vector;
    typedef T_Model_Vector::iterator       IT_Model_Vector;
    typedef T_Model_Vector::const_iterator CIT_Model_Vector;

    typedef std::map< uint, MOS_Gtia* > T_GtiaMap;
    typedef T_GtiaMap::iterator         IT_GtiaMap;
    typedef T_GtiaMap::const_iterator   CIT_GtiaMap;

    typedef std::map< uint, MOS_Team* > T_TeamMap;
    typedef T_TeamMap::iterator         IT_TeamMap;
    typedef T_TeamMap::const_iterator   CIT_TeamMap;

    typedef std::map< uint, MOS_LogMaintenanceConsign* > T_MaintenanceConsignMap;
    typedef T_MaintenanceConsignMap::iterator            IT_MaintenanceConsignMap;
    typedef T_MaintenanceConsignMap::const_iterator      CIT_MaintenanceConsignMap;

    typedef std::map< uint, MOS_LogSupplyConsign* > T_SupplyConsignMap;
    typedef T_SupplyConsignMap::iterator            IT_SupplyConsignMap;
    typedef T_SupplyConsignMap::const_iterator      CIT_SupplyConsignMap;

    typedef std::map< uint, MOS_LogMedicalConsign* > T_MedicalConsignMap;
    typedef T_MedicalConsignMap::iterator            IT_MedicalConsignMap;
    typedef T_MedicalConsignMap::const_iterator      CIT_MedicalConsignMap;

    typedef std::map< uint, MOS_TypePion* > T_TypePionMap;
    typedef T_TypePionMap::const_iterator   CIT_TypePionMap;

    typedef std::map< uint, MOS_TypeAutomate* > T_TypeAutomateMap;
    typedef T_TypeAutomateMap::const_iterator   CIT_TypeAutomateMap;

    typedef std::vector< MOS_Team* >        T_TeamIDVector;
    typedef T_TeamIDVector::const_iterator  CIT_TeamIDVector;
    //@}

public:
     MOS_AgentManager();
    ~MOS_AgentManager();
    
    //-------------------------------------------------------------------------
    /** @name Init */
    //-------------------------------------------------------------------------
    //@{
    void Initialize();
    void Terminate ();
    //@}

    //-------------------------------------------------------------------------
    /** @name Draw */
    //-------------------------------------------------------------------------
    //@{
    void Draw();
    void DrawVision();
    void DrawVisionSurface();
    void DrawVisionCone();

    //@}

    //-------------------------------------------------------------------------
    /** @name Agents */
    //-------------------------------------------------------------------------
    //@{
    void                  RegisterTeam  ( MOS_Team& team );
    void                  RemoveAllTeams();
    MOS_Team*             FindTeam      ( uint nID ) const;
    const T_TeamIDVector& GetTeams      () const;


    void         RegisterGtia  ( MOS_Gtia& gtia );
    void         RemoveAllGtias();
    MOS_Gtia*    FindGtia      ( uint nID );


    void         RegisterAgent  ( MOS_Agent& agent );
    void         UnregisterAgent( MOS_Agent& agent );
    void         RemoveAllAgents();
    MOS_Agent*   FindAgent      ( MIL_AgentID nAgentID );
    CT_AgentMap& GetAgentList   ();

    void         RegisterAgentListView       ( MOS_AgentListView& agentListView );
    void         UnregisterAgentListView     ( MOS_AgentListView& agentListView );
    void         RegisterListViewsForAgent   ( MOS_Agent& agent );

    void                       RegisterMaintenanceConsign  ( MOS_LogMaintenanceConsign& consign );
    void                       UnregisterMaintenanceConsign( MOS_LogMaintenanceConsign& consign );
    MOS_LogMaintenanceConsign* FindMaintenanceConsign      ( uint nID ) const;

    void                       RegisterSupplyConsign  ( MOS_LogSupplyConsign& consign );
    void                       UnregisterSupplyConsign( MOS_LogSupplyConsign& consign );
    MOS_LogSupplyConsign* FindSupplyConsign      ( uint nID ) const;

    void                       RegisterMedicalConsign  ( MOS_LogMedicalConsign& consign );
    void                       UnregisterMedicalConsign( MOS_LogMedicalConsign& consign );
    MOS_LogMedicalConsign* FindMedicalConsign      ( uint nID ) const;
    

    const MOS_AgentModel*   FindModel       ( const std::string& strModelName ) const;
    const MOS_TypePion*     FindTypePion    ( const std::string& strName ) const;
    const MOS_TypePion*     FindTypePion    ( uint nID ) const;
    const MOS_TypeAutomate* FindTypeAutomate( uint nID ) const;
    //@}

private:
    void InitializeModels       ( MT_InputArchive_ABC& input );
    void InitializeTypesPion    ( MT_InputArchive_ABC& input );
    void InitializeTypesAutomate( MT_InputArchive_ABC& input );

private:
    T_TeamMap    teamMap_;
    T_GtiaMap    gtiaMap_;
    T_AgentMap   agentMap_;
    T_TeamIDVector teams_;

    T_MaintenanceConsignMap maintenanceConsigns_;
    T_MedicalConsignMap     medicalConsigns_;
    T_SupplyConsignMap      supplyConsigns_;

    T_AgentListViewSet   agentListViewSet_;

    T_Model_Vector vAvailableModels_;

    T_TypePionMap     typesPion_;
    T_TypeAutomateMap typesAutomate_;
};

#ifdef MOS_USE_INLINE
#   include "MOS_AgentManager.inl"
#endif

#endif // __MOS_AgentManager_h_
