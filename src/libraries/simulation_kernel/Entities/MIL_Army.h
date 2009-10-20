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

#include "Entities/MIL_Army_ABC.h"
#include "MT_Tools/MT_Converter.h"

#include "tools/Resolver.h"

namespace xml
{
    class xostream;
    class xistream;
}

class DEC_KnowledgeBlackBoard_Army;
class DEC_Knowledge_Object;
class DEC_Knowledge_Agent;
class DEC_Knowledge_Population;
class MIL_KnowledgeGroup;
class MIL_EntityManager;
class MIL_Formation;
class MIL_Population;
class MIL_Object_ABC;
struct ASN1T_MsgChangeDiplomacy;

// =============================================================================
// @class  MIL_Army
// Created: JVT 2004-08-03
// =============================================================================
class MIL_Army : public MIL_Army_ABC
               , public tools::Resolver< MIL_Formation >
               , public tools::Resolver< MIL_Population >
               , private boost::noncopyable
{

public:
    //! @name Types
    //@{
    typedef std::map< uint, MIL_KnowledgeGroup* > T_KnowledgeGroupMap;
    typedef T_KnowledgeGroupMap::const_iterator   CIT_KnowledgeGroupMap;

    typedef std::map< const MIL_Army_ABC*, E_Diplomacy > T_DiplomacyMap;
    typedef T_DiplomacyMap::const_iterator           CIT_DiplomacyMap;

    typedef std::set< MIL_Object_ABC* > T_ObjectSet;
    typedef T_ObjectSet::const_iterator     CIT_ObjectSet;
    //@}

public:
             MIL_Army( MIL_EntityManager& manager, uint nID, xml::xistream& xis );
             MIL_Army();
    virtual ~MIL_Army();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    void WriteODB         ( xml::xostream& xos ) const;
    void WriteDiplomacyODB( xml::xostream& xos ) const;
    //@}

    //! @name Manager
    //@{
    static void Initialize();
    static void Terminate ();
    //@}

    //! @name Init
    //@{
    void InitializeDiplomacy( xml::xistream& xis );
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
    void RegisterObject  ( MIL_Object_ABC& object );
    void UnregisterObject( MIL_Object_ABC& object );

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
    E_Tristate IsAFriend  ( const MIL_Army_ABC& army )                   const;
    E_Tristate IsAnEnemy  ( const DEC_Knowledge_Population & knowledge ) const;
    E_Tristate IsAnEnemy  ( const MIL_Army_ABC& army )                   const;
    E_Tristate IsNeutral  ( const MIL_Army_ABC& army )                   const;
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
    void SendFullState              () ; //@TODO MGD Change MIL_Population sendFullState to be const;
    void SendKnowledge              () const;

    void OnReceiveMsgChangeDiplomacy( const ASN1T_MsgChangeDiplomacy& msg );
    //@}

private:
    //! @name Tools
    //@{
    E_Diplomacy GetDiplomacy( const MIL_Army_ABC& army ) const;
    void ReadLogistic           ( xml::xistream& xis );
    void ReadAutomat            ( xml::xistream& xis );
    void ReadSubordinate        ( xml::xistream& xis, MIL_Automate* pSuperior );
    void ReadDiplomacy          ( xml::xistream& xis );
    //@}

private:
    MIL_EntityManager&  manager_;
    const uint          nID_;
          std::string   strName_;
          E_Diplomacy   nType_;
    T_DiplomacyMap      diplomacies_;
    T_KnowledgeGroupMap knowledgeGroups_;
    T_ObjectSet         objects_;

    DEC_KnowledgeBlackBoard_Army* pKnowledgeBlackBoard_;

private:
    static MT_Converter< std::string, E_Diplomacy > diplomacyConverter_;
};

#endif // __MIL_Army_h_
