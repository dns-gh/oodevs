#include "clients_kernel_test_pch.h"

#include "clients_kernel/SymbolFactory.h"
#include <xeumeuleu/xml.hpp>

namespace
{
    void Expect( const kernel::SymbolFactory& factory, const std::string& category, const std::string& expectedBareSymbol, const std::string& expectedMoveSymbol, const std::string& expectedStaticSymbol )
    {
        BOOST_CHECK_EQUAL( factory.CreateSymbol( category ), expectedBareSymbol );
        BOOST_CHECK_EQUAL( factory.CreateMoveSymbol( category ), expectedMoveSymbol );
        BOOST_CHECK_EQUAL( factory.CreateStaticSymbol( category ), expectedStaticSymbol );
    }
}

BOOST_AUTO_TEST_CASE( TestSymbolRule )
{
    std::string factoryInput(
            "<app6>"
              "<symbols base='symbols/s*gpu'>"
                "<choice name='Category'>"
                  "<case name='Base' symbol='A'>"
                    "<choice name='Stuff'>"
                      "<case name='ANP' symbol='A' move='-mvt' static='-s'/>"
                      "<case name='Move' symbol='M' move='-mvt'>"
                        "<choice name='Stuff'>"
                          "<case name='A' symbol='A'/>"
                          "<case name='B' symbol='B' move='-mvt'/>"
                          "<case name='C' symbol='C' move='-mvt' static='-s'/>"
                        "</choice>"
                      "</case>"
                      "<case name='Full' symbol='F' move='-mvt' static='-s'>"
                        "<choice name='Stuff'>"
                          "<case name='A' symbol='A'>"
                            "<choice name='Stuff'>"
                              "<case name='D' symbol='D'/>"
                            "</choice>"
                          "</case>"
                          "<case name='B' symbol='B' move='-mvt'/>"
                          "<case name='C' symbol='C' move='-mvt' static='-s'/>"
                        "</choice>"
                      "</case>"
                      "<case name='Other' symbol='O'/>"
                    "</choice>"
                  "</case>"
                "</choice>"
              "</symbols>"
              "<levels base='levels/'/>"
              "<automats base='headquarter'/>"
              "<infrastructures base='infrastructures'/>"
            "</app6>" );
    xml::xistringstream xis( factoryInput );
    kernel::SymbolFactory factory( xis );
    Expect( factory, "Base", "symbols/s*gpuA", "", "" );
    Expect( factory, "Base/Other", "symbols/s*gpuAO", "", "" );
    Expect( factory, "Base/ANP", "symbols/s*gpuAA", "symbols/s*gpuAA-mvt", "symbols/s*gpuAA-s" );
    Expect( factory, "Base/Move", "symbols/s*gpuAM", "symbols/s*gpuAM-mvt", "" );
    Expect( factory, "Base/Move/A", "symbols/s*gpuAMA", "symbols/s*gpuAM-mvt", "" );
    Expect( factory, "Base/Move/B", "symbols/s*gpuAMB", "symbols/s*gpuAMB-mvt", "" );
    Expect( factory, "Base/Move/C", "symbols/s*gpuAMC", "symbols/s*gpuAMC-mvt", "symbols/s*gpuAMC-s" );
    Expect( factory, "Base/Full/A", "symbols/s*gpuAFA", "symbols/s*gpuAF-mvt", "symbols/s*gpuAF-s" );
    Expect( factory, "Base/Full/B", "symbols/s*gpuAFB", "symbols/s*gpuAFB-mvt", "symbols/s*gpuAF-s" );
    Expect( factory, "Base/Full/C", "symbols/s*gpuAFC", "symbols/s*gpuAFC-mvt", "symbols/s*gpuAFC-s" );
    Expect( factory, "Base/Full/A/D", "symbols/s*gpuAFAD", "symbols/s*gpuAF-mvt", "symbols/s*gpuAF-s" );
}
