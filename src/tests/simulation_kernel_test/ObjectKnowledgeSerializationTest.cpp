// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "MockArmy.h"
#include "MockBuilder.h"
#include "MockMIL_Time_ABC.h"
#include "MockNET_Publisher_ABC.h"
#include "StubTER_World.h"
#include "simulation_kernel/Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "simulation_kernel/Entities/Objects/MIL_ObjectLoader.h"
#include "simulation_kernel/Entities/Objects/Object.h"
#include "simulation_kernel/Knowledge/DEC_Knowledge_Object.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include <xeumeuleu/xml.hpp>

/**
 * StubArmy is used instead of MockArmy because it is deserialised,
 * and there is no way in MockArmy to say during deserialization that one expects some methods to be called.
 */
namespace
{
    class ArmySerializationProxy : public MockArmy
    {
    public:
        BOOST_SERIALIZATION_SPLIT_MEMBER();
        void load( MIL_CheckPointInArchive& archive, const unsigned int )
        {
            archive >> boost::serialization::base_object< MockArmy >( *this );
            MOCK_EXPECT( this, GetID ).returns( 42u );
        }
        void save( MIL_CheckPointOutArchive& archive, const unsigned int ) const
        {
            archive << boost::serialization::base_object< MockArmy >( *this );
        }
    };

    struct ObjectKnowledgeSerializationFixture
    {
        ObjectKnowledgeSerializationFixture()
            : group( "<knowledge-groups>"
            "    <knowledge-group name='Standard' communication-delay='01m'>"
            "        <unit-knowledge max-lifetime='03h' max-unit-to-knowledge-distance='60000'/>"
            "        <population-knowledge max-lifetime='2m'/>"
            "    </knowledge-group>"
            "</knowledge-groups>" )
        {
            WorldInitialize( "worldwide/Paris" );
            PHY_ConsumptionType::Initialize();
            MOCK_EXPECT( time, GetCurrentTick ).returns( 1u );
            MOCK_EXPECT( time, GetTickDuration ).returns( 10u );
            MIL_KnowledgeGroupType::Initialize( group );
        }
        ~ObjectKnowledgeSerializationFixture()
        {
            MIL_KnowledgeGroupType::Terminate();
            PHY_ConsumptionType::Terminate();
            TER_World::DestroyWorld();
            mock::verify();
        }
        MockMIL_Time_ABC time;
        xml::xistringstream group;
    };
}

BOOST_CLASS_EXPORT_IMPLEMENT( MockArmy )
BOOST_CLASS_EXPORT( ArmySerializationProxy )

// -----------------------------------------------------------------------------
// Name: VerifyObjectKnowledge_Serialization
// Created: LDC 2009-07-16
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( VerifyObjectKnowledge_Serialization, ObjectKnowledgeSerializationFixture )
{
    MIL_ObjectLoader loader;
    {
        xml::xistringstream xis( "<objects>"
                                 "    <object type='object'>"
                                 "        <constructor unit-type='raw' default-consumption-mode='EnTravaux'>"
                                 "            <buildable/>"
                                 "        </constructor>"
                                 "    </object>"
                                 "</objects>" );
        loader.Initialize( xis );
    }

    const MIL_ObjectType_ABC& type = loader.GetType( "object" );

    ArmySerializationProxy army;
    std::auto_ptr< MIL_Object_ABC > pObject;
    {
        MockBuilder builder;
        MOCK_EXPECT( builder, GetType ).once().returns( boost::cref( type ) );
        MOCK_EXPECT( builder, Build ).once();
        MOCK_EXPECT( army, RegisterObject ).once();
        pObject.reset( loader.CreateObject( builder, army ) );
        builder.verify();
        army.verify();
    }
    MOCK_EXPECT( army, UnregisterObject ).with( mock::same( *pObject ) ).once();

    MockNET_Publisher_ABC publisher;
    MOCK_EXPECT( publisher, Send ).exactly( 2 ); // object knowledge creation / knowledge group creation
    MOCK_EXPECT( army, GetID ).returns( 42u );
    MOCK_EXPECT( army, RegisterKnowledgeGroup );
    MOCK_EXPECT( army, UnregisterKnowledgeGroup );
    boost::shared_ptr< MIL_KnowledgeGroup > groupArmy( new MIL_KnowledgeGroup( *MIL_KnowledgeGroupType::FindType( "Standard" ), 30, army ) );
    army.RegisterKnowledgeGroup( groupArmy );
    DEC_Knowledge_Object knowledge( groupArmy, *pObject );
    knowledge.Update( PHY_PerceptionLevel::identified_ );
    std::stringstream s;
    {
        MIL_CheckPointOutArchive* out = new MIL_CheckPointOutArchive( s );
        ( *out ) << knowledge;
#ifndef _DEBUG //$$$$ boost + nedmalloc + binary_ioarchive + std::locale = crash
        delete out;
#endif
    }
    {
        MIL_CheckPointInArchive* in = new MIL_CheckPointInArchive( s );
        DEC_Knowledge_Object reloaded;
        ( *in ) >> reloaded;
        MOCK_EXPECT( publisher, Send ).once(); // object knowledge destruction
#ifndef _DEBUG //$$$$ boost + nedmalloc + binary_ioarchive + std::locale = crash
        delete in;
#endif
    }
    MOCK_EXPECT( publisher, Send ).once(); // object knowledge destruction
}
