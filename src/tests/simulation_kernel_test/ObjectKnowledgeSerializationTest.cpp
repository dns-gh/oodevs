// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"

// ASN
//struct MagicActionCreateObject;
//struct MsgObjectAttributes;
//struct Location;

#include "simulation_kernel/Checkpoints/MIL_CheckPointInArchive.h"
#include "simulation_kernel/Checkpoints/MIL_CheckPointOutArchive.h"

#include "simulation_kernel/Entities/Objects/MIL_ObjectLoader.h"
#include "simulation_kernel/Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "simulation_kernel/Entities/MIL_Army_ABC.h"
#include "simulation_kernel/Entities/Objects/BuildableCapacity.h"
#include "simulation_kernel/Entities/Objects/ConstructionAttribute.h"
#include "simulation_kernel/Entities/Objects/MIL_ObjectBuilder_ABC.h"
#include "simulation_kernel/Entities/Objects/MIL_ObjectFactory.h"
#include "simulation_kernel/Entities/Objects/MIL_ObjectManager.h"
#include "simulation_kernel/Entities/Objects/MIL_ObjectType_ABC.h"
#include "simulation_kernel/Entities/Objects/Object.h"

#include "simulation_kernel/Knowledge/DEC_Knowledge_Object.h"

#include "simulation_terrain/TER_World.h"

#include <xeumeuleu/xml.h>

#include "MockArmy.h"
#include "MockBuilder.h"
#include "MockMIL_Time_ABC.h"
#include "MockNET_Publisher_ABC.h"

using namespace mockpp;

/**
 * StubArmy is used instead of MockArmy because it is deserialised,
 * and there is no way in MockArmy to say during deserialization that one expects some methods to be called.
 */
class StubArmy : public MockArmy
{
    public:

        virtual void RegisterObject( MIL_Object_ABC& object ) {}        
        virtual uint GetID() const { return 1; }
    template< typename Archive >
    void serialize( Archive&, const unsigned int ) 
    {
        boost::serialization::base_object< MockArmy >( *this );
    }
        
};
BOOST_CLASS_EXPORT_IMPLEMENT( MockArmy )
BOOST_CLASS_EXPORT( StubArmy )

// -----------------------------------------------------------------------------
// Name: VerifyObjectKnowledge_Serialization
// Created: LDC 2009-07-16
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( VerifyObjectKnowledge_Serialization )
{
    MIL_ObjectLoader loader;
    PHY_ConsumptionType::Initialize();
    {
        xml::xistringstream xis( "<objects>" 
                "<object type='object'>"
                    "<constructor unit-type='raw' default-consumption-mode='EnTravaux'>"
                        "<buildable/>"                        
                    "</constructor>"
                "</object>" 
            "</objects>"
            ); 
        BOOST_CHECK_NO_THROW( loader.Initialize( xis ) );
    }
    const MIL_ObjectType_ABC& type =  loader.GetType( "object" );

    StubArmy army;
    
    MIL_Object_ABC* pObject = 0;    
    {
        MockBuilder builder;
        builder.GetType_mocker.expects( mockpp::once() ) .will( returnValue( &type ) );
        MOCKPP_CHAINER_FOR( MockBuilder, Build )         ( &builder ).expects( mockpp::once() );
        BOOST_CHECK_NO_THROW(
            pObject = loader.CreateObject( builder, army );
        );
        builder.verify();
    }

    MockNET_Publisher_ABC publisher;
    MOCK_EXPECT( publisher, Send ).once();
    DEC_Knowledge_Object knowledge( army, *pObject );
    MockMIL_Time_ABC time;
    MOCK_EXPECT( time, GetCurrentTick ).returns( 1u );
    knowledge.Update( PHY_PerceptionLevel::identified_ );
    std::stringstream stringstream;
    {
        MIL_CheckPointOutArchive outStream( stringstream );
        outStream << knowledge;
    }
    MOCK_EXPECT( publisher, Send ).at_least( 1 );
    {
        TER_World::Initialize( "../../data/data/terrains/Paris_Est/Terrain.xml" );
        MIL_CheckPointInArchive inStream( stringstream );
        DEC_Knowledge_Object reloaded;
        inStream >> reloaded;
        TER_World::DestroyWorld();
    }
    PHY_ConsumptionType::Terminate();
}