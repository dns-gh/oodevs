// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"
#include "MockArmy.h"
#include "MockBuilder.h"
#include "simulation_kernel/Entities/Objects/MIL_ObjectFactory.h"
#include "simulation_kernel/Entities/Objects/Object.h"
#include "simulation_kernel/Entities/MIL_Army_ABC.h"
#include "simulation_kernel/Entities/Objects/MIL_ObjectBuilder_ABC.h"
#include "simulation_kernel/Entities/Objects/MIL_ObjectType_ABC.h"
#include "simulation_kernel/Entities/Objects/BuildableCapacity.h"
#include "simulation_kernel/Entities/Objects/ConstructionAttribute.h"
#include "simulation_kernel/Tools/MIL_Color.h"
#include "StubTER_World.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( VerifyObjectCapacity_Instance )
{
    FakeWorld world( "worldwide/tests/EmptyParis-ML" );
    MIL_ObjectFactory factory;
    {
        xml::xistringstream xis(
            "<objects>"
            "    <object type='object'>"
            "       <constructor unit-type='raw' default-consumption-mode='EnTravaux'>"
            "           <buildable/>"
            "       </constructor>"
            "   </object>"
            "</objects>" );
        BOOST_CHECK_NO_THROW( factory.Initialize( xis ) );
    }
    const MIL_ObjectType_ABC& type = factory.FindType( "object" );

    MIL_Color color;
    MockArmy army;
    MOCK_EXPECT( army.RegisterObject ).once();
    MOCK_EXPECT( army.GetColor ).once().returns( boost::cref( color ) );

    std::unique_ptr< MIL_Object_ABC > pObject;
    {
        MockBuilder builder;
        MOCK_EXPECT( builder.GetType ).once().returns( boost::cref( type ) );
        MOCK_EXPECT( builder.Build ).once();
        BOOST_CHECK_NO_THROW( pObject.reset( factory.CreateObject( builder, &army ) ) );
        mock::verify( builder );
    }

    BOOST_REQUIRE( pObject.get() != 0 );
    BOOST_CHECK( pObject->GetID() > 0 );
    BOOST_CHECK_EQUAL( &pObject->GetType(), &type );

    BOOST_CHECK( pObject->Retrieve< BuildableCapacity >() != 0 );
    BOOST_CHECK_NO_THROW( static_cast< Object* >( pObject.get() )->GetAttribute< ConstructionAttribute >() );
    BOOST_CHECK_NO_THROW( pObject->GetAttribute< ConstructionAttribute >() );
    BOOST_CHECK_EQUAL( pObject->GetAttribute< ConstructionAttribute >().GetState(), 1. );

    MOCK_EXPECT( army.UnregisterObject ).once();
    pObject.reset();
    mock::verify( army );
}
