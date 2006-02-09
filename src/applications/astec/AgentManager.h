//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentManager.h $
// $Author: Age $
// $Modtime: 14/04/05 12:42 $
// $Revision: 11 $
// $Workfile: AgentManager.h $
//
//*****************************************************************************

#ifndef __AgentManager_h_
#define __AgentManager_h_

#ifdef __GNUG__
#    pragma interface
#endif

#include "ASN_Types.h"

class Agent;
class AgentModel;
class Gtia;
class Team;
class TypePion;
class TypeAutomate;
class TypeComposante;
class TypePopulation;
class LogMaintenanceConsign;
class LogMedicalConsign;
class LogSupplyConsign;
class Population;
class Agent_ABC;

// =============================================================================
/** @class  AgentManager
    @brief  Manages the agents, agent models, teams, gtias and sensors.
*/
// Created: APE 2004-03-10
// =============================================================================
class AgentManager
{
    MT_COPYNOTALLOWED( AgentManager );

    friend class GLTool;

public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    typedef std::map< MIL_AgentID, Agent* > T_AgentMap;
    typedef const T_AgentMap                    CT_AgentMap;
    typedef T_AgentMap::iterator                IT_AgentMap;
    typedef T_AgentMap::const_iterator          CIT_AgentMap;
    typedef T_AgentMap::reverse_iterator        RIT_AgentMap;
    typedef T_AgentMap::const_reverse_iterator  RCIT_AgentMap;

    typedef std::map< MIL_AgentID, Agent_ABC* >     T_AllAgentMap;
    typedef const T_AllAgentMap                         CT_AllAgentMap;
    typedef T_AllAgentMap::iterator                     IT_AllAgentMap;
    typedef T_AllAgentMap::const_iterator               CIT_AllAgentMap;
    typedef T_AllAgentMap::reverse_iterator             RIT_AllAgentMap;
    typedef T_AllAgentMap::const_reverse_iterator       RCIT_AllAgentMap;

	typedef std::map< MIL_AgentID, Population* > T_PopulationMap;
	typedef const T_PopulationMap					 CT_PopulationMap;
	typedef T_PopulationMap::iterator				 IT_PopulationMap;
	typedef T_PopulationMap::const_iterator			 CIT_PopulationMap;
    typedef T_PopulationMap::reverse_iterator        RIT_PopulationMap;
    typedef T_PopulationMap::const_reverse_iterator  RCIT_PopulationMap;

    typedef std::vector< AgentModel* > T_ModelVector;
    typedef T_ModelVector::iterator       IT_ModelVector;
    typedef T_ModelVector::const_iterator CIT_ModelVector;

    typedef std::map< uint, Team* > T_TeamMap;
    typedef T_TeamMap::iterator         IT_TeamMap;
    typedef T_TeamMap::const_iterator   CIT_TeamMap;

    typedef struct
    {
        Agent_ABC* pOrigin_;
        Agent_ABC* pDirectFireTarget_;
        MT_Vector2D    vIndirectFireTarget_;
    } T_Conflict;

    typedef std::map< uint, T_Conflict >    T_ConflictMap;
    typedef T_ConflictMap::iterator         IT_ConflictMap;
    typedef T_ConflictMap::const_iterator   CIT_ConflictMap;

    typedef std::map< uint, TypePion* > T_TypePionMap;
    typedef T_TypePionMap::const_iterator   CIT_TypePionMap;

    typedef std::map< uint, TypeAutomate* > T_TypeAutomateMap;
    typedef T_TypeAutomateMap::const_iterator   CIT_TypeAutomateMap;

    typedef std::map< uint, TypeComposante* > T_TypeComposanteMap;
    typedef T_TypeComposanteMap::const_iterator   CIT_TypeComposanteMap;

	typedef std::map< uint, TypePopulation* >  T_TypePopulationMap;
	typedef T_TypePopulationMap::const_iterator	   CIT_TypePopulationMap;

    typedef std::map< uint, LogMaintenanceConsign* > T_MaintenanceConsigns;
    typedef T_MaintenanceConsigns::iterator             IT_MaintenanceConsigns;
    typedef T_MaintenanceConsigns::const_iterator       CIT_MaintenanceConsigns;

    typedef std::map< uint, LogSupplyConsign* > T_SupplyConsigns;
    typedef T_SupplyConsigns::iterator             IT_SupplyConsigns;
    typedef T_SupplyConsigns::const_iterator       CIT_SupplyConsigns;

    typedef std::map< uint, LogMedicalConsign* > T_MedicalConsigns;
    typedef T_MedicalConsigns::iterator             IT_MedicalConsigns;
    typedef T_MedicalConsigns::const_iterator       CIT_MedicalConsigns;
    //@}

public:
     AgentManager();
    ~AgentManager();
    
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
    void               CreateTeam     ( uint32 nId, DIN::DIN_Input& input );
    void               RegisterTeam   ( Team& team );
    void               DeleteAllTeams ();
    Team*          FindTeam       ( uint nID ) const;
    Team*          FindTeam       ( const std::string& strName ) const;
    Team*          FindTeamFromIdx( uint nIdx ) const;
    const T_TeamMap&   GetTeams       () const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Agent Types */
    //-------------------------------------------------------------------------
    //@{
    const T_TypePopulationMap* GetTypePopulations() const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Gtias */
    //-------------------------------------------------------------------------
    //@{
    Gtia*   FindGtia( uint nID );
    void        DeleteAllGtias();
    //@}

    //-------------------------------------------------------------------------
    /** @name Agents */
    //-------------------------------------------------------------------------
    //@{
    void				CreateAgent			( const ASN1T_MsgAutomateCreation& asnMsg );
    void				CreateAgent			( const ASN1T_MsgPionCreation& asnMsg );
	void				CreatePopulation    ( const ASN1T_MsgPopulationCreation& asnMsg );
    void				AddAgent			( Agent& agent );
    void				RemoveAgent			( Agent& agent );
	void				AddPopulation		( Population& popu );
	void				RemovePopulation	( Population& popu );
    void				DeleteAllAgents		();
    Agent*			FindAgent			( MIL_AgentID nAgentID );
    Agent_ABC*      FindAllAgent        ( MIL_AgentID nAgentID );
    CT_AgentMap&		GetAgentList		();
    CT_AllAgentMap&     GetAllAgentList     ();
	CT_PopulationMap&	GetPopulationList	();
    //@}

    //! @name Conflicts
    //@{
    void           AddDirectConflict  ( ASN1T_OID nConflictID, Agent_ABC& origin, Agent_ABC&     target );
    void           AddIndirectConflict( ASN1T_OID nConflictID, Agent_ABC& origin, const MT_Vector2D& vTarget );
    void           DeleteConflict     ( ASN1T_OID nConflictID );
    Agent_ABC* FindConflictOrigin ( ASN1T_OID nConflictID );
    void           DeleteAllConflicts ();
    //@}

    //! @name Logistic
    //@{
    void RegisterConsign( LogMaintenanceConsign& consign );
    void RegisterConsign( LogMedicalConsign& consign );
    void RegisterConsign( LogSupplyConsign& consign );

    LogMaintenanceConsign* FindMaintenanceConsign( ASN1T_OID nConsignId );
    LogMedicalConsign*     FindMedicalConsign( ASN1T_OID nConsignId );
    LogSupplyConsign*      FindSupplyConsign( ASN1T_OID nConsignId );

    const T_MaintenanceConsigns&        GetMaintenanceConsigns() const;
    const T_MedicalConsigns&            GetMedicalConsigns() const;
    const T_SupplyConsigns&             GetSupplyConsigns() const;

    void DeleteMaintenanceConsign( ASN1T_OID nConsignId );
    void DeleteMedicalConsign( ASN1T_OID nConsignId );
    void DeleteSupplyConsign( ASN1T_OID nConsignId );
    //@}


    void ReadODB( InputArchive& archive );
    void WriteODB( MT_XXmlOutputArchive& archive );

          AgentModel*     FindModel         ( const std::string& strModelName ) const;
    const TypePion*       FindTypePion      ( const std::string& strName ) const;
    const TypeAutomate*   FindTypeAutomate  ( const std::string& strName ) const;
    const TypeComposante* FindTypeComposante( const std::string& strName ) const;
	const TypePopulation* FindTypePopulation( const std::string& strName ) const;
    const TypePion*       FindTypePion      ( uint nID ) const;
    const TypeAutomate*   FindTypeAutomate  ( uint nID ) const;
    const TypeComposante* FindTypeComposante( uint nID ) const;
	const TypePopulation* FindTypePopulation( uint nID ) const;
	Population*	  FindPopulation	( MIL_AgentID );

private:
    void InitializeModels         ( InputArchive& input );
    void InitializeTypesComposante( InputArchive& input );
    void InitializeTypesPion      ( InputArchive& input );
    void InitializeTypesAutomate  ( InputArchive& input );
    void ReadModelList( InputArchive& modelArchive, bool bAutomata, bool popu = false );
	void InitializeTypesPopulation ( InputArchive& input );

    template< typename T >
    void AddConsign( std::map< ASN1T_OID, T* >& cont, T& consign );
    template< typename T >
    void DeleteConsign( std::map< ASN1T_OID, T* >& cont, ASN1T_OID nConsign );
    template< typename T >
    T* FindConsign( std::map< ASN1T_OID, T* >& cont, ASN1T_OID nConsign );

private:
    T_TeamMap       teamMap_;
    T_AgentMap      agentMap_;
    T_ConflictMap   conflictMap_;
	T_PopulationMap	populationMap_;
    T_AllAgentMap   allMap_;

    T_ModelVector vAvailableModels_;

    T_TypePionMap         typesPion_;
    T_TypeAutomateMap     typesAutomate_;
    T_TypeComposanteMap   typesComposante_;
	T_TypePopulationMap	  typesPopulation_;

    T_MaintenanceConsigns maintenanceConsigns_;
    T_SupplyConsigns      supplyConsigns_;
    T_MedicalConsigns     medicalConsigns_;
};

#   include "AgentManager.inl"

#endif // __AgentManager_h_
