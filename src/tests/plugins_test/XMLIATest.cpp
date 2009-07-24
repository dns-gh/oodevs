// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "plugins_test_pch.h"

#include "dispatcher_dll/DispatcherFacade.h"

#include "xmliaPlugin/Point.h"
#include "xmliaPlugin/EtatOperationnel.h"
#include "xmliaPlugin/UniteAgent.h"
#include "xmliaPlugin/RapportManager.h"
#include "xmliaPlugin/SerializationTools.h"

#include <xeumeuleu/xml.h>

using namespace plugins::xmliaExport;


namespace local
{
  DispatcherFacade* dispatcher = 0;
}


void InitializeWorkspace()
{

  char* args[3] = {
    "simulation_kernel_test.exe",
    "--root-dir=../../data",
    "--exercise=test"
  };

  local::dispatcher = new DispatcherFacade( 3, args );    
}

void CleanWorkspace()
{
  delete local::dispatcher;
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE PointRead
// Created: MGD 2009-06-13
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( PointRead )
{
  xml::xistringstream xis( "<mpia:PointGeographique xmlns:mpia='urn:MPIA-schema' id='Loca-5000'>"
                             "<mpia:Latitude>50</mpia:Latitude>"
                             "<mpia:Longitude>50</mpia:Longitude>"
                           "</mpia:PointGeographique>");
  Point p1( xis );
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE PointWrite
// Created: MGD 2009-06-13
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( PointWrite )
{
  xml::xostringstream xos;
  Point p1( 50, 50, 5000 );
  p1.Serialize( xos, "rapport-5000" );
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE EtatOperationnelRead
// Created: MGD 2009-06-13
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( EtatOperationnelRead )
{
  xml::xistringstream xis( "<mpia:EtatOperationnelEntiteOrganisationnelle xmlns:mpia='urn:MPIA-schema' id='Etat-5000'>"
    "<mpia:Comportement>SUSPCT</mpia:Comportement>"
    "<mpia:EstEtat_InstanceObjet>"
    "<mpia:refid>unite-5000</mpia:refid>"
    "</mpia:EstEtat_InstanceObjet>"
    "<mpia:EstRapportePar_Rapport>"
    "<mpia:refid>rapport-5000</mpia:refid>"
    "</mpia:EstRapportePar_Rapport>"
    "<mpia:EtatCarburants>4</mpia:EtatCarburants>"
    "<mpia:EtatEquipements>4</mpia:EtatEquipements>"
    "<mpia:EtatMunitions>4</mpia:EtatMunitions>"
    "<mpia:EtatOperationnel>OPR</mpia:EtatOperationnel>"
    "<mpia:EtatPersonnel>4</mpia:EtatPersonnel>"
    "</mpia:EtatOperationnelEntiteOrganisationnelle>");

  EtatOperationnel e1( xis );
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE UniteAgentRead
// Created: MGD 2009-06-13
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( UniteAgentRead )
{
  xml::xistringstream xis( "<doc xmlns:mpia='urn:MPIA-schema'>"
    "<mpia:Unite id='unite-5000'>"
    "<mpia:Nom>Compagnie</mpia:Nom>"
    "<mpia:EstRapporteePar_Rapport>"
    "<mpia:refid>rapport-5000</mpia:refid>"
    "</mpia:EstRapporteePar_Rapport>"
    "<mpia:APourEtat_EtatOperationnel>"
    "<mpia:refid>Etat-5000</mpia:refid>"
    "</mpia:APourEtat_EtatOperationnel>"
    "<mpia:EstLocalisePar_AssocInstanceObjetLocalisation>"
    "<mpia:refid>asso_loca-5000</mpia:refid>"
    "</mpia:EstLocalisePar_AssocInstanceObjetLocalisation>"
    "</mpia:Unite>"
    "<mpia:EtatOperationnelEntiteOrganisationnelle id='Etat-5000'>"
    "<mpia:Comportement>SUSPCT</mpia:Comportement>"
    "<mpia:EstEtat_InstanceObjet>"
    "<mpia:refid>unite-5000</mpia:refid>"
    "</mpia:EstEtat_InstanceObjet>"
    "<mpia:EstRapportePar_Rapport>"
    "<mpia:refid>rapport-5000</mpia:refid>"
    "</mpia:EstRapportePar_Rapport>"
    "<mpia:EtatCarburants>4</mpia:EtatCarburants>"
    "<mpia:EtatEquipements>4</mpia:EtatEquipements>"
    "<mpia:EtatMunitions>4</mpia:EtatMunitions>"
    "<mpia:EtatOperationnel>OPR</mpia:EtatOperationnel>"
    "<mpia:EtatPersonnel>4</mpia:EtatPersonnel>"
    "</mpia:EtatOperationnelEntiteOrganisationnelle>"
    "<mpia:PointGeographique id='Loca-5000'>"
    "<mpia:Latitude>50</mpia:Latitude>"
    "<mpia:Longitude>50</mpia:Longitude>"
    "</mpia:PointGeographique>"
    "</doc>");
  xis >> xml::start( "doc" );
  xis >> xml::start( "mpia:Unite");

  UniteAgent e1( xis );
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE WriteSitrep
// Created: MGD 2009-06-13
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( WriteSitrep )
{
  InitializeWorkspace();

  

  CleanWorkspace();
}

// -----------------------------------------------------------------------------
// Name: BOOST_AUTO_TEST_CASE ReadSitrep
// Created: MGD 2009-06-13
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( ReadSitrep )
{

}
