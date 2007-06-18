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
// physical.xml
// *****************************************************************************
static const char* physicalXml =
"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
"<physical>"
"    <Decisionnel>Decisionnel.xml</Decisionnel>"
"    <Volumes>Volumes.xml</Volumes>"
"    <Protections>Protections.xml</Protections>"
"    <DotationNatures>DotationNatures.xml</DotationNatures>"
"    <Objets>Objets.xml</Objets>"
"    <Dotations>Dotations.xml</Dotations>"
"    <Lanceurs>Lanceurs.xml</Lanceurs>"
"    <Armements>Armements.xml</Armements>"
"    <Capteurs>Capteurs.xml</Capteurs>"
"    <Composantes>Composantes.xml</Composantes>"
"    <Pions>Pions.xml</Pions>"
"    <Automates>Automates.xml</Automates>"
"    <NBC>NBC.xml</NBC>"
"    <ClasseIDs>IDClasses.xml</ClasseIDs>"
"    <FacteursHumains>FacteursHumains.xml</FacteursHumains>"
"    <Pannes>Pannes.xml</Pannes>"
"    <GroupesConnaissance>GroupesConnaissance.xml</GroupesConnaissance>"
"    <Sante>Sante.xml</Sante>"
"    <Maintenance>Maintenance.xml</Maintenance>"
"    <Ravitaillement>Ravitaillement.xml</Ravitaillement>"
"    <Communications>Communications.xml</Communications>"
"    <PathFinder>Pathfind.xml</PathFinder>"
"    <HLA>HLA.xml</HLA>"
"    <Populations>Populations.xml</Populations>"
"    <ComptesRendus>Reports.xml</ComptesRendus>"
"    <Modeles>Modeles.xml</Modeles>"
"</physical>";

// *****************************************************************************
// idclasses.xml
// *****************************************************************************
static const char* idClassesXml = 
"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"
"<Classes>"
"    <Classe nom=\"Unite\"                                 id=\"1\" />"
"    <Classe nom=\"UnitFire\"                               id=\"2\" />"
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
