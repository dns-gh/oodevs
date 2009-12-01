#include "clients_test_pch.h"

#include "clients_kernel/KnowledgeGroupType.h"
#include "preparation/IdManager.h"
#include "preparation/KnowledgeGroup.h"
#include "MockResolver.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/KnowledgeGroupFactory_ABC.h"
#include "preparation/StaticModel.h"
#include "preparation/KnowledgeGroupsModel.h"

#include "preparation/model.h"
#include "preparation/TeamCommunications.h"

#include "clients_kernel/Team_ABC.h"
#include <xeumeuleu/xml.h>

#include "QApplication.h"

namespace
{
    MOCK_BASE_CLASS( MockTeam, kernel::Team_ABC )
    {
        MOCK_METHOD( GetName, 0 );
        MOCK_METHOD( GetId, 0 );
    };

    MOCK_BASE_CLASS( MockKnowledgeGroup_ABC, kernel::KnowledgeGroup_ABC)
    {
        MOCK_METHOD( GetName, 0 );
        MOCK_METHOD( GetId, 0 );
        MOCK_METHOD( GetTypeName, 0 );
        MOCK_METHOD( Select, 1 );
        MOCK_METHOD( ContextMenu, 2 );
        MOCK_METHOD( Activate, 1 );

    };

    MOCK_BASE_CLASS( MockKnowledgeGroupsModel, KnowledgeGroupsModel )
    {
        explicit MockKnowledgeGroupsModel( kernel::Controllers& controllers, kernel::KnowledgeGroupFactory_ABC& knowledgeGroupFactory )
            : KnowledgeGroupsModel( controllers, knowledgeGroupFactory )
        {}

        MOCK_METHOD_EXT( Create, 1, void( kernel::Team_ABC& ), Create1 );
        MOCK_METHOD_EXT( Create, 3, void( xml::xistream&, kernel::Team_ABC&, Model& ), Create3 );
        MOCK_METHOD_EXT( CreateSubKnowledgeGroup, 1, void ( kernel::Team_ABC& ), Create21 );
        MOCK_METHOD_EXT( CreateSubKnowledgeGroup, 3, void ( xml::xistream& , kernel::KnowledgeGroup_ABC& , Model& ), Create23 );
    };

    MOCK_BASE_CLASS( MockKnowledgeGroupFactory, kernel::KnowledgeGroupFactory_ABC)
    {
        MOCK_METHOD_EXT( Create, 1, kernel::KnowledgeGroup_ABC*( kernel::Team_ABC& ), Create1 );
        MOCK_METHOD_EXT( Create, 2, kernel::KnowledgeGroup_ABC*( xml::xistream& , kernel::Team_ABC& ), Create2 );
        MOCK_METHOD_EXT( Create, 1, kernel::KnowledgeGroup_ABC*( kernel::KnowledgeGroup_ABC& ), Create3 );
        MOCK_METHOD_EXT( Create, 2, kernel::KnowledgeGroup_ABC*( xml::xistream&, kernel::KnowledgeGroup_ABC&  ), Create4 );
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
BOOST_AUTO_TEST_CASE( KnowledgeGroupTest )
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
    Model model( controllers, staticModel);
    MockTeam team;
    MockKnowledgeGroup_ABC group;
    MOCK_EXPECT( factory, Create2 ).with( mock::same( xis ), mock::same( team ) ).returns( &group );
    MOCK_EXPECT( group, GetId ).returns( 42 );
    kgModel.Create( xis, team, model );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupComposite
// Created:  FHD 2009-11-30: 
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( KnowledgeGroupComposite )
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
    Model model( controllers, staticModel);
    MockTeam team;
    MockKnowledgeGroup_ABC group;
    MOCK_EXPECT( factory, Create2 ).with( mock::same( xis ), mock::same( team ) ).returns( &group );
    MOCK_EXPECT( group, GetId ).returns( 42 );
    MockKnowledgeGroup_ABC subGroup;
    MOCK_EXPECT( factory, Create4 ).with( mock::any, mock::same( group ) ).returns( &subGroup );
    MOCK_EXPECT( subGroup, GetId ).returns( 2 );

    kgModel.Create( xis, team, model );
}
