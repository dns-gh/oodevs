#include "clients_test_pch.h"

#include "clients_kernel/KnowledgeGroupType.h"
#include <xeumeuleu/xml.h>

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