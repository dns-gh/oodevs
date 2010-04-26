// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MockArmy_h_
#define __MockArmy_h_

#include "simulation_kernel/Entities/MIL_Army_ABC.h"
#include <boost/serialization/export.hpp>

class DEC_Knowledge_Object;
class DEC_KnowledgeBlackBoard_Army;
class MIL_KnowledgeGroup;

namespace xml
{
    class xostream;
    class xistream;
}

// =============================================================================
/** @class  MockArmy
    @brief  MockArmy
*/
// Created: JCR 2008-09-01
// =============================================================================
class MockArmy
    : public mockpp::ChainableMockObject
    , public MIL_Army_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MockArmy() 
        : mockpp::ChainableMockObject( MOCKPP_PCHAR( "MockArmy" ), 0 )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetNameShadow )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetID )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetKnowledgeShadow )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( RegisterObject )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( UnregisterObject )        
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( IsPerceived )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( IsAFriend )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( IsAnEnemy )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( IsNeutral )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( RegisterKnowledgeGroup )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( UnregisterKnowledgeGroup )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( RegisterFormation )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( UnregisterFormation )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( InitializeDiplomacy )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( OnReceiveMsgChangeDiplomacy )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( UpdateKnowledges )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( CleanKnowledges )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( WriteDiplomacyODB )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( WriteODB )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( SendCreation )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( SendFullState )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( SendKnowledge )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( FindKnowledgeGroup )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( ApplyOnKnowledgeGroup )
    {
    }
    virtual ~MockArmy() {}
    //@}
        
    virtual const std::string& GetName() const
    {
        return *GetNameShadow();
    }

    virtual DEC_KnowledgeBlackBoard_Army& GetKnowledge() const
    {
        return *GetKnowledgeShadow();
    }

//    MIL_KnowledgeGroup* FindKnowledgeGroup(uint) const
//    {
//        return *FindKnowledgeGroupShadow();
//    }
    
    virtual const std::map< unsigned int, MIL_KnowledgeGroup* >& GetKnowledgeGroups() const
    {
        throw;
    }

    template< typename Archive >
    void serialize( Archive&, const unsigned int ) 
    {
        boost::serialization::base_object< MIL_Army_ABC >( *this );
    }

    MOCKPP_CONST_CHAINABLE_EXT0      ( MockArmy, uint, GetID, void, );   
    MOCKPP_CONST_CHAINABLE_EXT0      ( MockArmy, const std::string*, GetNameShadow, std::string, );   
    MOCKPP_CONST_CHAINABLE_EXT0      ( MockArmy, DEC_KnowledgeBlackBoard_Army*, GetKnowledgeShadow, DEC_KnowledgeBlackBoard_Army, );   
    MOCKPP_VOID_CHAINABLE_EXT1       ( MockArmy, RegisterObject, MIL_Object_ABC&, , MIL_Object_ABC );   
    MOCKPP_VOID_CHAINABLE_EXT1       ( MockArmy, UnregisterObject, MIL_Object_ABC&, , MIL_Object_ABC );   
    MOCKPP_CONST_CHAINABLE_EXT1      ( MockArmy, bool, IsPerceived, const DEC_Knowledge_Object&, bool, , DEC_Knowledge_Object );
    MOCKPP_CONST_CHAINABLE_EXT1      ( MockArmy, E_Tristate, IsAFriend, const MIL_Army_ABC&, E_Tristate, , MIL_Army_ABC );
    MOCKPP_CONST_CHAINABLE_EXT1      ( MockArmy, E_Tristate, IsAnEnemy, const MIL_Army_ABC&, E_Tristate, , MIL_Army_ABC );
    MOCKPP_CONST_CHAINABLE_EXT1      ( MockArmy, E_Tristate, IsNeutral, const MIL_Army_ABC&, E_Tristate, , MIL_Army_ABC );
    MOCKPP_VOID_CHAINABLE_EXT1       ( MockArmy, RegisterKnowledgeGroup, MIL_KnowledgeGroup&, , MIL_KnowledgeGroup );   
    MOCKPP_VOID_CHAINABLE_EXT1       ( MockArmy, UnregisterKnowledgeGroup, MIL_KnowledgeGroup&, , MIL_KnowledgeGroup );   
    MOCKPP_VOID_CHAINABLE_EXT1       ( MockArmy, RegisterFormation, MIL_Formation&, , MIL_Formation );   
    MOCKPP_VOID_CHAINABLE_EXT1       ( MockArmy, UnregisterFormation, MIL_Formation&, , MIL_Formation);   
    MOCKPP_VOID_CHAINABLE_EXT1       ( MockArmy, InitializeDiplomacy, xml::xistream&,  ,  xml::xistream);   
    MOCKPP_VOID_CHAINABLE_EXT1       ( MockArmy, OnReceiveMsgChangeDiplomacy, const Common::MsgMissionParameters&, , const Common::MsgMissionParameters );   
    MOCKPP_VOID_CONST_CHAINABLE_EXT1 ( MockArmy, WriteODB, xml::xostream&, , xml::xostream );  
    MOCKPP_VOID_CONST_CHAINABLE_EXT1 ( MockArmy, WriteDiplomacyODB, xml::xostream& , , xml::xostream );   
    MOCKPP_VOID_CHAINABLE_EXT1       ( MockArmy, UpdateKnowledges, int, , int);  
    MOCKPP_VOID_CHAINABLE0           ( MockArmy, CleanKnowledges );   
    MOCKPP_VOID_CONST_CHAINABLE0     ( MockArmy, SendCreation );   
    MOCKPP_VOID_CONST_CHAINABLE0     ( MockArmy, SendFullState );   
    MOCKPP_VOID_CONST_CHAINABLE0     ( MockArmy, SendKnowledge );
    MOCKPP_CONST_CHAINABLE_EXT1      ( MockArmy, MIL_KnowledgeGroup*, FindKnowledgeGroup, unsigned int, MIL_KnowledgeGroup*, , unsigned int );
    MOCKPP_VOID_CHAINABLE_EXT1       ( MockArmy, ApplyOnKnowledgeGroup, KnowledgeVisitor_ABC&, , KnowledgeVisitor_ABC );
};

BOOST_CLASS_EXPORT_KEY( MockArmy )

#endif // __MockArmy_h_
