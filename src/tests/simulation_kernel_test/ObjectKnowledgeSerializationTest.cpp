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
//struct ASN1T_MagicActionCreateObject;
//struct ASN1T_ObjectAttributes;
//struct ASN1T_Location;

#include "simulation_kernel/Checkpoints/MIL_CheckPointInArchive.h"
#include "simulation_kernel/Checkpoints/MIL_CheckPointOutArchive.h"

#include "simulation_kernel/Entities/Objects/MIL_ObjectLoader.h"
#include "simulation_kernel/Entities/Objects/Object.h"
#include "simulation_kernel/Entities/MIL_Army_ABC.h"
#include "simulation_kernel/Entities/Objects/MIL_ObjectBuilder_ABC.h"
#include "simulation_kernel/Entities/Objects/MIL_ObjectType_ABC.h"

#include "simulation_kernel/Entities/Objects/BuildableCapacity.h"
#include "simulation_kernel/Entities/Objects/ConstructionAttribute.h"

#include "simulation_kernel/Knowledge/DEC_Knowledge_Object.h"

#include <xeumeuleu/xml.h>

#include "MockArmy.h"
#include "MockBuilder.h"

using namespace mockpp;

// -----------------------------------------------------------------------------
// Name: VerifyObjectKnowledge_Serialization
// Created: LDC 2009-07-16
// -----------------------------------------------------------------------------
// $$$$ LDC FIXME This test should be rewritten without the hidden dependency on MIL_AgentServer
//BOOST_AUTO_TEST_CASE( VerifyObjectKnowledge_Serialization )
//{
//    MIL_ObjectLoader loader;
//    
//    {
//        xml::xistringstream xis( "<objects>" 
//                "<object type='object'>"
//                    "<constructor unit-type='raw' default-consumption-mode='EnTravaux'>"
//                        "<buildable/>"                        
//                    "</constructor>"
//                "</object>" 
//            "</objects>"
//            ); 
//        BOOST_CHECK_NO_THROW( loader.Initialize( xis ) );
//    }
//    const MIL_ObjectType_ABC& type = loader.GetType( "object" );
//
//    MockArmy army;
//    MOCKPP_CHAINER_FOR( MockArmy, RegisterObject ) ( &army ).expects( mockpp::once() );
//
//    MIL_Object_ABC* pObject = 0;    
//    {
//        MockBuilder builder;
//        builder.GetType_mocker.expects( mockpp::once() ) .will( returnValue( &type ) );
//        MOCKPP_CHAINER_FOR( MockBuilder, Build )         ( &builder ).expects( mockpp::once() );
//        BOOST_CHECK_NO_THROW(
//            pObject = loader.CreateObject( builder, army );
//        );
//        builder.verify();
//    }
//
//    army.GetID_mocker.expects( mockpp::once() );//.will( returnValue( 42 ) );
//    DEC_Knowledge_Object knowledge( army, *pObject );
//    knowledge.Update( PHY_PerceptionLevel::identified_ );
//    std::stringstream stringstream;
//    {
//        MIL_CheckPointOutArchive outStream( stringstream );
//        outStream << knowledge;
//    }
//    {
//        MIL_CheckPointInArchive inStream( stringstream );
//        DEC_Knowledge_Object reloaded;
//        inStream >> reloaded;
//    }
//}