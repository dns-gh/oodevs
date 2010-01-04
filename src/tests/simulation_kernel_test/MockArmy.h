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

class DEC_Knowledge_Object;
class DEC_KnowledgeBlackBoard_Army;
class MIL_KnowledgeGroup;

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
        , GetID_mocker                      ( "GetID", this )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetKnowledgeShadow )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( RegisterObject )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( UnregisterObject )        
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( IsPerceived )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( IsAFriend )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( IsAnEnemy )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( IsNeutral )
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

    MIL_KnowledgeGroup* FindKnowledgeGroup(uint) const
    {
        MOCKPP_NOT_IMPLEMENTED( __FUNCTION__ );
        throw;
    }

    virtual uint GetID() const
    {
        return GetID_mocker.forward();
    }

    mockpp::ChainableMockMethod< uint > GetID_mocker;
    MOCKPP_CONST_CHAINABLE_EXT0      ( MockArmy, const std::string*, GetNameShadow, std::string, );   
    MOCKPP_CONST_CHAINABLE_EXT0      ( MockArmy, DEC_KnowledgeBlackBoard_Army*, GetKnowledgeShadow, DEC_KnowledgeBlackBoard_Army, );   
    MOCKPP_VOID_CHAINABLE_EXT1       ( MockArmy, RegisterObject, MIL_Object_ABC&, , MIL_Object_ABC );   
    MOCKPP_VOID_CHAINABLE_EXT1       ( MockArmy, UnregisterObject, MIL_Object_ABC&, , MIL_Object_ABC );   
    MOCKPP_CONST_CHAINABLE_EXT1      ( MockArmy, bool, IsPerceived, const DEC_Knowledge_Object&, bool, , DEC_Knowledge_Object );
    MOCKPP_CONST_CHAINABLE_EXT1      ( MockArmy, E_Tristate, IsAFriend, const MIL_Army_ABC&, E_Tristate, , MIL_Army_ABC );
    MOCKPP_CONST_CHAINABLE_EXT1      ( MockArmy, E_Tristate, IsAnEnemy, const MIL_Army_ABC&, E_Tristate, , MIL_Army_ABC );
    MOCKPP_CONST_CHAINABLE_EXT1      ( MockArmy, E_Tristate, IsNeutral, const MIL_Army_ABC&, E_Tristate, , MIL_Army_ABC );
};

#endif // __MockArmy_h_
