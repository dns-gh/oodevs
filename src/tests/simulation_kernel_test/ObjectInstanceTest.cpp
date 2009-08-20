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
struct ASN1T_MagicActionCreateObject;
struct ASN1T_ObjectAttributes;
struct ASN1T_Location;

#include "simulation_kernel/Entities/Objects/MIL_ObjectLoader.h"
#include "simulation_kernel/Entities/Objects/Object.h"
#include "simulation_kernel/Entities/MIL_Army_ABC.h"
#include "simulation_kernel/Entities/Objects/MIL_ObjectBuilder_ABC.h"
#include "simulation_kernel/Entities/Objects/MIL_ObjectType_ABC.h"

#include "simulation_kernel/Entities/Objects/BuildableCapacity.h"
#include "simulation_kernel/Entities/Objects/ConstructionAttribute.h"

#include <xeumeuleu/xml.h>

#include "MockArmy.h"
#include "MockBuilder.h"

using namespace mockpp;


// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( VerifyObjectCapacity_Instance )
{
    MIL_ObjectLoader loader;
    
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
    const MIL_ObjectType_ABC& type = loader.GetType( "object" );

    MockArmy army;
    MOCKPP_CHAINER_FOR( MockArmy, RegisterObject ) ( &army ).expects( mockpp::once() );

    MIL_Object_ABC* pObject = 0;
    {
        MockBuilder builder;
        builder.GetType_mocker.expects( mockpp::once() ) .will( returnValue( &type ) );
        MOCKPP_CHAINER_FOR( MockBuilder, Build )( &builder ).expects( mockpp::once() );
        BOOST_CHECK_NO_THROW(
            pObject = loader.CreateObject( builder, army );
        );
        builder.verify();
    }

    BOOST_REQUIRE( pObject != 0 );
    BOOST_CHECK( pObject->GetID() > 0 );    
    BOOST_CHECK_EQUAL( &pObject->GetType(), &type );

    BOOST_CHECK( pObject->Retrieve< BuildableCapacity >() != 0 );
    BOOST_CHECK_NO_THROW( static_cast< Object* >( pObject )->GetAttribute< ConstructionAttribute >() );
    BOOST_CHECK_NO_THROW( pObject->GetAttribute< ConstructionAttribute >() );
    BOOST_CHECK_EQUAL( pObject->GetAttribute< ConstructionAttribute >().GetState(), 1. );

    army.verify();
}