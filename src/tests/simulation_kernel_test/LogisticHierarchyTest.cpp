// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"

#include "MockLogisticHierarchyOwner_ABC.h"
#include "MockMIL_AutomateLOG.h"
#include <xeumeuleu/xml.hpp>
#include "Entities/Specialisations/LOG/LogisticHierarchy.h"
#include "Entities/Specialisations/LOG/LogisticLink_ABC.h"
#include "Entities/Specialisations/LOG/ObjectLogisticHierarchy.h"
#include "Entities/Agents/Units/Logistic/PHY_LogisticLevel.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationNature.h"
#include "protocol/ClientSenders.h"
#include <tools/iterator.h>
#include <boost/shared_ptr.hpp>

using namespace logistic;

namespace {
    struct DotationsFixture
    {
        DotationsFixture()
        {
            xml::xistringstream xisNat(
                "<natures>"
                "   <nature type='Solid' id='1'/>"
                "   <nature type='Liquid' id='2'/>"
                "</natures>" );
            PHY_DotationNature::Initialize( xisNat );

            xml::xistringstream xis(
                "<resources>"
                "   <resource name='Food' id='42' category='ration' nature='Solid' id-nature='1' package-mass='0.015' package-size='10' package-volume='0.01'/>"
                "   <resource name='Bidule' id='43' category='carburant' nature='Liquid' id-nature='2' package-mass='0.015' package-size='10' package-volume='0.01'/>"
                "</resources>" );
            PHY_DotationType::Initialize( xis );
        }

        ~DotationsFixture()
        {
            PHY_DotationType::Terminate();
            PHY_DotationNature::Terminate();
//            UrbanType::Terminate();
        }
    };


    template< typename T >
    void testIteratorContent( tools::Iterator< T& > inputIterator, const std::vector< T* >& wantedOutput )
    {
        std::vector< T* > output;
        while( inputIterator.HasMoreElements() )
            output.push_back( &inputIterator.NextElement() );
        BOOST_CHECK_EQUAL_COLLECTIONS( wantedOutput.begin(), wantedOutput.end(), output.begin(), output.end() );
    }

    /*template< typename T >
    void testIteratorContent( tools::Iterator< T > inputIterator, const T tmp[] )
    {
        std::vector< T > wantedOutput( tmp, tmp + sizeof( tmp ) / sizeof( T ) );
        testIteratorContent( inputIterator, wantedOutput );
    }*/

    template< typename T >
    void testIteratorContent( tools::Iterator< T& > inputIterator )
    {
        std::vector< T* > wantedOutput;
        testIteratorContent( inputIterator, wantedOutput );
    }
    
    template< typename T >
    void testIteratorContent( tools::Iterator< T& > inputIterator, T& elt1 )
    {
        std::vector< T* > wantedOutput;
        wantedOutput.push_back( &elt1 );
        testIteratorContent( inputIterator, wantedOutput );
    }
    
    template< typename T >
    void testIteratorContent( tools::Iterator< T& > inputIterator, T& elt1, T& elt2 )
    {
        std::vector< T* > wantedOutput;
        wantedOutput.push_back( &elt1 );
        wantedOutput.push_back( &elt2 );
        testIteratorContent( inputIterator, wantedOutput );
    }
}

BOOST_FIXTURE_TEST_SUITE( LogisticHierarchyTestSuite, DotationsFixture )

// -----------------------------------------------------------------------------
// Name: TestLogisticHierarchy
// Created: NLD 2011-01-19
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestLogisticHierarchy )
{
    {
        MockLogisticHierarchyOwner_ABC owner;
        LogisticHierarchy logHierarchy( owner );

        BOOST_CHECK_EQUAL( false, logHierarchy.HasSuperior() );
        BOOST_CHECK_EQUAL( (MIL_AutomateLOG*)0, logHierarchy.GetPrimarySuperior() );
             
        testIteratorContent( logHierarchy.CreateSuperiorsIterator() );
        testIteratorContent( logHierarchy.CreateSuperiorLinksIterator() );
    }

    {
        MockLogisticHierarchyOwner_ABC owner;
        MockMIL_AutomateLOG brainLog1( *(MIL_Automate*)0, PHY_LogisticLevel::logistic_base_ );
        MockMIL_AutomateLOG brainLog2( *(MIL_Automate*)0, PHY_LogisticLevel::logistic_base_ );

        LogisticHierarchy logHierarchy( owner, brainLog1 );

        BOOST_CHECK_EQUAL( true, logHierarchy.HasSuperior() );
        BOOST_CHECK_EQUAL( &brainLog1, logHierarchy.GetPrimarySuperior() );

        boost::shared_ptr< LogisticLink_ABC > linkBrainLog1 = logHierarchy.FindSuperiorLink( brainLog1 );
        BOOST_CHECK_NE( (LogisticLink_ABC*)0, linkBrainLog1.get() );
        BOOST_CHECK_EQUAL( &brainLog1, &linkBrainLog1->GetSuperior() );

        testIteratorContent< MIL_AutomateLOG >( logHierarchy.CreateSuperiorsIterator(), brainLog1 );
        testIteratorContent< const LogisticLink_ABC >( logHierarchy.CreateSuperiorLinksIterator(), *linkBrainLog1 );

        // Change links
        logHierarchy.ChangeLinks( &brainLog1, &brainLog2 );
        BOOST_CHECK_EQUAL( &brainLog2, logHierarchy.GetPrimarySuperior() );

        linkBrainLog1 = logHierarchy.FindSuperiorLink( brainLog1 );
        BOOST_CHECK_NE( (LogisticLink_ABC*)0, linkBrainLog1.get() );
        BOOST_CHECK_EQUAL( &brainLog1, &linkBrainLog1->GetSuperior() );

        boost::shared_ptr< LogisticLink_ABC > linkBrainLog2 = logHierarchy.FindSuperiorLink( brainLog2 );
        BOOST_CHECK_NE( (LogisticLink_ABC*)0, linkBrainLog2.get() );
        BOOST_CHECK_EQUAL( &brainLog2, &linkBrainLog2->GetSuperior() );

        testIteratorContent< MIL_AutomateLOG >( logHierarchy.CreateSuperiorsIterator(), brainLog2, brainLog1 );
        testIteratorContent< const LogisticLink_ABC >( logHierarchy.CreateSuperiorLinksIterator(), *linkBrainLog2, *linkBrainLog1 );

        // Change links : nominal and current are the same
        logHierarchy.ChangeLinks( &brainLog2, &brainLog2 );
        BOOST_CHECK_EQUAL( &brainLog2, logHierarchy.GetPrimarySuperior() );
        linkBrainLog2 = logHierarchy.FindSuperiorLink( brainLog2 );
        BOOST_CHECK_NE( (LogisticLink_ABC*)0, linkBrainLog2.get() );
        BOOST_CHECK_EQUAL( &brainLog2, &linkBrainLog2->GetSuperior() );

        testIteratorContent< MIL_AutomateLOG >( logHierarchy.CreateSuperiorsIterator(), brainLog2 );
        testIteratorContent< const LogisticLink_ABC >( logHierarchy.CreateSuperiorLinksIterator(), *linkBrainLog2 );
    }
}

// -----------------------------------------------------------------------------
// Name: TestLogisticHierarchyQuota
// Created: NLD 2011-01-19
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestLogisticHierarchyXml )
{
    MockLogisticHierarchyOwner_ABC owner;
    MockMIL_AutomateLOG brainLog( *(MIL_Automate*)0, PHY_LogisticLevel::logistic_base_ );

    xml::xistringstream xis(
        "<quotas>"
        "   <resource name=\"Bidule\" quantity=\"6666\"/>"
        "</quotas>" );
   
    LogisticHierarchy logHierarchy( owner, brainLog, xis );
    boost::shared_ptr< LogisticLink_ABC > linkBrainLog = logHierarchy.FindSuperiorLink( brainLog );
    BOOST_CHECK_EQUAL( linkBrainLog->GetQuota( *PHY_DotationType::FindDotationCategory( 43 ) ), 6666 );
}

// -----------------------------------------------------------------------------
// Name: TestLogisticHierarchyQuota
// Created: NLD 2011-01-19
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestLogisticHierarchyQuota )
{
    MockLogisticHierarchyOwner_ABC owner;
    
    MockMIL_AutomateLOG brainLog1( *(MIL_Automate*)0, PHY_LogisticLevel::logistic_base_ );
    MockMIL_AutomateLOG brainLog2( *(MIL_Automate*)0, PHY_LogisticLevel::logistic_base_ );

    LogisticHierarchy logHierarchy( owner, brainLog1 );
    boost::shared_ptr< LogisticLink_ABC > linkBrainLog1 = logHierarchy.FindSuperiorLink( brainLog1 );

    sword::MissionParameter quotaParameter;
    sword::MissionParameter_Value* parameterValue = quotaParameter.add_value();
    parameterValue->add_list()->set_identifier( 42 );
    parameterValue->add_list()->set_quantity( 4 );
    parameterValue = quotaParameter.add_value();
    parameterValue->add_list()->set_identifier( 43 );
    parameterValue->add_list()->set_quantity( 5 );

    linkBrainLog1->OnReceiveChangeQuotas( quotaParameter );

    BOOST_CHECK_EQUAL( linkBrainLog1->GetQuota( *PHY_DotationType::FindDotationCategory( 42 ) ), 4 );
    BOOST_CHECK_EQUAL( linkBrainLog1->GetQuota( *PHY_DotationType::FindDotationCategory( 43 ) ), 5 );

    linkBrainLog1->ConsumeQuota( *PHY_DotationType::FindDotationCategory( 42 ), 1.1 );

    BOOST_CHECK_EQUAL( linkBrainLog1->GetQuota( *PHY_DotationType::FindDotationCategory( 42 ) ), 2.9 );
}


// -----------------------------------------------------------------------------
// Name: TestLogisticHierarchySwitchHierarchy
// Created: NLD 2011-01-19
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestLogisticHierarchySwitchHierarchy )
{
    MockLogisticHierarchyOwner_ABC mainOwner;    
    MockMIL_AutomateLOG mainBrainLog1( *(MIL_Automate*)0, PHY_LogisticLevel::logistic_base_ );
    MockMIL_AutomateLOG mainBrainLog2( *(MIL_Automate*)0, PHY_LogisticLevel::logistic_base_ );
    LogisticHierarchy mainLogHierarchy( mainOwner, mainBrainLog1);
    mainLogHierarchy.ChangeLinks( &mainBrainLog1, &mainBrainLog2 );

    MockLogisticHierarchyOwner_ABC newOwner;
    MockMIL_AutomateLOG newBrainLog1( *(MIL_Automate*)0, PHY_LogisticLevel::logistic_base_ );
    MockMIL_AutomateLOG newBrainLog2( *(MIL_Automate*)0, PHY_LogisticLevel::logistic_base_ );
    LogisticHierarchy newLogHierarchy( mainOwner, newBrainLog1);
    newLogHierarchy.ChangeLinks( &newBrainLog1, &newBrainLog2 );

    testIteratorContent< MIL_AutomateLOG >( newLogHierarchy.CreateSuperiorsIterator(), newBrainLog2, newBrainLog1 );
    testIteratorContent< MIL_AutomateLOG >( mainLogHierarchy.CreateSuperiorsIterator(), mainBrainLog2, mainBrainLog1 );

    mainLogHierarchy.SwitchToHierarchy( newLogHierarchy );

    testIteratorContent< MIL_AutomateLOG >( newLogHierarchy.CreateSuperiorsIterator(), newBrainLog2, newBrainLog1 );
    testIteratorContent< MIL_AutomateLOG >( mainLogHierarchy.CreateSuperiorsIterator(), newBrainLog2, newBrainLog1 );

    mainLogHierarchy.SwitchBackToNominalHierarchy();

    testIteratorContent< MIL_AutomateLOG >( newLogHierarchy.CreateSuperiorsIterator(), newBrainLog2, newBrainLog1 );
    testIteratorContent< MIL_AutomateLOG >( mainLogHierarchy.CreateSuperiorsIterator(), mainBrainLog2, mainBrainLog1 );

    mainLogHierarchy.DisconnectFromHierarchy();

    testIteratorContent< MIL_AutomateLOG >( newLogHierarchy.CreateSuperiorsIterator(), newBrainLog2, newBrainLog1 );
    testIteratorContent< MIL_AutomateLOG >( mainLogHierarchy.CreateSuperiorsIterator() );

    mainLogHierarchy.SwitchBackToNominalHierarchy();

    testIteratorContent< MIL_AutomateLOG >( newLogHierarchy.CreateSuperiorsIterator(), newBrainLog2, newBrainLog1 );
    testIteratorContent< MIL_AutomateLOG >( mainLogHierarchy.CreateSuperiorsIterator(), mainBrainLog2, mainBrainLog1 );

    mainLogHierarchy.SwitchToHierarchy( newLogHierarchy );

    testIteratorContent< MIL_AutomateLOG >( newLogHierarchy.CreateSuperiorsIterator(), newBrainLog2, newBrainLog1 );
    testIteratorContent< MIL_AutomateLOG >( mainLogHierarchy.CreateSuperiorsIterator(), newBrainLog2, newBrainLog1 );
    
    mainLogHierarchy.DisconnectFromHierarchy();

    testIteratorContent< MIL_AutomateLOG >( newLogHierarchy.CreateSuperiorsIterator(), newBrainLog2, newBrainLog1 );
    testIteratorContent< MIL_AutomateLOG >( mainLogHierarchy.CreateSuperiorsIterator() );

    mainLogHierarchy.SwitchBackToNominalHierarchy();

    testIteratorContent< MIL_AutomateLOG >( newLogHierarchy.CreateSuperiorsIterator(), newBrainLog2, newBrainLog1 );
    testIteratorContent< MIL_AutomateLOG >( mainLogHierarchy.CreateSuperiorsIterator(), mainBrainLog2, mainBrainLog1 );
}

// -----------------------------------------------------------------------------
// Name: TestObjectLogisticHierarchy
// Created: NLD 2011-01-19
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestObjectLogisticHierarchy )
{
    MockMIL_AutomateLOG objectBrainLog1( *(MIL_Automate*)0, PHY_LogisticLevel::logistic_base_ );
    ObjectLogisticHierarchy objectLogHierarchy( objectBrainLog1 );
    testIteratorContent< MIL_AutomateLOG >( objectLogHierarchy.CreateSuperiorsIterator(), objectBrainLog1 );

    MockLogisticHierarchyOwner_ABC mainOwner;    
    MockMIL_AutomateLOG mainBrainLog1( *(MIL_Automate*)0, PHY_LogisticLevel::logistic_base_ );
    MockMIL_AutomateLOG mainBrainLog2( *(MIL_Automate*)0, PHY_LogisticLevel::logistic_base_ );
    LogisticHierarchy mainLogHierarchy( mainOwner, mainBrainLog1);
    mainLogHierarchy.ChangeLinks( &mainBrainLog1, &mainBrainLog2 );

    testIteratorContent< MIL_AutomateLOG >( mainLogHierarchy.CreateSuperiorsIterator(), mainBrainLog2, mainBrainLog1 );

    mainLogHierarchy.SwitchToHierarchy( objectLogHierarchy );
    testIteratorContent< MIL_AutomateLOG >( mainLogHierarchy.CreateSuperiorsIterator(), objectBrainLog1 );

    mainLogHierarchy.SwitchBackToNominalHierarchy();
    testIteratorContent< MIL_AutomateLOG >( mainLogHierarchy.CreateSuperiorsIterator(), mainBrainLog2, mainBrainLog1 );
}

BOOST_AUTO_TEST_SUITE_END()