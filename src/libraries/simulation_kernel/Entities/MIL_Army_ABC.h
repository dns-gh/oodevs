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

namespace sword
{
    class MissionParameters;
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
    //! @name Diplomacy Types
    //@{
    enum E_Diplomacy
    {
        eUnknown,
        eFriend,
        eEnemy,
        eNeutral
    };
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

    //! @name Network
    //@{
    virtual void SendCreation () const = 0;
    virtual void SendFullState() const = 0;
    virtual void SendKnowledge() const = 0;
    //@}

    //! @name Operations
    //@{
    virtual bool IsPerceived( const DEC_Knowledge_Object& knowledge ) const = 0;

    virtual E_Tristate IsAFriend( const MIL_Army_ABC& army ) const = 0;
    virtual E_Tristate IsAnEnemy( const MIL_Army_ABC& army ) const = 0;
    virtual E_Tristate IsNeutral( const MIL_Army_ABC& army ) const = 0;

    virtual MIL_KnowledgeGroup* FindKnowledgeGroup( unsigned int nID ) const = 0;
    virtual void                RegisterKnowledgeGroup  ( MIL_KnowledgeGroup& knowledgeGroup ) = 0;
    virtual void                UnregisterKnowledgeGroup( MIL_KnowledgeGroup& knowledgeGroup ) = 0;

    virtual void InitializeDiplomacy( xml::xistream& xis ) = 0;
    virtual void OnReceiveChangeDiplomacy( const sword::MissionParameters& msg ) = 0;
    virtual void ApplyOnKnowledgeGroup( KnowledgeVisitor_ABC& ) = 0;
    //@}

    //! @name Accessors
    //@{
    virtual unsigned int                  GetID() const = 0;
    virtual const std::string&            GetName() const = 0;
    virtual DEC_KnowledgeBlackBoard_Army& GetKnowledge() const = 0;
    virtual const MIL_Color& GetColor() const = 0;
    virtual const std::map< unsigned int, MIL_KnowledgeGroup* >& GetKnowledgeGroups() const = 0;
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
