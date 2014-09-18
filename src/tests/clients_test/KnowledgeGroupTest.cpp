// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
// LTO
//
// *****************************************************************************

#include "clients_test_pch.h"
#include "preparation/KnowledgeGroup.h"
#include "preparation/KnowledgeGroupCommunications.h"
#include "preparation/KnowledgeGroupsModel.h"
#include "preparation/Model.h"
#include "preparation/StaticModel.h"
#include "preparation/TeamCommunications.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/KnowledgeGroupFactory_ABC.h"
#include "clients_kernel/KnowledgeGroupType.h"
#include "clients_kernel/Team_ABC.h"
#include "tools/IdManager.h"
#include "XmlChecks.h"
#include <xeumeuleu/xml.hpp>

namespace
{
    MOCK_BASE_CLASS( MockTeam, kernel::Team_ABC )
    {
        MOCK_METHOD( GetName, 0 );
        MOCK_METHOD( GetId, 0 );
    };

    MOCK_BASE_CLASS( MockKnowledgeGroup, kernel::KnowledgeGroup_ABC)
    {
        MOCK_METHOD( GetName, 0 );
        MOCK_METHOD( GetId, 0 );
        MOCK_METHOD( GetTypeName, 0 );
        MOCK_METHOD( Select, 1 );
        MOCK_METHOD( ContextMenu, 3 );
        MOCK_METHOD( Activate, 1 );
        MOCK_METHOD( IsActivated, 0 );
        MOCK_METHOD( IsCrowd, 0 );
    };

    template< typename T, typename Identifier >
    struct MockResolver : tools::Resolver_ABC< T, Identifier >
    {
        MOCK_METHOD_TPL( Find, 1, T*( const Identifier& id ) )
        MOCK_METHOD_TPL( Get, 1, T&( const Identifier& id ) )
        MOCK_METHOD_TPL( CreateIterator, 0, tools::Iterator< const T& >() )
    };

    MOCK_BASE_CLASS( MockKnowledgeGroupsModel, KnowledgeGroupsModel )
    {
        explicit MockKnowledgeGroupsModel( kernel::Controllers& controllers, kernel::KnowledgeGroupFactory_ABC& knowledgeGroupFactory )
            : KnowledgeGroupsModel( controllers, knowledgeGroupFactory )
        {}

        MOCK_METHOD( Create, 1, void( kernel::Team_ABC& ), Create1 );
        MOCK_METHOD( Create, 3, void( xml::xistream&, kernel::Team_ABC&, Model& ), Create3 );
        MOCK_METHOD( CreateSubKnowledgeGroup, 1, void ( kernel::Team_ABC& ), Create21 );
        MOCK_METHOD( CreateSubKnowledgeGroup, 3, void ( xml::xistream& , kernel::KnowledgeGroup_ABC& , Model& ), Create23 );
    };

    MOCK_BASE_CLASS( MockKnowledgeGroupFactory, kernel::KnowledgeGroupFactory_ABC)
    {
        MOCK_METHOD( Create, 1, kernel::KnowledgeGroup_ABC*( kernel::Team_ABC& ), Create1 );
        MOCK_METHOD( Create, 2, kernel::KnowledgeGroup_ABC*( xml::xistream& , kernel::Team_ABC& ), Create2 );
        MOCK_METHOD( Create, 1, kernel::KnowledgeGroup_ABC*( kernel::KnowledgeGroup_ABC& ), Create3 );
        MOCK_METHOD( Create, 2, kernel::KnowledgeGroup_ABC*( xml::xistream&, kernel::KnowledgeGroup_ABC& ), Create4 );
    };

    MOCK_BASE_CLASS( MockKnowledgeGroupTypeResolver, tools::Resolver< kernel::KnowledgeGroupType, std::string > )
    {
        MOCK_METHOD( Find, 1 );
        MOCK_METHOD( Get, 1 );
    };
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupTypeShowCommunicationDelayHoursMinutesSeconds
// Created: LDC 2009-11-30
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( KnowledgeGroupTypeShowCommunicationDelayHoursMinutesSeconds )
{
    xml::xistringstream xis( "<stuff name =\"group\" communication-delay=\"5465s\" />" );
    xis >> xml::start( "stuff" );
    kernel::KnowledgeGroupType type( xis );
    BOOST_CHECK_EQUAL( "1h31m5s", type.ShowCommunicationDelay() );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupTypeShowCommunicationDelayMinutesSeconds
// Created: LDC 2009-11-30
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( KnowledgeGroupTypeShowCommunicationDelayMinutesSeconds )
{
    xml::xistringstream xis( "<stuff name =\"group\" communication-delay=\"465s\" />" );
    xis >> xml::start( "stuff" );
    kernel::KnowledgeGroupType type( xis );
    BOOST_CHECK_EQUAL( "7m45s", type.ShowCommunicationDelay() );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupTypeShowCommunicationDelaySeconds
// Created: LDC 2009-11-30
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( KnowledgeGroupTypeShowCommunicationDelaySeconds )
{
    xml::xistringstream xis( "<stuff name =\"group\" communication-delay=\"55s\" />" );
    xis >> xml::start( "stuff" );
    kernel::KnowledgeGroupType type( xis );
    BOOST_CHECK_EQUAL( "55s", type.ShowCommunicationDelay() );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupTypeShowCommunicationDelayMinutes
// Created: LDC 2009-11-30
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( KnowledgeGroupTypeShowCommunicationDelayMinutes )
{
    xml::xistringstream xis( "<stuff name =\"group\" communication-delay=\"60s\" />" );
    xis >> xml::start( "stuff" );
    kernel::KnowledgeGroupType type( xis );
    BOOST_CHECK_EQUAL( "1m0s", type.ShowCommunicationDelay() );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupTest
// Created:  FHD 2009-11-30:
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ReadKnowledgeGroupTest )
{
    int argc = 1;
    char* argv = "";
    QApplication app(argc, &argv);

    assert(qApp);
    xml::xistringstream xis( "<knowledge-group id=\"42\" name =\"group1\"/>" );
    xis >> xml::start( "knowledge-group" );
    kernel::Controller controller;
    MockKnowledgeGroupFactory factory;
    kernel::Controllers controllers;
    KnowledgeGroupsModel kgModel( controllers, factory );

    StaticModel staticModel( controllers );
    Model model( controllers, staticModel );
    MockTeam team;
    MockKnowledgeGroup group;
    MOCK_EXPECT( factory.Create2 ).with( mock::same( xis ), mock::same( team ) ).returns( &group );
    MOCK_EXPECT( group.GetId ).returns( 42 );
    kgModel.Create( xis, team, model );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupComposite
// Created:  FHD 2009-11-30:
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ReadKnowledgeGroupCompositeTest )
{
    int argc = 1;
    char* argv = "";
    QApplication app(argc, &argv);

    assert(qApp);
    xml::xistringstream xis( "<knowledge-group id=\"42\" name =\"group1\"><knowledge-group id=\"2\" name =\"group2\"/></knowledge-group>" );
    xis >> xml::start( "knowledge-group" );
    kernel::Controller controller;
    MockKnowledgeGroupFactory factory;
    kernel::Controllers controllers;
    KnowledgeGroupsModel kgModel( controllers, factory );

    StaticModel staticModel( controllers );
    Model model( controllers, staticModel );
    MockTeam team;
    MockKnowledgeGroup group;
    MOCK_EXPECT( factory.Create2 ).with( mock::same( xis ), mock::same( team ) ).returns( &group );
    MOCK_EXPECT( group.GetId ).returns( 42 );
    MockKnowledgeGroup subGroup;
    MOCK_EXPECT( factory.Create4 ).with( mock::any, mock::same( group ) ).returns( &subGroup );
    MOCK_EXPECT( subGroup.GetId ).returns( 2 );

    kgModel.Create( xis, team, model );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupComposite
// Created:  FHD 2009-11-30:
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ReadKnowledgeGroupMultiTest )
{
    int argc = 1;
    char* argv = "";
    QApplication app(argc, &argv);

    assert(qApp);
    xml::xistringstream xis( "<knowledge-group id=\"42\" name =\"group1\"><knowledge-group id=\"2\" name =\"group2\"><knowledge-group id=\"71\" name =\"group3\"/></knowledge-group></knowledge-group>" );
    xis >> xml::start( "knowledge-group" );
    kernel::Controller controller;
    MockKnowledgeGroupFactory factory;
    kernel::Controllers controllers;
    KnowledgeGroupsModel kgModel( controllers, factory );

    StaticModel staticModel( controllers );
    Model model( controllers, staticModel );
    MockTeam team;
    MockKnowledgeGroup group;
    MOCK_EXPECT( factory.Create2 ).with( mock::same( xis ), mock::same( team ) ).returns( &group );
    MOCK_EXPECT( group.GetId ).returns( 42 );
    MockKnowledgeGroup subGroup;
    MOCK_EXPECT( factory.Create4 ).with( mock::any, mock::same( group ) ).returns( &subGroup );
    MOCK_EXPECT( subGroup.GetId ).returns( 2 );
    MockKnowledgeGroup subSubGroup;
    MOCK_EXPECT( factory.Create4 ).with( mock::any, mock::same( subGroup ) ).returns( &subSubGroup );
    MOCK_EXPECT( subSubGroup.GetId ).returns( 71 );

    kgModel.Create( xis, team, model );
}

namespace
{
    kernel::KnowledgeGroupType* MakeKnowledgeGroupType()
    {
        xml::xistringstream xis( "<root name='Standard' communication-delay='1s'/>" );
        xis >> xml::start( "root" );
        return new kernel::KnowledgeGroupType( xis );
    }
}

BOOST_AUTO_TEST_CASE( WriteKnowledgeGroupTest )
{
    int argc = 1;
    char* argv = "";
    QApplication app( argc, &argv );
    tools::IdManager idManager;
    MockKnowledgeGroupTypeResolver types;
    std::unique_ptr< kernel::KnowledgeGroupType > standardType( MakeKnowledgeGroupType() );
    MOCK_EXPECT( types.Find ).with( mock::any ).returns( standardType.get() );
    kernel::Controller controller;
    MockTeam team;
    {
        const std::string expected = "<knowledge-group id='1' name='test' type='Standard'/>";
        xml::xistringstream xis( expected );
        xis >> xml::start( "knowledge-group" );
        KnowledgeGroup kg( xis, controller, idManager, types );
        kg.Attach< kernel::CommunicationHierarchies >( *new KnowledgeGroupCommunications( controller, kg, &team ) );
        xml::xostringstream xos;
        xos << xml::start( "knowledge-group" );
            kg.SerializeAttributes( xos );
        xos << xml::end;
        BOOST_CHECK_XML_EQUAL( expected, xos.str() );
    }
}

//BOOST_AUTO_TEST_CASE( WriteKnowledgeGroupCompositeTest )
//{
//    int argc = 1;
//    char* argv = "";
//    QApplication app( argc, &argv );
//    IdManager idManager;
//    MockKnowledgeGroupTypeResolver types;
//    std::unique_ptr< kernel::KnowledgeGroupType > standardType( MakeKnowledgeGroupType() );
//    MOCK_EXPECT( types.Get ).with( mock::any ).returns( boost::ref( *standardType ) );
//    kernel::Controller controller;
//    MockTeam team;
//    {
//        // create Knowledge group
//        const std::string expected_kg = "<knowledge-group id='1' name='test' type='Standard'/>";
//        xml::xistringstream xis( expected_kg );
//        xis >> xml::start( "knowledge-group" );
//        KnowledgeGroup kg( xis, controller, idManager, types );
//        KnowledgeGroupCommunications *kgCom = new KnowledgeGroupCommunications( controller, kg, &team );
//        kg.Attach< kernel::CommunicationHierarchies >( *kgCom );
//
//        const std::string expected = "<communication><knowledge-group id='1' name='test' type='Standard'/></communication>";
//        TeamCommunications* tcom = new TeamCommunications( controller, team, NULL );
//        team.Attach< kernel::CommunicationHierarchies >( *tcom );
//        tcom->AddSubordinate( kg );
//
//        xml::xostringstream xos;
//        team.Interface().Apply( & kernel::Serializable_ABC::SerializeAttributes, xos );
//        std::string str = xos.str();
//        BOOST_CHECK_XML_EQUAL( expected, xos.str() );
//
//        tcom->RemoveSubordinate( kg );
//        //delete kgCom;
//        //delete tcom;
//    }
//}
