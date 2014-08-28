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
#include "MT_Tools/MT_String.h"
#include <tools/Resolver.h>
#include <tools/Map.h>
#include <boost/shared_ptr.hpp>

namespace sword
{
    class MissionParameters;
}

namespace xml
{
    class xostream;
    class xistream;
}

class ArmyFactory_ABC;
class AutomateFactory_ABC;
class DEC_KnowledgeBlackBoard_Army;
class DEC_Knowledge_Agent;
class DEC_Knowledge_Object;
class DEC_Knowledge_Population;
class FormationFactory_ABC;
class InhabitantFactory_ABC;
class KnowledgeGroupFactory_ABC;
class MIL_AutomateLOG;
class MIL_DictionaryExtensions;
class MIL_Formation;
class MIL_Inhabitant;
class MIL_KnowledgeGroup;
class MIL_ObjectManager;
class MIL_Object_ABC;
class MIL_Population;
class MIL_Color;
class PopulationFactory_ABC;

// =============================================================================
// @class  MIL_Army
// Created: JVT 2004-08-03
// =============================================================================
class MIL_Army : public MIL_Army_ABC
               , public tools::Resolver< MIL_Formation >
               , public tools::Resolver< MIL_Population >
               , public tools::Resolver< MIL_Inhabitant >
               , public tools::Resolver< MIL_Object_ABC >
               , private boost::noncopyable
{
public:
    //! @name Constructor/Destructor
    //@{
             MIL_Army( xml::xistream& xis, ArmyFactory_ABC& armyFactory, FormationFactory_ABC& formationFactory, AutomateFactory_ABC& automateFactory, MIL_ObjectManager& objectFactory,
                       PopulationFactory_ABC& populationFactory, InhabitantFactory_ABC& inhabitantFactory, KnowledgeGroupFactory_ABC& knowledgegroupFactory,
                       const MT_Converter< std::string, E_Diplomacy, sCaseInsensitiveLess >& diplomacyConverter, bool canCreateChildren );
    virtual ~MIL_Army();
     //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;

    virtual void WriteODB( xml::xostream& xos ) const;
    virtual void WriteDiplomacyODB( xml::xostream& xos ) const;
    virtual void WriteKnowledges( xml::xostream& xos ) const;
    //@}

    //! @name Init
    //@{
    virtual void InitializeDiplomacy( xml::xistream& xis );
    //@}

    //! @name Knowledge
    //@{
    virtual void UpdateKnowledges( int currentTimeStep );
    virtual void CleanKnowledges();
    virtual void CleanDeletedAgentKnowledges();
    virtual void ApplyOnKnowledgeGroup( KnowledgeVisitor_ABC& );
    //@}

    //! @name Hierarchy
    //@{
    virtual void RegisterFormation( MIL_Formation& formation );
    virtual void UnregisterFormation( MIL_Formation& formation );
    void RegisterObject( MIL_Object_ABC& object );
    void UnregisterObject( MIL_Object_ABC& object );

    void RegisterPopulation( MIL_Population& population );
    void UnregisterPopulation( MIL_Population& population );

    void RegisterInhabitant( MIL_Inhabitant& inhabitant );
    void UnregisterInhabitant( MIL_Inhabitant& inhabitant );

    boost::shared_ptr< MIL_KnowledgeGroup > FindKnowledgeGroup( unsigned int nID ) const;
    boost::shared_ptr< MIL_KnowledgeGroup > FindCrowdKnowledgeGroup() const;
    void RegisterKnowledgeGroup( const boost::shared_ptr< MIL_KnowledgeGroup > & knowledgeGroup );
    void UnregisterKnowledgeGroup( const boost::shared_ptr< MIL_KnowledgeGroup > & knowledgeGroup );
    //@}

    //! @name Operations
    //@{
    bool IsPerceived( const DEC_Knowledge_Object& knowledge ) const;
    E_Tristate IsAFriend( const DEC_Knowledge_Agent & knowledge ) const;
    virtual E_Tristate IsAFriend( const MIL_Army_ABC& army ) const;
    E_Tristate IsAnEnemy( const DEC_Knowledge_Population & knowledge ) const;
    virtual E_Tristate IsAnEnemy( const MIL_Army_ABC& army ) const;
    virtual E_Tristate IsNeutral( const MIL_Army_ABC& army ) const;
    virtual void Finalize();
    //@}

    //! @name Accessors
    //@{
    unsigned int GetID() const;
    const std::string& GetName() const;
    virtual const T_KnowledgeGroups& GetKnowledgeGroups() const;
    virtual const T_Objects& GetObjects() const;
    DEC_KnowledgeBlackBoard_Army& GetKnowledge() const;
    virtual const MIL_Color& GetColor() const;
    //@}

    //! @name Network
    //@{
    virtual void OnReceiveUnitMagicAction( const sword::UnitMagicAction& msg );
    virtual void SendCreation() const;
    virtual void SendFullState() const;
    virtual void SendKnowledge() const;
    virtual void ChangeDiplomacy( MIL_Army_ABC& other, E_Diplomacy diplomacy );
    //@}

private:
    //! @name Tools
    //@{
    E_Diplomacy GetDiplomacy( const MIL_Army_ABC& army ) const;
    void ReadFormation( xml::xistream& xis, FormationFactory_ABC& formationFactory );
    void ReadObject( xml::xistream& xis, MIL_ObjectManager& objectFactory );
    void ReadPopulation( xml::xistream& xis, PopulationFactory_ABC& populationFactory );
    void ReadInhabitant( xml::xistream& xis, InhabitantFactory_ABC& inhabitantFactory );
    void ReadKnowledgeGroup( xml::xistream& xis, KnowledgeGroupFactory_ABC& knowledgegroupFactory ); // LTO
    void ReadLogisticLink( xml::xistream& xis, AutomateFactory_ABC& automateFactory, FormationFactory_ABC& formationFactory );
    void ReadLogisticLinkSubordinate( xml::xistream& xis, AutomateFactory_ABC& automateFactory, FormationFactory_ABC& formationFactory , MIL_AutomateLOG& superior );
    void ReadDiplomacy( xml::xistream& xis );

    MIL_AutomateLOG* FindBrainLogistic( unsigned int nID, AutomateFactory_ABC& automateFactory, FormationFactory_ABC& formationFactory ) const; //$$$ Remplacer par une factory
    void OnReceiveRename( const sword::MissionParameters& parameters );
    //@}

    //! @name CheckPoint
    //@{
    MIL_Army( ArmyFactory_ABC& armyFactory, const MT_Converter< std::string, E_Diplomacy, sCaseInsensitiveLess >& diplomacyConverter );
    template< typename Archive > friend  void save_construct_data( Archive& archive, const MIL_Army* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, MIL_Army* role, const unsigned int /*version*/ );
    //@}

private:
    typedef tools::Map< const MIL_Army_ABC*, E_Diplomacy > T_DiplomacyMap;

private:
    //! @name Member data
    //@{
    unsigned int nID_;
    std::string strName_;
    E_Diplomacy nType_;
    T_DiplomacyMap diplomacies_;
    T_KnowledgeGroups knowledgeGroups_;
    ArmyFactory_ABC& armyFactory_;
    DEC_KnowledgeBlackBoard_Army* pKnowledgeBlackBoard_;
    const MT_Converter< std::string, E_Diplomacy, sCaseInsensitiveLess >& diplomacyConverter_;
    std::unique_ptr< MIL_DictionaryExtensions > pExtensions_;
    std::unique_ptr< MIL_Color > pColor_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_Army )

#endif // o
