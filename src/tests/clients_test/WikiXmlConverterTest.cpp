// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "clients_test_pch.h"
#include "clients_gui/WikiXmlConverter.h"

BOOST_AUTO_TEST_CASE( WikiXmlConverter_roundtrip )
{
    struct TestCase
    {
        std::string xml;
        std::string wiki;
    };
    const TestCase tests[] =
    {
        { 
            "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n"
            "<root>\n"
            "  <line>\n"
            "    <text>something </text>\n"
            "    <italic>\n"
            "      <text>italic</text>\n"
            "    </italic>\n"
            "    <bold>\n"
            "      <text> bold</text>\n"
            "    </bold>\n"
            "    <underlined>\n"
            "      <text> underlined</text>\n"
            "    </underlined>\n"
            "    <text> and now a </text>\n"
            "    <link>\n"
            "      <text>link</text>\n"
            "    </link>\n"
            "  </line>\n"
            "</root>\n",
            "something ''italic''\"\" bold\"\"__ underlined__ and now a $$link$$\n"
        },
        {
            "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n"
            "<root>\n"
            "  <ul>\n"
            "    <li>\n"
            "      <line>\n"
            "        <text>titre 1</text>\n"
            "      </line>\n"
            "    </li>\n"
            "    <ul>\n"
            "      <li>\n"
            "        <line>\n"
            "          <text>titre 2</text>\n"
            "        </line>\n"
            "      </li>\n"
            "      <li>\n"
            "        <line>\n"
            "          <text>titre 3</text>\n"
            "        </line>\n"
            "      </li>\n"
            "    </ul>\n"
            "    <li>\n"
            "      <line>\n"
            "        <text>titre 4</text>\n"
            "      </line>\n"
            "    </li>\n"
            "  </ul>\n"
            "</root>\n",
            " * titre 1\n"
            "  * titre 2\n"
            "  * titre 3\n"
            " * titre 4\n\n"
        },
        {
            "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n"
            "<root>\n"
            "  <ul>\n"
            "    <li>\n"
            "      <line>\n"
            "        <text>titre 1</text>\n"
            "      </line>\n"
            "    </li>\n"
            "    <ul>\n"
            "      <li>\n"
            "        <line>\n"
            "          <text>titre 2</text>\n"
            "        </line>\n"
            "      </li>\n"
            "    </ul>\n"
            "  </ul>\n"
            "</root>\n",
            " * titre 1\n"
            "  * titre 2\n\n",
        },
        {
            "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n"
            "<root>\n"
            "  <ul>\n"
            "    <li/>\n"
            "  </ul>\n"
            "</root>\n",
            " * \n\n",
        }
    };

    for( size_t i = 0; i < sizeof( tests )/sizeof*( tests ); ++i )
    {
        xml::xistringstream xis( tests[ i ].xml );
        std::string wiki;
        FromXmlToWiki( xis >> xml::start( "root" ), wiki );
        BOOST_CHECK_EQUAL( tests[ i ].wiki, wiki );

        xml::xostringstream xos;
        xos << xml::start( "root");
        FromWikiToXml( xos, tests[ i ].wiki );
        xos << xml::end;
        BOOST_CHECK_EQUAL( tests[ i ].xml, xos.str() );
    }
}

BOOST_AUTO_TEST_CASE( WikiXmlConverter_to_xml_is_stateless )
{
    // FromWikiToXml must return the input stream at the same level it was
    // before being written too, aka generated XML is a pure subtree.
    xml::xostringstream xos;
    xos << xml::start( "root");
    FromWikiToXml( xos, "  * some list" );
    FromWikiToXml( xos, "some text" );
    xos << xml::end;
    const std::string xml =
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n"
        "<root>\n"
        "  <ul>\n"
        "    <ul>\n"
        "      <li>\n"
        "        <line>\n"
        "          <text>some list</text>\n"
        "        </line>\n"
        "      </li>\n"
        "    </ul>\n"
        "  </ul>\n"
        "  <line>\n"
        "    <text>some text</text>\n"
        "  </line>\n"
        "</root>\n";
    BOOST_CHECK_EQUAL( xml, xos.str() );
}
