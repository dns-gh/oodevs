// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "msdl_test_pch.h"

BOOST_AUTO_TEST_CASE( msdl_importer_transforms_from_msdl_to_orbat )
{
    std::stringstream fichier_msdl_a_importer;
    fichier_msdl_a_importer << "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                            << "<MilitaryScenario xmlns=\"urn:sisostds:scenario:military:data:draft:msdl:1\">"
                            << "  "
                            << "</MilitaryScenario>"
                            << std::endl;

    std::stringstream fichier_orbat_resultat;
    fichier_orbat_resultat << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>"
                           << "<orbat model-version=\"4.8.2\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"schemas/4.3.2/exercise/orbat.xsd\">"
                           << "<parties>"
                           << "</parties>"
                           << "</orbat>"
                           << std::endl;






}