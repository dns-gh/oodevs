//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentManager.h $
// $Author: Age $
// $Modtime: 14/04/05 12:42 $
// $Revision: 11 $
// $Workfile: MOS_AgentManager.h $
//
//*****************************************************************************

#ifndef __MOS_AgentManager_h_
#define __MOS_AgentManager_h_

#ifdef __GNUG__
#    pragma interface
#endif

#include "MOS_ASN_Types.h"

class MOS_Agent;
class MOS_AgentModel;
class MOS_Gtia;
class MOS_Team;
class MOS_TypePion;
class MOS_TypeAutomate;
class MOS_TypeComposante;
class MOS_LogMaintenanceConsign;
class MOS_LogMedicalConsign;
class MOS_LogSupplyConsign;

// =============================================================================
/** @class  MOS_AgentManager
    @brief  Manages the agents, agent models, teams, gtias and sensors.
*/
// Created: APE 2004-03-10
// =============================================================================
class MOS_AgentManager
{
    MT_COPYNOTALLOWED( MOS_AgentManager );

    friend class MOS_GLTool;

public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    typedef std::map< MIL_AgentID, MOS_Agent* > T_AgentMap;
    typedef const T_AgentMap                    CT_AgentMap;
    typedef T_AgentMap::iterator                IT_AgentMap;
    typedef T_AgentMap::const_iterator          CIT_AgentMap;
    typedef T_AgentMap::reverse_iterator        RIT_AgentMap;
    typedef T_AgentMap::const_reverse_iterator  RCIT_AgentMap;

    typedef std::vector< MOS_AgentModel* > T_ModelVector;
    typedef T_ModelVector::iterator       IT_ModelVector;
    typedef T_ModelVector::const_iterator CIT_ModelVector;

    typedef std::map< uint, MOS_Team* > T_TeamMap;
    typedef T_TeamMap::iterator         IT_TeamMap;
    typedef T_TeamMap::const_iterator   CIT_TeamMap;

    typedef struct
    {
        MOS_Agent* pOrigin_;
        MOS_Agent* pDirectFireTarget_;
        MT_Vector2D vIndirectFireTarget_;
    } T_Conflict;

    typedef std::map< uint, T_Conflict >    T_ConflictMap;
    typedef T_ConflictMap::iterator         IT_ConflictMap;
    typedef T_ConflictMap::const_iterator   CIT_ConflictMap;

    typedef std::map< uint, MOS_TypePion* > T_TypePionMap;
    typedef T_TypePionMap::const_iterator   CIT_TypePionMap;

    typedef std::map< uint, MOS_TypeAutomate* > T_TypeAutomateMap;
    typedef T_TypeAutomateMap::const_iterator   CIT_TypeAutomateMap;

    typedef std::map< uint, MOS_TypeComposante* > T_TypeComposanteMap;
    typedef T_TypeComposanteMap::const_iterator   CIT_TypeComposanteMap;

    typedef std::map< uint, MOS_LogMaintenanceConsign* > T_MaintenanceConsigns;
    typedef T_MaintenanceConsigns::iterator             IT_MaintenanceConsigns;

    typedef std::map< uint, MOS_LogSupplyConsign* > T_SupplyConsigns;
    typedef T_SupplyConsigns::iterator             IT_SupplyConsigns;

    typedef std::map< uint, MOS_LogMedicalConsign* > T_MedicalConsigns;
    typedef T_MedicalConsigns::iterator             IT_MedicalConsigns;
    //@}

public:
     MOS_AgentManager();
    ~MOS_AgentManager();
    
    //-------------------------------------------------------------------------
    /** @name Init */
    //-------------------------------------------------------------------------
    //@{
    void Initialize();
    //@}

    //-------------------------------------------------------------------------
    /** @name Teams */
    //-------------------------------------------------------------------------
    //@{
    void               RegisterTeam   ( MOS_Team& team );
    void               DeleteAllTeams ();
    MOS_Team*          FindTeam       ( uint nID ) const;
    MOS_Team*          FindTeam       ( const std::string& strName ) const;
    MOS_Team*          FindTeamFromIdx( uint nIdx ) const;
    const T_TeamMap&   GetTeams       () const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Gtias */
    //-------------------------------------------------------------------------
    //@{
    MOS_Gtia*   FindGtia( uint nID );
    void        DeleteAllGtias();
    //@}

    //-------------------------------------------------------------------------
    /** @name Agents */
    //-------------------------------------------------------------------------
    //@{
    void         AddAgent       ( MOS_Agent& agent );
    void         RemoveAgent    ( MOS_Agent& agent );
    void         DeleteAllAgents();
    MOS_Agent*   FindAgent      ( MIL_AgentID nAgentID );
    CT_AgentMap& GetAgentList   ();
    //@}

    //-------------------------------------------------------------------------
    /** @name Conflicts */
    //-------------------------------------------------------------------------
    //@{
    void AddDirectConflict( ASN1T_OID nConflictID, MOS_Agent& origin, MOS_Agent& target );
    void AddIndirectConflict( ASN1T_OID nConflictID, MOS_Agent& origin, const MT_Vector2D& vTarget );
    void DeleteConflict( ASN1T_OID nConflictID );
    //@}

    //-------------------------------------------------------------------------
    /** @name Logistic */
    //-------------------------------------------------------------------------
    //@{
    void RegisterConsign( MOS_LogMaintenanceConsign& consign );
    void RegisterConsign( MOS_LogMedicalConsign& consign );
    void RegisterConsign( MOS_LogSupplyConsign& consign );

    MOS_LogMaintenanceConsign* FindMaintenanceConsign( ASN1T_OID nConsignId );
    MOS_LogMedicalConsign*     FindMedicalConsign( ASN1T_OID nConsignId );
    MOS_LogSupplyConsign*      FindSupplyConsign( ASN1T_OID nConsignId );
    MOS_Agent*                 FindConflictOrigin( ASN1T_OID nConflictID );

    void DeleteMaintenanceConsign( ASN1T_OID nConsignId );
    void DeleteMedicalConsign( ASN1T_OID nConsignId );
    void DeleteSupplyConsign( ASN1T_OID nConsignId );
    //@}


    void ReadODB( MOS_InputArchive& archive );
    void WriteODB( MT_XXmlOutputArchive& archive );

          MOS_AgentModel*     FindModel         ( const std::string& strModelName ) const;
    const MOS_TypePion*       FindTypePion      ( const std::string& strName ) const;
    const MOS_TypeAutomate*   FindTypeAutomate  ( const std::string& strName ) const;
    const MOS_TypeComposante* FindTypeComposante( const std::string& strName ) const;
    const MOS_TypePion*       FindTypePion      ( uint nID ) const;
    const MOS_TypeAutomate*   FindTypeAutomate  ( uint nID ) const;
    const MOS_TypeComposante* FindTypeComposante( uint nID ) const;

private:
    void InitializeModels         ( MOS_InputArchive& input );
    void InitializeTypesComposante( MOS_InputArchive& input );
    void InitializeTypesPion      ( MOS_InputArchive& input );
    void InitializeTypesAutomate  ( MOS_InputArchive& input );
    void ReadModelList( MOS_InputArchive& modelArchive, bool bAutomata );

    template< typename T >
    void AddConsign( std::map< ASN1T_OID, T* >& cont, T& consign );
    template< typename T >
    void DeleteConsign( std::map< ASN1T_OID, T* >& cont, ASN1T_OID nConsign );
    template< typename T >
    T* FindConsign( std::map< ASN1T_OID, T* >& cont, ASN1T_OID nConsign );

private:
    T_TeamMap     teamMap_;
    T_AgentMap    agentMap_;
    T_ConflictMap conflictMap_;

    T_ModelVector vAvailableModels_;

    T_TypePionMap         typesPion_;
    T_TypeAutomateMap     typesAutomate_;
    T_TypeComposanteMap   typesComposante_;

    T_MaintenanceConsigns maintenanceConsigns_;
    T_SupplyConsigns      supplyConsigns_;
    T_MedicalConsigns     medicalConsigns_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_AgentManager.inl"
#endif

#endif // __MOS_AgentManager_h_
