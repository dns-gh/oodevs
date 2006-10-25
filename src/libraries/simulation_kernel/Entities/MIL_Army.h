// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/MIL_Army.h $
// $Author: Jvt $
// $Modtime: 6/04/05 17:14 $
// $Revision: 9 $
// $Workfile: MIL_Army.h $
//
// *****************************************************************************

#ifndef __MIL_Army_h_
#define __MIL_Army_h_

#include "MIL.h"

#include "MT_Tools/MT_Converter.h"

class DEC_KnowledgeBlackBoard_Army;
class DEC_Knowledge_Object;
class DEC_Knowledge_Agent;
class DEC_Knowledge_Population;
class MIL_KnowledgeGroup;
class MIL_EntityManager;
class MIL_Formation;
class MIL_Population;
class MIL_RealObject_ABC;
struct ASN1T_MsgChangeDiplomatie;

// =============================================================================
// @class  MIL_Army
// Created: JVT 2004-08-03
// =============================================================================
class MIL_Army
{
    MT_COPYNOTALLOWED( MIL_Army )

public:
    //! @name Types
    //@{
    enum E_Diplomacy
    {
        eUnknown,
        eFriend,
        eEnemy,
        eNeutral
    };

    typedef std::map< uint, MIL_KnowledgeGroup* > T_KnowledgeGroupMap;
    typedef T_KnowledgeGroupMap::const_iterator   CIT_KnowledgeGroupMap;

    typedef std::map< const MIL_Army*, E_Diplomacy > T_DiplomacyMap;
    typedef T_DiplomacyMap::const_iterator           CIT_DiplomacyMap;

    typedef std::set< MIL_Formation* >      T_FormationSet;
    typedef T_FormationSet::const_iterator  CIT_FormationSet;
    
    typedef std::set< MIL_Population* >      T_PopulationSet;
    typedef T_PopulationSet::const_iterator  CIT_PopulationSet;

    typedef std::set< MIL_RealObject_ABC* > T_ObjectSet;
    typedef T_ObjectSet::const_iterator     CIT_ObjectSet;
    //@}

public:
     MIL_Army( uint nID, MIL_InputArchive& archive );
     MIL_Army();
    ~MIL_Army();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    void WriteODB         ( MT_XXmlOutputArchive& archive ) const;
    void WriteDiplomacyODB( MT_XXmlOutputArchive& archive ) const;
    //@}

    //! @name Manager
    //@{
    static void Initialize();
    static void Terminate ();
    //@}

    //! @name Init
    //@{
    void InitializeDiplomacy( MIL_InputArchive& archive );
    //@}

    //! @name Knowledge
    //@{
    void UpdateKnowledges();
    void CleanKnowledges ();
    //@}

    //! @name Hierarchy
    //@{
    void RegisterFormation  ( MIL_Formation& formation );
    void UnregisterFormation( MIL_Formation& formation );

    void RegisterObject  ( MIL_RealObject_ABC& object );
    void UnregisterObject( MIL_RealObject_ABC& object );

    void RegisterPopulation  ( MIL_Population& population );
    void UnregisterPopulation( MIL_Population& population );

    MIL_KnowledgeGroup* FindKnowledgeGroup      ( uint nID ) const;
    void                RegisterKnowledgeGroup  ( MIL_KnowledgeGroup& knowledgeGroup );
    void                UnregisterKnowledgeGroup( MIL_KnowledgeGroup& knowledgeGroup );
    //@}

    //! @name Operations
    //@{
    bool       IsPerceived( const DEC_Knowledge_Object& knowledge )      const;
    E_Tristate IsAFriend  ( const DEC_Knowledge_Agent & knowledge )      const;
    E_Tristate IsAFriend  ( const MIL_Army& army )                       const;
    E_Tristate IsAnEnemy  ( const DEC_Knowledge_Agent& knowledge       ) const;
    E_Tristate IsAnEnemy  ( const DEC_Knowledge_Population & knowledge ) const;
    E_Tristate IsAnEnemy  ( const MIL_Army& army )                       const;
    E_Tristate IsNeutral  ( const MIL_Army& army )                       const;
    bool       operator== ( const MIL_Army& rhs )                        const;
    bool       operator!= ( const MIL_Army& rhs )                        const;
    //@}

    //! @name Accessors
    //@{
          uint                          GetID             () const;
    const std::string&                  GetName           () const;
    const T_KnowledgeGroupMap&          GetKnowledgeGroups() const;
          DEC_KnowledgeBlackBoard_Army& GetKnowledge      () const;
    //@}

    //! @name Network
    //@{
    void SendCreation               () const;
    void SendFullState              () const;
    void SendKnowledge              () const;

    void OnReceiveMsgChangeDiplomacy( ASN1T_MsgChangeDiplomatie& msg, MIL_MOSContextID nCtx );
    //@}

private:
    //! @name Tools
    //@{
    E_Diplomacy GetDiplomacy( const MIL_Army& army ) const;

    void InitializeCommunication( MIL_InputArchive& archive );
    void InitializeTactical     ( MIL_InputArchive& archive );
    void InitializeObjects      ( MIL_InputArchive& archive );
    void InitializePopulations  ( MIL_InputArchive& archive );
    void InitializeLogistic     ( MIL_InputArchive& archive );
    //@}

private:
    const uint          nID_;
          std::string   strName_;
          E_Diplomacy   nType_;
    T_DiplomacyMap      diplomacies_;
    T_KnowledgeGroupMap knowledgeGroups_;
    T_FormationSet      formations_;
    T_PopulationSet     populations_;
    T_ObjectSet         objects_;

    DEC_KnowledgeBlackBoard_Army* pKnowledgeBlackBoard_;

private:
    static MT_Converter< std::string, E_Diplomacy > diplomacyConverter_;
};

#include "MIL_Army.inl"

#endif // __MIL_Army_h_
