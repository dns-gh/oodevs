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
"<physical version=\"4.2.6\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"schemas/physical/Physical.xsd\">"
"  <decisional file=\"Decisional.xml\"/>"
"  <volumes file=\"Volumes.xml\"/>"
"  <protections file=\"Protections.xml\"/>"
"  <active-protections file=\"ActiveProtections.xml\"/>"
"  <resource-natures file=\"DotationNatures.xml\"/>"
"  <logistic-supply-classes file=\"LogisticSupplyClasses.xml\"/>"
"  <disasters file=\"Disasters.xml\"/>"
"  <objects file=\"Objects.xml\"/>"
"  <resources file=\"Dotations.xml\"/>"
"  <launchers file=\"Launchers.xml\"/>"
"  <weapon-systems file=\"WeaponSystems.xml\"/>"
"  <sensors file=\"Sensors.xml\"/>"
"  <components file=\"Equipments.xml\"/>"
"  <units file=\"Units.xml\"/>"
"  <automats file=\"Automats.xml\"/>"
"  <nbc file=\"NBC.xml\"/>"
"  <fires file=\"Fires.xml\"/>"
"  <medical-treatment file=\"MedicalTreatment.xml\"/>"
"  <health file=\"Health.xml\"/>"
"  <human-factors file=\"HumanFactors.xml\"/>"
"  <breakdowns file=\"Breakdowns.xml\"/>"
"  <knowledge-groups file=\"KnowledgeGroups.xml\"/>"
"  <maintenance file=\"Maintenance.xml\"/>"
"  <supply file=\"Supply.xml\"/>"
"  <funeral file=\"Funeral.xml\"/>"
"  <communications file=\"Communications.xml\"/>"
"  <pathfinder file=\"Pathfind.xml\"/>"
"  <populations file=\"Populations.xml\"/>"
"  <inhabitants file=\"Inhabitants.xml\"/>"
"  <reports file=\"Reports.xml\"/>"
"  <models file=\"Models.xml\"/>"
"  <missions file=\"Orders.xml\"/>"
"  <object-names file=\"ObjectNames.xml\"/>"
"  <urban file=\"Urban.xml\"/>"
"  <resource-networks file=\"ResourceNetworks.xml\"/>"
"  <drawing-templates file=\"DrawingTemplates.xml\"/>"
"  <symbols file=\"symbols.xml\"/>"
"  <scores file=\"scores.xml\"/>"
"  <filters file=\"Filters/Filters.xml\"/>"
"  <urban-templates file=\"UrbanTemplates.xml\"/>"
"  <units-mission-sheets-directory path=\"Mission sheets/units\"/>"
"  <automata-mission-sheets-directory path=\"Mission sheets/automata\"/>"
"  <crowds-mission-sheets-directory path=\"Mission sheets/crowds\"/>"
"  <fragorders-mission-sheets-directory path=\"Mission sheets/fragmentary orders\"/>"
"  <symbols-directory path=\"Symbols\"/>"
"  <mission-sheets-xsl file=\"missionSheets.xslt\"/>"
"  <stages file=\"Stages.xml\"/>"
"  <languages file=\"Languages.xml\"/>"
"  <locales-directory path=\"Locales\"/>"
"</physical>";
