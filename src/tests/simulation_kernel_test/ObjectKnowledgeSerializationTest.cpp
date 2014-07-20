// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "SingletonTerminator.h"
#include "MockArmy.h"
#include "MockBuilder.h"
#include "MockMIL_Time_ABC.h"
#include "MockNET_Publisher_ABC.h"
#include "MockObjectTypeResolver.h"
#include "simulation_kernel/Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "simulation_kernel/Entities/Objects/MIL_ObjectFactory.h"
#include "simulation_kernel/Entities/Objects/MIL_ObjectType_ABC.h"
#include "simulation_kernel/Entities/Objects/Object.h"
#include "simulation_kernel/Knowledge/DEC_Knowledge_Object.h"
#include "simulation_kernel/Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "simulation_kernel/Tools/MIL_Color.h"
#include "simulation_terrain/TER_World.h"
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
            MOCK_EXPECT( GetID ).returns( 42u );
        }
        void save( MIL_CheckPointOutArchive& archive, const unsigned int ) const
        {
            archive << boost::serialization::base_object< MockArmy >( *this );
        }
    };

    struct ObjectKnowledgeSerializationFixture
    {
        ObjectKnowledgeSerializationFixture()
            : world_( "worldwide/tests/EmptyParis-ML" )
        {
            PHY_ConsumptionType::Initialize();
            MOCK_EXPECT( time.GetCurrentTimeStep ).returns( 1u );
        }
        ~ObjectKnowledgeSerializationFixture()
        {
            mock::verify();
        }
        MockMIL_Time_ABC time;

    private:
        SingletonTerminator terminator_;
        FakeWorld world_;
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
    MIL_ObjectFactory factory;
    {
        xml::xistringstream xis( "<objects>"
                                 "    <object type='object'>"
                                 "        <constructor unit-type='raw' default-consumption-mode='EnTravaux'>"
                                 "            <buildable/>"
                                 "        </constructor>"
                                 "    </object>"
                                 "</objects>" );
        factory.Initialize( xis );
    }
    const MIL_ObjectType_ABC& type = factory.FindType( "object" );

    ArmySerializationProxy army;
    std::unique_ptr< MIL_Object_ABC > pObject;
    {
        MockBuilder builder;
        MOCK_EXPECT( builder.GetType ).once().returns( boost::cref( type ) );
        MOCK_EXPECT( builder.Build ).once();
        MOCK_EXPECT( army.RegisterObject ).once();
        MIL_Color color;
        MOCK_EXPECT( army.GetColor ).once().returns( boost::cref( color ) );
        pObject.reset( factory.CreateObject( builder, &army ) );
        mock::verify( builder );
        mock::verify( army );
    }
    MOCK_EXPECT( army.UnregisterObject ).with( mock::same( *pObject ) ).once();

    const auto world = boost::shared_ptr< TER_World >();
    MockNET_Publisher_ABC publisher;
    MOCK_EXPECT( publisher.Send ).once(); // object knowledge creation
    MOCK_EXPECT( army.GetID ).returns( 42u );
    DEC_Knowledge_Object knowledge( army, *pObject );
    knowledge.Update( PHY_PerceptionLevel::identified_ );
    std::stringstream s;
    {
        MIL_CheckPointOutArchive out( s, world );
        out << knowledge;
    }
    {
        MockObjectTypeResolver resolver;
        MOCK_EXPECT( resolver.FindType ).exactly( 2 ).returns( boost::cref( type ) );
        MIL_CheckPointInArchive in( s, resolver, world );
        DEC_Knowledge_Object reloaded;
        in >> reloaded;
        MOCK_EXPECT( publisher.Send ).once(); // object knowledge destruction
    }
    MOCK_EXPECT( publisher.Send ).once(); // object knowledge destruction
}
