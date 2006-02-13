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
    typedef std::vector< AgentModel* > T_ModelVector;
    typedef T_ModelVector::iterator       IT_ModelVector;
    typedef T_ModelVector::const_iterator CIT_ModelVector;

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
    /** @name Agent Types */
    //-------------------------------------------------------------------------
    //@{
    const T_TypePopulationMap* GetTypePopulations() const;
    //@}

    //! @name Conflicts
    //@{
    void       AddDirectConflict  ( ASN1T_OID nConflictID, Agent_ABC& origin, Agent_ABC&     target );
    void       AddIndirectConflict( ASN1T_OID nConflictID, Agent_ABC& origin, const MT_Vector2D& vTarget );
    void       DeleteConflict     ( ASN1T_OID nConflictID );
    Agent_ABC* FindConflictOrigin ( ASN1T_OID nConflictID );
    void       DeleteAllConflicts ();
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

private:
    void InitializeModels         ( InputArchive& input );
    void InitializeTypesComposante( InputArchive& input );
    void InitializeTypesPion      ( InputArchive& input );
    void InitializeTypesAutomate  ( InputArchive& input );
    void ReadModelList( InputArchive& modelArchive, bool bAutomata, bool popu = false );
	void InitializeTypesPopulation ( InputArchive& input );

private:
    T_ConflictMap   conflictMap_;

    T_ModelVector vAvailableModels_;
    T_TypePionMap         typesPion_;
    T_TypeAutomateMap     typesAutomate_;
    T_TypeComposanteMap   typesComposante_;
	T_TypePopulationMap	  typesPopulation_;
};

#endif // __AgentManager_h_
