// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "orbat_generator_test_pch.h"
#include "orbat_generator/Facade.h"
#include <xeumeuleu/xml.hpp>

using namespace orbat_generator;

namespace
{
    struct Fixture
    {
        Fixture()
            : expected( "<orbat model-version=\"4.4.1\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"schemas/4.3.0/exercise/orbat.xsd\">"
                        "  <parties>"
                        "    <party id='1' name='aa' type='friend'>"
                        "      <objects/>"
                        "      <tactical>"
                        "        <formation id='101' level='ii' name='aa.001'>"
                        "          <automat engaged='true' id='10101' knowledge-group='1000' name='aa.001.001' type='ART - Artillerie Animation LRM'>"
                        "            <unit command-post='true' id='10010101' name='aa.001.001.001' position='31TCM1007287483' type='ART - Section CDT Batterie LRM'/>"
                        "            <unit command-post='false' id='20010101' name='aa.001.001.002' position='31TCM1007287483' type='ART-  Artillerie Animation LRM'/>"
                        "            <unit command-post='false' id='30010101' name='aa.001.001.003' position='31TCM1007287483' type='ART-  Artillerie Animation LRM'/>"
                        "          </automat>"
                        "          <automat engaged='true' id='20101' knowledge-group='1000' name='aa.001.002' type='ART - Batterie MISTRAL'>"
                        "            <unit command-post='true' id='10020101' name='aa.001.002.001' position='31TCM1007287483' type='ART - PC Batterie MISTRAL'/>"
                        "            <unit command-post='false' id='20020101' name='aa.001.002.002' position='31TCM1007287483' type='ART - Section MISTRAL'/>"
                        "            <unit command-post='false' id='30020101' name='aa.001.002.003' position='31TCM1007287483' type='ART - Section MISTRAL'/>"
                        "            <unit command-post='false' id='40020101' name='aa.001.002.004' position='31TCM1007287483' type='ART - Section MISTRAL'/>"
                        "          </automat>"
                        "        </formation>"
                        "        <formation id='201' level='iii' name='aa.002'>"
                        "          <automat engaged='true' id='10201' knowledge-group='1000' name='aa.002.001' type='ART - Section Reco Mortier'>"
                        "            <unit command-post='true' id='10010201' name='aa.002.001.001' position='31TCM1007287483' type='ART - PC Equipe Reco Mortier'/>"
                        "            <unit command-post='false' id='20010201' name='aa.002.001.002' position='31TCM1007287483' type='ART - Equipe Reco Mortier'/>"
                        "            <unit command-post='false' id='30010201' name='aa.002.001.003' position='31TCM1007287483' type='ART - Equipe Reco Mortier'/>"
                        "          </automat>"
                        "        </formation>"
                        "        <formation id='301' level='iii' name='aa.003'>"
                        "          <automat engaged='true' id='10301' knowledge-group='1000' name='aa.003.001' type='ART - Batterie COBRA'>"
                        "            <unit command-post='true' id='10010301' name='aa.003.001.001' position='31TCM1007287483' type='ART - PC Batterie COBRA'/>"
                        "            <unit command-post='false' id='20010301' name='aa.003.001.002' position='31TCM1007287483' type='ART - Station COBRA'/>"
                        "            <unit command-post='false' id='30010301' name='aa.003.001.003' position='31TCM1007287483' type='ART - Station COBRA'/>"
                        "          </automat>"
                        "        </formation>"
                        "      </tactical>"
                        "      <communication>"
                        "        <knowledge-group id='1000' name='Groupe de connaissance' type='Standard'/>"
                        "      </communication>"
                        "      <populations/>"
                        "      <inhabitants/>"
                        "      <logistics/>"
                        "    </party>"
                        "    <party id='2' name='ab' type='enemy'>"
                        "      <objects/>"
                        "      <tactical>"
                        "        <formation id='102' level='ii' name='ab.001'>"
                        "          <automat engaged='true' id='10102' knowledge-group='2000' name='ab.001.001' type='ART - Batterie Reco COBRA'>"
                        "            <unit command-post='true' id='10010102' name='ab.001.001.001' position='31TCM1007287483' type='ART - PC Batterie COBRA'/>"
                        "            <unit command-post='false' id='20010102' name='ab.001.001.002' position='31TCM1007287483' type='ART - Equipe Reco COBRA'/>"
                        "            <unit command-post='false' id='30010102' name='ab.001.001.003' position='31TCM1007287483' type='ART - Equipe Reco COBRA'/>"
                        "            <unit command-post='false' id='40010102' name='ab.001.001.004' position='31TCM1007287483' type='ART - Equipe Reco COBRA'/>"
                        "            <unit command-post='false' id='50010102' name='ab.001.001.005' position='31TCM1007287483' type='ART - Equipe Reco COBRA'/>"
                        "          </automat>"
                        "          <automat engaged='true' id='20102' knowledge-group='2000' name='ab.001.002' type='ART - Batterie HAWK'>"
                        "            <unit command-post='true' id='10020102' name='ab.001.002.001' position='31TCM1007287483' type='ART - PC Batterie HAWK'/>"
                        "            <unit command-post='false' id='20020102' name='ab.001.002.002' position='31TCM1007287483' type='ART - Section de Tir HAWK'/>"
                        "            <unit command-post='false' id='30020102' name='ab.001.002.003' position='31TCM1007287483' type='ART - Section de Tir HAWK'/>"
                        "          </automat>"
                        "        </formation>"
                        "        <formation id='202' level='iii' name='ab.002'>"
                        "          <automat engaged='true' id='10202' knowledge-group='2000' name='ab.002.001' type='ART - Batterie CAROL'>"
                        "            <unit command-post='true' id='10010202' name='ab.002.001.001' position='31TCM1007287483' type='ART - PC Batterie SACP CAR'/>"
                        "            <unit command-post='false' id='20010202' name='ab.002.001.002' position='31TCM1007287483' type='ART - Section CAROL'/>"
                        "            <unit command-post='false' id='30010202' name='ab.002.001.003' position='31TCM1007287483' type='ART - Section CAROL'/>"
                        "          </automat>"
                        "        </formation>"
                        "      </tactical>"
                        "      <communication>"
                        "        <knowledge-group id='2000' name='Groupe de connaissance' type='Standard'/>"
                        "      </communication>"
                        "      <populations/>"
                        "      <inhabitants/>"
                        "      <logistics/>"
                        "    </party>"
                        "    <party id='3' name='ac' type='neutral'>"
                        "      <objects/>"
                        "      <tactical/>"
                        "      <communication>"
                        "        <knowledge-group id='3000' name='Groupe de connaissance' type='Standard'/>"
                        "      </communication>"
                        "      <populations>"
                        "        <population attitude='calme' id='10000003' name='ac.01' position='31TCM1007287483' type='Population standard BM'>"
                        "          <composition contaminated='0' dead='0' healthy='1000' wounded='0'/>"
                        "        </population>"
                        "        <population attitude='calme' id='20000003' name='ac.02' position='31TCM1007287483' type='Population standard'>"
                        "          <composition contaminated='0' dead='0' healthy='1000' wounded='0'/>"
                        "        </population>"
                        "      </populations>"
                        "      <inhabitants/>"
                        "      <logistics/>"
                        "    </party>"
                        "  </parties>"
                        "  <diplomacy>"
                        "    <party id='1'>"
                        "      <relationship diplomacy='enemy' party='2'/>"
                        "      <relationship diplomacy='neutral' party='3'/>"
                        "    </party>"
                        "    <party id='2'>"
                        "      <relationship diplomacy='enemy' party='1'/>"
                        "      <relationship diplomacy='neutral' party='3'/>"
                        "    </party>"
                        "    <party id='3'>"
                        "      <relationship diplomacy='neutral' party='1'/>"
                        "      <relationship diplomacy='neutral' party='2'/>"
                        "    </party>"
                        "  </diplomacy>"
                        "</orbat>" )
            , inputXis( "<config type='automats'>"
                        "    <structure pattern='[$P][$F][$A][$U]' point='31TCM1007287483'>"
                        "        <party           pattern='aa'  quantity='2' />"
                        "        <object          pattern='111' quantity='0' />"
                        "        <formation       pattern='111' quantity='2' level='ii' />"
                        "        <formation       pattern='111' quantity='3' level='iii' />"
                        "        <automat         pattern='111' quantity='7' />"
                        "        <unit            pattern='111' quantity='7000' />"
                        "        <crowd           pattern='11' quantity='2' />"
                        "    </structure>"
                        "    <excludes>"
                        "        <exclude name='ART - Artillerie Animation AUF1'/>"
                        "    </excludes>"
                        "</config>"
                      )
            , base( "../../data/tests/orbat_generator_test/base/" )
            {}
            const std::string expected;
            xml::xistringstream inputXis;
            bfs::path base;
    };
}

// -----------------------------------------------------------------------------
// Name: orbat_generator_creates_a_well_formed_orbat_file
// Created: RCD 2011-03-09
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( orbat_generator_creates_a_well_formed_orbat_file, Fixture )
{
    inputXis >> xml::start( "config" );
    Facade facade( inputXis, base );
    {
        xml::xostringstream xos;
        facade.CreateOrbat( xos );
        BOOST_CHECK_XML_EQUAL( expected, xos.str() );
    }
    {
        xml::xostringstream xos;
        facade.CreateOrbat( xos );
        BOOST_CHECK_XML_EQUAL( expected, xos.str() );
    }
}

// -----------------------------------------------------------------------------
// Name: orbat_facade_compute_signature
// Created: RCD 2011-03-18
// -----------------------------------------------------------------------------
BOOST_FIXTURE_TEST_CASE( orbat_facade_compute_signature, Fixture )
{
    inputXis >> xml::start( "config" );
    Facade facade( inputXis, base );
    std::istringstream is( "test" );
    BOOST_CHECK_EQUAL( facade.ComputeSignature( is ), "<!-- signature=\"3632233996\" -->" );
}
