// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"

// *****************************************************************************
// terrain.xml
// *****************************************************************************
static const char* terrainXml = 
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
"<Terrain>"
"    <Geoid>geoid.dat</Geoid>"
"    <World>World.xml</World>"
"    <Graph>Pathfind\\Graph.bin</Graph>"
"    <Nodes>Pathfind\\Nodes.bin</Nodes>"
"    <Links>Pathfind\\Links.bin</Links>"
"    <RawVision>Detection\\detection.dat</RawVision>"
"    <Graphics>Graphics</Graphics>"
"</Terrain>";


// *****************************************************************************
// scipio.xml
// *****************************************************************************
static const char* scipioXml =
"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
"<Scipio>"
"  <SIM>"
"    <AutoStart>1</AutoStart>"
"    <TimeStep>10</TimeStep>"
"    <TimeFactor>10</TimeFactor>"
"    <SauvegardesAutomatiques>"
"        <TempsEntreSauvegardes>1000</TempsEntreSauvegardes>"
"        <NombreMax>10</NombreMax>"
"    </SauvegardesAutomatiques>"
"  </SIM>"
"  <Donnees>"
"    <Terrain>Data/Terrain/Terrain.xml</Terrain>"
"    <Decisionnel>Data/Decisionnel.xml</Decisionnel>"
"    <Reseau>Data/Reseau.xml</Reseau>"
"    <Volumes>Data/Volumes.xml</Volumes>"
"    <Protections>Data/Protections.xml</Protections>"
"    <DotationNatures>Data/DotationNatures.xml</DotationNatures>"
"    <Objets>Data/Objets.xml</Objets>"
"    <Dotations>Data/Dotations.xml</Dotations>"
"    <Lanceurs>Data/Lanceurs.xml</Lanceurs>"
"    <Armements>Data/Armements.xml</Armements>"
"    <Capteurs>Data/Capteurs.xml</Capteurs>"
"    <Composantes>Data/Composantes.xml</Composantes>"
"    <Pions>Data/Pions.xml</Pions>"
"    <Automates>Data/Automates.xml</Automates>"
"    <NBC>Data/NBC.xml</NBC>"
"    <Meteo>Data/Meteo.xml</Meteo>"
"    <ClasseIDs>Data/IDClasses.xml</ClasseIDs>"
"    <FacteursHumains>Data/FacteursHumains.xml</FacteursHumains>"
"    <Pannes>Data/Pannes.xml</Pannes>"
"    <GroupesConnaissance>Data/GroupesConnaissance.xml</GroupesConnaissance>"
"    <Sante>Data/Sante.xml</Sante>"
"    <Maintenance>Data/Maintenance.xml</Maintenance>"
"    <Ravitaillement>Data/Ravitaillement.xml</Ravitaillement>"
"    <Communications>Data/Communications.xml</Communications>"
"    <ODB>Data/ODB.xml</ODB>"
"    <Pathfind>Data/Pathfind.xml</Pathfind>"
"    <Missions>Data/Missions.xml</Missions>"
"    <HLA>Data/HLA.xml</HLA>"
"    <Populations>Data/Populations.xml</Populations>"
"  </Donnees>"
"</Scipio>";


// *****************************************************************************
// reseau.xml
// *****************************************************************************
static const char* reseauXml =
"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>"
"<Reseau>"
"    <AgentServer-MOSServer>"
"      <BasePort>10000</BasePort>"
"      <Magic>1</Magic>"
"    </AgentServer-MOSServer>"
"    <AgentServer-Launcher>"
"      <Port>4322</Port>"
"      <Magic>3232</Magic>"
"    </AgentServer-Launcher>"
"	<ThreadReseauActif>true</ThreadReseauActif>"
"</Reseau>";


// *****************************************************************************
// idclasses.xml
// *****************************************************************************
static const char* idClassesXml = 
"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
"<Classes>"
"    <Classe nom=\"Unite\"                                 id=\"1\" />"
"    <Classe nom=\"TirPion\"                               id=\"2\" />"
"    <Classe nom=\"TirPopulation\"                         id=\"227\" />"
"    <Classe nom=\"Lima\"                                  id=\"137\" />"
"    <Classe nom=\"Limite\"                                id=\"138\" />"
"    <Classe nom=\"Ordre\"                                 id=\"145\" />"
"    <Classe nom=\"GhostAgent\"                            id=\"158\" />"
"    <Classe nom=\"EffetsTirs\"                            id=\"188\" />"
"    <Classe nom=\"TraitementLogMaintenance\"              id=\"203\" />"
"    <Classe nom=\"TraitementLogSante\"                    id=\"204\" />"
"    <Classe nom=\"TraitementLogRavitaillement\"           id=\"201\" />"
"    <Classe nom=\"fosse anti char\"                       id=\"147\" />"
"    <Classe nom=\"abattis\"                               id=\"147\" />"
"    <Classe nom=\"barricade\"                             id=\"147\" />"
"    <Classe nom=\"bouchon mines\"                         id=\"147\" />"
"    <Classe nom=\"zone minee lineaire\"                   id=\"147\" />"
"    <Classe nom=\"zone minee par dispersion\"             id=\"147\" />"
"    <Classe nom=\"eboulement\"                            id=\"147\" />"
"    <Classe nom=\"destruction route\"                     id=\"147\" />"
"    <Classe nom=\"destruction pont\"                      id=\"147\" />"
"    <Classe nom=\"pont flottant continu\"                 id=\"168\" />"
"    <Classe nom=\"pont flottant discontinu\"              id=\"168\" />"
"    <Classe nom=\"poste tir\"                             id=\"166\" />"
"    <Classe nom=\"zone protegee\"                         id=\"166\" />"
"    <Classe nom=\"zone implantation canon\"               id=\"167\" />"
"    <Classe nom=\"zone implantation cobra\"               id=\"167\" />"
"    <Classe nom=\"zone implantation lrm\"                 id=\"167\" />"
"    <Classe nom=\"site franchissement\"                   id=\"168\" />"
"    <Classe nom=\"nuage nbc\"                             id=\"190\" />"
"    <Classe nom=\"site decontamination\"                  id=\"213\" />"
"    <Classe nom=\"plot ravitaillement\"                   id=\"189\" />"
"    <Classe nom=\"zone brouillage brod\"                  id=\"212\" />"
"    <Classe nom=\"zone brouillage bromure\"               id=\"212\" />"
"    <Classe nom=\"rota\"                                  id=\"197\" />"
"    <Classe nom=\"zone NBC\"                              id=\"126\" />"
"    <Classe nom=\"aire poser\"                            id=\"210\" />"
"    <Classe nom=\"piste\"                                 id=\"209\" />"
"    <Classe nom=\"plateforme\"                            id=\"210\" />"
"    <Classe nom=\"zone mobilite amelioree\"               id=\"210\" />"
"    <Classe nom=\"zone poser helicoptere\"                id=\"210\" />"
"    <Classe nom=\"aire logistique\"                       id=\"210\" />"
"    <Classe nom=\"camp prisonniers\"                      id=\"207\" />"
"    <Classe nom=\"camp refugies\"                         id=\"207\" />"
"    <Classe nom=\"itineraire logistique\"                 id=\"211\" />"
"    <Classe nom=\"poste controle\"                        id=\"208\" />"
"    <Classe nom=\"terrain largage\"                       id=\"210\" />"
"    <Classe nom=\"zone interdite tir\"                    id=\"215\" />"
"    <Classe nom=\"zone interdite mouvement\"              id=\"215\" />"
"    <Classe nom=\"zone implantation mortier\"             id=\"167\" />"
"    <Classe nom=\"installation\"                          id=\"210\" />"
"    <Classe nom=\"Population\"                            id=\"139\" />"
"    <Classe nom=\"PopulationConcentration\"               id=\"218\" />"
"    <Classe nom=\"PopulationFlux\"                        id=\"220\" />"
"    <Classe nom=\"ConnaissancePopulation\"                id=\"223\" />"
"    <Classe nom=\"ConnaissancePopulationConcentration\"   id=\"222\" />"
"    <Classe nom=\"ConnaissancePopulationFlux\"            id=\"221\" />"
"</Classes>";
