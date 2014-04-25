// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_test_pch.h"

#include "SingletonTerminator.h"
#include "MockLogisticHierarchyOwner_ABC.h"
#include "MockMIL_AutomateLOG.h"
#include <xeumeuleu/xml.hpp>
#include "Entities/Specialisations/LOG/LogisticHierarchy.h"
#include "Entities/Specialisations/LOG/LogisticLink_ABC.h"
#include "Entities/Specialisations/LOG/ObjectLogisticHierarchy.h"
#include "Entities/Agents/Units/Logistic/PHY_LogisticLevel.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationLogisticType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationNature.h"
#include "protocol/ClientSenders.h"
#include <tools/iterator.h>
#include <boost/shared_ptr.hpp>
#include <boost/assign/list_of.hpp>

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

            xml::xistringstream xisLogClass(
                "<logistic-supply-classes>"
                "   <logistic-supply-class id='12' type='whatever'/>"
                "</logistic-supply-classes>" );
            PHY_DotationLogisticType::Initialize( xisLogClass );

            xml::xistringstream xis(
                "<resources>"
                "   <resource name='Food' id='42' category='ration' nature='Solid' logistic-supply-class='whatever' id-nature='1' package-mass='0.015' package-size='10' package-volume='0.01'/>"
                "   <resource name='Bidule' id='43' category='carburant' nature='Liquid' id-nature='2' logistic-supply-class='whatever' package-mass='0.015' package-size='10' package-volume='0.01'/>"
                "</resources>" );
            PHY_DotationType::Initialize( xis );
        }

    private:
        SingletonTerminator terminator_;
    };

    void testSuperiors( tools::Iterator< MIL_AutomateLOG& > inputIterator, std::vector< MIL_AutomateLOG* > wantedOutput = std::vector< MIL_AutomateLOG* >() )
    {
        std::vector< MIL_AutomateLOG* > output;
        while( inputIterator.HasMoreElements() )
            output.push_back( &inputIterator.NextElement() );
        BOOST_CHECK_EQUAL_COLLECTIONS( wantedOutput.begin(), wantedOutput.end(), output.begin(), output.end() );
    }

    void testLinks( tools::Iterator< boost::shared_ptr< LogisticLink_ABC > > inputIterator, std::vector< boost::shared_ptr< LogisticLink_ABC > > wantedOutput = std::vector< boost::shared_ptr< LogisticLink_ABC > >() )
    {
        std::vector< boost::shared_ptr< LogisticLink_ABC > > output;
        while( inputIterator.HasMoreElements() )
            output.push_back( inputIterator.NextElement() );
        BOOST_CHECK_EQUAL_COLLECTIONS( wantedOutput.begin(), wantedOutput.end(), output.begin(), output.end() );
    }

}

BOOST_FIXTURE_TEST_SUITE( LogisticHierarchyTestSuite, DotationsFixture )

namespace
{
    bool HasSuperior( const logistic::LogisticLink_ABC& link, const MIL_AutomateLOG* superior )
    {
        return &link.GetSuperior() == superior;
    }
}

// -----------------------------------------------------------------------------
// Name: TestLogisticHierarchy
// Created: NLD 2011-01-19
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestLogisticHierarchy )
{
    {
        MockLogisticHierarchyOwner_ABC owner;
        LogisticHierarchy logHierarchy( owner, true );

        BOOST_CHECK_EQUAL( false, logHierarchy.HasSuperior() );
        BOOST_CHECK_EQUAL( (MIL_AutomateLOG*)0, logHierarchy.GetPrimarySuperior() );

        testSuperiors( logHierarchy.CreateSuperiorsIterator() );
        testLinks( logHierarchy.CreateSuperiorLinksIterator() );
    }

    {
        MockLogisticHierarchyOwner_ABC owner;
        MockMIL_AutomateLOG brainLog1( *(MIL_Automate*)0, PHY_LogisticLevel::logistic_base_ );
        MockMIL_AutomateLOG brainLog2( *(MIL_Automate*)0, PHY_LogisticLevel::logistic_base_ );

        LogisticHierarchy logHierarchy( owner, brainLog1, true );

        BOOST_CHECK_EQUAL( true, logHierarchy.HasSuperior() );
        BOOST_CHECK_EQUAL( &brainLog1, logHierarchy.GetPrimarySuperior() );

        boost::shared_ptr< LogisticLink_ABC > linkBrainLog1 = logHierarchy.FindSuperiorLink( brainLog1 );
        BOOST_CHECK_NE( (LogisticLink_ABC*)0, linkBrainLog1.get() );
        BOOST_CHECK_EQUAL( &brainLog1, &linkBrainLog1->GetSuperior() );

        testSuperiors( logHierarchy.CreateSuperiorsIterator(), boost::assign::list_of( &brainLog1 ) );
        testLinks( logHierarchy.CreateSuperiorLinksIterator(), boost::assign::list_of( linkBrainLog1 ) );

        // Change links
        MOCK_EXPECT( owner.NotifyLinkAdded ).once().with( boost::bind( &HasSuperior, _1, &brainLog2 ) );
        logHierarchy.ChangeLinks( boost::assign::list_of( &brainLog1 )( &brainLog2 ) );
        BOOST_CHECK_EQUAL( &brainLog2, logHierarchy.GetPrimarySuperior() );

        linkBrainLog1 = logHierarchy.FindSuperiorLink( brainLog1 );
        BOOST_CHECK_NE( (LogisticLink_ABC*)0, linkBrainLog1.get() );
        BOOST_CHECK_EQUAL( &brainLog1, &linkBrainLog1->GetSuperior() );

        boost::shared_ptr< LogisticLink_ABC > linkBrainLog2 = logHierarchy.FindSuperiorLink( brainLog2 );
        BOOST_CHECK_NE( (LogisticLink_ABC*)0, linkBrainLog2.get() );
        BOOST_CHECK_EQUAL( &brainLog2, &linkBrainLog2->GetSuperior() );

        testSuperiors( logHierarchy.CreateSuperiorsIterator(), boost::assign::list_of( &brainLog2 )( &brainLog1 ) );
        testLinks( logHierarchy.CreateSuperiorLinksIterator(), boost::assign::list_of( linkBrainLog2 ) ( linkBrainLog1 ) );

        // Change links : nominal and current are the same
        MOCK_EXPECT( owner.NotifyLinkRemoved ).once().with( boost::bind( &HasSuperior, _1, &brainLog1 ) );
        logHierarchy.ChangeLinks( boost::assign::list_of( &brainLog2 )( &brainLog2 ) );
        BOOST_CHECK_EQUAL( &brainLog2, logHierarchy.GetPrimarySuperior() );
        linkBrainLog2 = logHierarchy.FindSuperiorLink( brainLog2 );
        BOOST_CHECK_NE( (LogisticLink_ABC*)0, linkBrainLog2.get() );
        BOOST_CHECK_EQUAL( &brainLog2, &linkBrainLog2->GetSuperior() );

        testSuperiors( logHierarchy.CreateSuperiorsIterator(), boost::assign::list_of( &brainLog2 ) );
        testLinks( logHierarchy.CreateSuperiorLinksIterator(), boost::assign::list_of( linkBrainLog2 ) );
    }
}

// -----------------------------------------------------------------------------
// Name: TestLogisticHierarchyXml
// Created: NLD 2011-01-19
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestLogisticHierarchyXml )
{
    MockLogisticHierarchyOwner_ABC owner;
    MockMIL_AutomateLOG brainLog( *(MIL_Automate*)0, PHY_LogisticLevel::logistic_base_ );

    xml::xistringstream xis(
        "<quotas>"
        "   <resource name='Bidule' quantity='6666'/>"
        "</quotas>" );

    LogisticHierarchy logHierarchy( owner, brainLog, true, xis );
    boost::shared_ptr< LogisticLink_ABC > linkBrainLog = logHierarchy.FindSuperiorLink( brainLog );
    MOCK_EXPECT( owner.NotifyQuotaThresholdReached ).once();
    std::vector< const MIL_AgentPion* > requesters;
    BOOST_CHECK_EQUAL( linkBrainLog->ConsumeQuota( *PHY_DotationType::FindDotationCategory( 43 ), 10000, requesters ), 6666 );
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

    LogisticHierarchy logHierarchy( owner, brainLog1, true );
    boost::shared_ptr< LogisticLink_ABC > linkBrainLog1 = logHierarchy.FindSuperiorLink( brainLog1 );

    sword::MissionParameter quotaParameter;
    sword::MissionParameter_Value* parameterValue = quotaParameter.add_value();
    parameterValue->add_list()->set_identifier( 42 );
    parameterValue->add_list()->set_quantity( 4 );
    parameterValue = quotaParameter.add_value();
    parameterValue->add_list()->set_identifier( 43 );
    parameterValue->add_list()->set_quantity( 5 );

    linkBrainLog1->OnReceiveChangeQuotas( quotaParameter );

    const MIL_AgentPion* pion = reinterpret_cast< const MIL_AgentPion* >( 42 );
    const std::vector< const MIL_AgentPion* > requesters( 1, pion );
    const std::vector< const MIL_AgentPion* > empty;
    MOCK_EXPECT( owner.NotifyQuotaThresholdReached ).once();
    BOOST_CHECK_EQUAL( linkBrainLog1->ConsumeQuota( *PHY_DotationType::FindDotationCategory( 43 ), 10, requesters ), 5 );
    MOCK_EXPECT( owner.NotifyQuotaThresholdReached ).once();
    BOOST_CHECK_EQUAL( linkBrainLog1->ConsumeQuota( *PHY_DotationType::FindDotationCategory( 42 ), 10, requesters ), 4 );
    MOCK_EXPECT( owner.NotifyQuotaExceeded ).once().with( mock::any, requesters );
    BOOST_CHECK_EQUAL( linkBrainLog1->ConsumeQuota( *PHY_DotationType::FindDotationCategory( 42 ), 10, requesters ), 0 );
    MOCK_EXPECT( owner.NotifyQuotaExceeded ).once().with( mock::any, empty );
    BOOST_CHECK_EQUAL( linkBrainLog1->ConsumeQuota( *PHY_DotationType::FindDotationCategory( 42 ), 10, requesters ), 0 );
    linkBrainLog1->ReturnQuota( *PHY_DotationType::FindDotationCategory( 42 ), 1.1 );
    MOCK_EXPECT( owner.NotifyQuotaThresholdReached ).once();
    BOOST_CHECK_EQUAL( linkBrainLog1->ConsumeQuota( *PHY_DotationType::FindDotationCategory( 42 ), 10, requesters ), 1.1 );
}

// -----------------------------------------------------------------------------
// Name: TestLogisticHierarchyQuota
// Created: NLD 2011-01-19
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestLogisticHierarchyNoQuota )
{
    MockLogisticHierarchyOwner_ABC owner;

    MockMIL_AutomateLOG brainLog1( *(MIL_Automate*)0, PHY_LogisticLevel::logistic_base_ );
    MockMIL_AutomateLOG brainLog2( *(MIL_Automate*)0, PHY_LogisticLevel::logistic_base_ );

    LogisticHierarchy logHierarchy( owner, brainLog1, false );
    boost::shared_ptr< LogisticLink_ABC > linkBrainLog1 = logHierarchy.FindSuperiorLink( brainLog1 );

    sword::MissionParameter quotaParameter;
    sword::MissionParameter_Value* parameterValue = quotaParameter.add_value();
    parameterValue->add_list()->set_identifier( 42 );
    parameterValue->add_list()->set_quantity( 4 );
    parameterValue = quotaParameter.add_value();
    parameterValue->add_list()->set_identifier( 43 );
    parameterValue->add_list()->set_quantity( 5 );

    linkBrainLog1->OnReceiveChangeQuotas( quotaParameter );

    std::vector< const MIL_AgentPion* > requesters;
    BOOST_CHECK_EQUAL( linkBrainLog1->ConsumeQuota( *PHY_DotationType::FindDotationCategory( 43 ), 10, requesters ), 10 );
    BOOST_CHECK_EQUAL( linkBrainLog1->ConsumeQuota( *PHY_DotationType::FindDotationCategory( 42 ), 10, requesters ), 10 );
    BOOST_CHECK_EQUAL( linkBrainLog1->ConsumeQuota( *PHY_DotationType::FindDotationCategory( 42 ), 10, requesters ), 10 );
    linkBrainLog1->ReturnQuota( *PHY_DotationType::FindDotationCategory( 42 ), 1.1 );
    BOOST_CHECK_EQUAL( linkBrainLog1->ConsumeQuota( *PHY_DotationType::FindDotationCategory( 42 ), 10, requesters ), 10 );
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
    LogisticHierarchy mainLogHierarchy( mainOwner, mainBrainLog1, true );

    MOCK_EXPECT( mainOwner.NotifyLinkAdded ).once().with( boost::bind( &HasSuperior, _1, &mainBrainLog2 ) );
    mainLogHierarchy.ChangeLinks( boost::assign::list_of( &mainBrainLog1 )( &mainBrainLog2 ) );
    mock::verify();

    MockMIL_AutomateLOG newBrainLog1( *(MIL_Automate*)0, PHY_LogisticLevel::logistic_base_ );
    MockMIL_AutomateLOG newBrainLog2( *(MIL_Automate*)0, PHY_LogisticLevel::logistic_base_ );
    LogisticHierarchy newLogHierarchy( mainOwner, newBrainLog1, true );

    MOCK_EXPECT( mainOwner.NotifyLinkAdded ).once().with( boost::bind( &HasSuperior, _1, &newBrainLog2 ) );
    newLogHierarchy.ChangeLinks( boost::assign::list_of( &newBrainLog1 )( &newBrainLog2 ) );
    mock::verify();

    testSuperiors( newLogHierarchy.CreateSuperiorsIterator(), boost::assign::list_of( &newBrainLog2 )( &newBrainLog1 ) );
    testSuperiors( mainLogHierarchy.CreateSuperiorsIterator(), boost::assign::list_of( &mainBrainLog2 )( &mainBrainLog1 ) );

    MOCK_EXPECT( mainOwner.NotifyLinkAdded ).once().with( boost::bind( &HasSuperior, _1, &newBrainLog1 ) );
    MOCK_EXPECT( mainOwner.NotifyLinkAdded ).once().with( boost::bind( &HasSuperior, _1, &newBrainLog2 ) );
    MOCK_EXPECT( mainOwner.NotifyLinkRemoved ).once().with( boost::bind( &HasSuperior, _1, &mainBrainLog1 ) );
    MOCK_EXPECT( mainOwner.NotifyLinkRemoved ).once().with( boost::bind( &HasSuperior, _1, &mainBrainLog2 ) );
    mainLogHierarchy.SwitchToHierarchy( newLogHierarchy );
    mock::verify();

    testSuperiors( newLogHierarchy.CreateSuperiorsIterator(), boost::assign::list_of( &newBrainLog2 )( &newBrainLog1 ) );
    testSuperiors( mainLogHierarchy.CreateSuperiorsIterator(), boost::assign::list_of( &newBrainLog2 )( &newBrainLog1 ) );

    MOCK_EXPECT( mainOwner.NotifyLinkRemoved ).once().with( boost::bind( &HasSuperior, _1, &newBrainLog1 ) );
    MOCK_EXPECT( mainOwner.NotifyLinkRemoved ).once().with( boost::bind( &HasSuperior, _1, &newBrainLog2 ) );
    MOCK_EXPECT( mainOwner.NotifyLinkAdded ).once().with( boost::bind( &HasSuperior, _1, &mainBrainLog1 ) );
    MOCK_EXPECT( mainOwner.NotifyLinkAdded ).once().with( boost::bind( &HasSuperior, _1, &mainBrainLog2 ) );
    mainLogHierarchy.SwitchBackToNominalHierarchy();
    mock::verify();

    testSuperiors( newLogHierarchy.CreateSuperiorsIterator(), boost::assign::list_of( &newBrainLog2 )( &newBrainLog1 ) );
    testSuperiors( mainLogHierarchy.CreateSuperiorsIterator(), boost::assign::list_of( &mainBrainLog2 )( &mainBrainLog1 ) );

    MOCK_EXPECT( mainOwner.NotifyLinkRemoved ).once().with( boost::bind( &HasSuperior, _1, &mainBrainLog1 ) );
    MOCK_EXPECT( mainOwner.NotifyLinkRemoved ).once().with( boost::bind( &HasSuperior, _1, &mainBrainLog2 ) );
    mainLogHierarchy.DisconnectFromHierarchy();
    mock::verify();

    testSuperiors( newLogHierarchy.CreateSuperiorsIterator(), boost::assign::list_of( &newBrainLog2 )( &newBrainLog1 ) );
    testSuperiors( mainLogHierarchy.CreateSuperiorsIterator() );

    MOCK_EXPECT( mainOwner.NotifyLinkAdded ).once().with( boost::bind( &HasSuperior, _1, &mainBrainLog1 ) );
    MOCK_EXPECT( mainOwner.NotifyLinkAdded ).once().with( boost::bind( &HasSuperior, _1, &mainBrainLog2 ) );
    mainLogHierarchy.SwitchBackToNominalHierarchy();
    mock::verify();

    testSuperiors( newLogHierarchy.CreateSuperiorsIterator(), boost::assign::list_of( &newBrainLog2 )( &newBrainLog1 ) );
    testSuperiors( mainLogHierarchy.CreateSuperiorsIterator(), boost::assign::list_of( &mainBrainLog2 )( &mainBrainLog1 ) );

    MOCK_EXPECT( mainOwner.NotifyLinkAdded ).once().with( boost::bind( &HasSuperior, _1, &newBrainLog1 ) );
    MOCK_EXPECT( mainOwner.NotifyLinkAdded ).once().with( boost::bind( &HasSuperior, _1, &newBrainLog2 ) );
    MOCK_EXPECT( mainOwner.NotifyLinkRemoved ).once().with( boost::bind( &HasSuperior, _1, &mainBrainLog1 ) );
    MOCK_EXPECT( mainOwner.NotifyLinkRemoved ).once().with( boost::bind( &HasSuperior, _1, &mainBrainLog2 ) );
    mainLogHierarchy.SwitchToHierarchy( newLogHierarchy );
    mock::verify();

    testSuperiors( newLogHierarchy.CreateSuperiorsIterator(), boost::assign::list_of( &newBrainLog2 )( &newBrainLog1 ) );
    testSuperiors( mainLogHierarchy.CreateSuperiorsIterator(), boost::assign::list_of( &newBrainLog2 )( &newBrainLog1 ) );

    MOCK_EXPECT( mainOwner.NotifyLinkRemoved ).once().with( boost::bind( &HasSuperior, _1, &newBrainLog1 ) );
    MOCK_EXPECT( mainOwner.NotifyLinkRemoved ).once().with( boost::bind( &HasSuperior, _1, &newBrainLog2 ) );
    mainLogHierarchy.DisconnectFromHierarchy();
    mock::verify();

    testSuperiors( newLogHierarchy.CreateSuperiorsIterator(), boost::assign::list_of( &newBrainLog2 )( &newBrainLog1 ) );
    testSuperiors( mainLogHierarchy.CreateSuperiorsIterator() );

    MOCK_EXPECT( mainOwner.NotifyLinkAdded ).once().with( boost::bind( &HasSuperior, _1, &mainBrainLog1 ) );
    MOCK_EXPECT( mainOwner.NotifyLinkAdded ).once().with( boost::bind( &HasSuperior, _1, &mainBrainLog2 ) );
    mainLogHierarchy.SwitchBackToNominalHierarchy();

    testSuperiors( newLogHierarchy.CreateSuperiorsIterator(), boost::assign::list_of( &newBrainLog2 )( &newBrainLog1 ) );
    testSuperiors( mainLogHierarchy.CreateSuperiorsIterator(), boost::assign::list_of( &mainBrainLog2 )( &mainBrainLog1 ) );
}

// -----------------------------------------------------------------------------
// Name: TestObjectLogisticHierarchy
// Created: NLD 2011-01-19
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( TestObjectLogisticHierarchy )
{
    MockMIL_AutomateLOG objectBrainLog1( *(MIL_Automate*)0, PHY_LogisticLevel::logistic_base_ );
    ObjectLogisticHierarchy objectLogHierarchy( objectBrainLog1 );
    testSuperiors( objectLogHierarchy.CreateSuperiorsIterator(), boost::assign::list_of( &objectBrainLog1 ) );

    MockLogisticHierarchyOwner_ABC mainOwner;
    MockMIL_AutomateLOG mainBrainLog1( *(MIL_Automate*)0, PHY_LogisticLevel::logistic_base_ );
    MockMIL_AutomateLOG mainBrainLog2( *(MIL_Automate*)0, PHY_LogisticLevel::logistic_base_ );
    LogisticHierarchy mainLogHierarchy( mainOwner, mainBrainLog1, true );
    MOCK_EXPECT( mainOwner.NotifyLinkAdded ).once().with( boost::bind( &HasSuperior, _1, &mainBrainLog2 ) );
    mainLogHierarchy.ChangeLinks( boost::assign::list_of( &mainBrainLog1 )( &mainBrainLog2 ) );

    testSuperiors( mainLogHierarchy.CreateSuperiorsIterator(), boost::assign::list_of( &mainBrainLog2 )( &mainBrainLog1 ) );

    MOCK_EXPECT( mainOwner.NotifyLinkRemoved ).once().with( boost::bind( &HasSuperior, _1, &mainBrainLog1 ) );
    MOCK_EXPECT( mainOwner.NotifyLinkRemoved ).once().with( boost::bind( &HasSuperior, _1, &mainBrainLog2 ) );
    MOCK_EXPECT( mainOwner.NotifyLinkAdded ).once().with( boost::bind( &HasSuperior, _1, &objectBrainLog1 ) );
    mainLogHierarchy.SwitchToHierarchy( objectLogHierarchy );
    testSuperiors( mainLogHierarchy.CreateSuperiorsIterator(), boost::assign::list_of( &objectBrainLog1 ) );

    MOCK_EXPECT( mainOwner.NotifyLinkRemoved ).once().with( boost::bind( &HasSuperior, _1, &objectBrainLog1 ) );
    MOCK_EXPECT( mainOwner.NotifyLinkAdded ).once().with( boost::bind( &HasSuperior, _1, &mainBrainLog1 ) );
    MOCK_EXPECT( mainOwner.NotifyLinkAdded ).once().with( boost::bind( &HasSuperior, _1, &mainBrainLog2 ) );
    mainLogHierarchy.SwitchBackToNominalHierarchy();
    testSuperiors( mainLogHierarchy.CreateSuperiorsIterator(), boost::assign::list_of( &mainBrainLog2 )( &mainBrainLog1 ) );
}

BOOST_AUTO_TEST_SUITE_END()
