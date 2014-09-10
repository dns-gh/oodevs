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

#ifndef __MIL_Army_ABC_h_
#define __MIL_Army_ABC_h_

#include "MIL.h"
#include "Tristate.h"
#include "ENT/ENT_Enums.h"
#include <boost/serialization/export.hpp>
#include <map>

class MIL_Object_ABC;
class DEC_Knowledge_Object;
class DEC_KnowledgeBlackBoard_Army;
class MIL_KnowledgeGroup;
class MIL_Formation;
class MIL_Inhabitant;
class MIL_Population;
class KnowledgeVisitor_ABC;
class MIL_Color;
class KnowledgesVisitor_ABC;
class KnowledgeGroupFactory_ABC;

namespace sword
{
    class MissionParameters;
    class UnitMagicAction;
}

namespace xml
{
    class xostream;
    class xistream;
}

// =============================================================================
// @class  MIL_Army
// Created: JVT 2004-08-03
// =============================================================================
class MIL_Army_ABC
{
public:
    //! @name Types
    //@{
    typedef std::map< unsigned int, boost::shared_ptr< MIL_KnowledgeGroup > > T_KnowledgeGroups;
    typedef std::map< unsigned long, MIL_Object_ABC* > T_Objects;
    //@}

public:
    //! @name Constructor/destructor
    //@{
    virtual ~MIL_Army_ABC() {}
    //@}

    //! @name CheckPoints / Interface
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int ) {}
    virtual void WriteODB         ( xml::xostream& xos ) const = 0;
    virtual void WriteDiplomacyODB( xml::xostream& xos ) const = 0;
    virtual void WriteKnowledges  ( xml::xostream& xos ) const = 0;
    //@}

    //! @name Hierarchy
    //@{
    virtual void RegisterFormation  ( MIL_Formation& formation ) = 0;
    virtual void UnregisterFormation( MIL_Formation& formation ) = 0;

    virtual void RegisterObject( MIL_Object_ABC& object ) = 0;
    virtual void UnregisterObject( MIL_Object_ABC& object ) = 0;

    virtual void RegisterPopulation( MIL_Population& population ) = 0;
    virtual void UnregisterPopulation( MIL_Population& population ) = 0;

    virtual void RegisterInhabitant( MIL_Inhabitant& inhabitant ) = 0;
    virtual void UnregisterInhabitant( MIL_Inhabitant& inhabitant ) = 0;
    //@}

    virtual void UpdateKnowledges(int currentTimeStep) = 0;
    virtual void CleanKnowledges () = 0;
    virtual void CleanDeletedAgentKnowledges() = 0;

    //! @name Network
    //@{
    virtual void OnReceiveUnitMagicAction( const sword::UnitMagicAction& msg ) = 0;
    virtual void SendCreation () const = 0;
    virtual void SendFullState() const = 0;
    virtual void SendKnowledge() const = 0;
    //@}

    //! @name Operations
    //@{
    virtual void Finalize() = 0;

    virtual bool IsPerceived( const DEC_Knowledge_Object& knowledge ) const = 0;

    virtual E_Tristate IsAFriend( const MIL_Army_ABC& army ) const = 0;
    virtual E_Tristate IsAnEnemy( const MIL_Army_ABC& army ) const = 0;
    virtual E_Tristate IsNeutral( const MIL_Army_ABC& army ) const = 0;

    virtual boost::shared_ptr< MIL_KnowledgeGroup > FindKnowledgeGroup( unsigned int nID ) const = 0;
    virtual boost::shared_ptr< MIL_KnowledgeGroup > FindCrowdKnowledgeGroup() const = 0;
    virtual void                RegisterKnowledgeGroup  ( const boost::shared_ptr< MIL_KnowledgeGroup >& knowledgeGroup ) = 0;
    virtual void                UnregisterKnowledgeGroup( const boost::shared_ptr< MIL_KnowledgeGroup >& knowledgeGroup ) = 0;

    virtual void InitializeDiplomacy( xml::xistream& xis ) = 0;
    virtual void ChangeDiplomacy( MIL_Army_ABC& other, E_Diplomacy diplomacy ) = 0;
    virtual void ApplyOnKnowledgeGroup( KnowledgeVisitor_ABC& ) = 0;
    //@}

    //! @name Accessors
    //@{
    virtual unsigned int                  GetID() const = 0;
    virtual const std::string&            GetName() const = 0;
    virtual DEC_KnowledgeBlackBoard_Army& GetKnowledge() const = 0;
    virtual const MIL_Color& GetColor() const = 0;
    virtual const T_KnowledgeGroups& GetKnowledgeGroups() const = 0;
    virtual const T_Objects& GetObjects() const = 0;
    //@}

    //! @name
    //@{
    bool operator== ( const MIL_Army_ABC& rhs ) const
    {
        return GetID() == rhs.GetID();
    }

    bool operator!= ( const MIL_Army_ABC& rhs ) const
    {
        return GetID() != rhs.GetID();
    }
    //@}
};

#endif // __MIL_Army_ABC_h_
