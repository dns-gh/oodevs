// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "Rapport.h"
#include "SerializationTools.h"
#include <xeumeuleu/xml.h>

#include "dispatcher/Model.h"
#include "dispatcher/Formation.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Agent.h"

#include "xmlia_plugin/RapportManager.h"
#include "xmlia_plugin/Unite_ABC.h"
#include "xmlia_plugin/UniteAutomat.h"
#include "xmlia_plugin/UniteFormation.h"
#include "xmlia_plugin/UniteAgent.h"

using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: Rapport constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Rapport::Rapport( RapportManager& manager, xml::xistream& xis )
: rapportManager_( manager )
{
  std::string sAuthorQname;

  xis >> xml::start( "mpia:Rapport" )
        >> xml::start( "mpia:EstRedigePar_EntiteOrganisationnelle" )
        >> xml::content( "mpia:refid", sAuthorQname )
        >> xml::end()
      >> xml::end();
  //Hack assert that author and dest are in the same order than the send message
  xis >> xml::start( "mpia:Unite" );
  author_ = new UniteAutomat( xis );
  xis >> xml::end();
  xis >> xml::start( "mpia:Unite" );
  dest_   = new UniteFormation( xis );
  xis >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: Rapport constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Rapport::Rapport( RapportManager& manager, const dispatcher::Automat& author,  const std::string& type  )
: rapportManager_( manager )
, type_( type )
{
  author_ = new UniteAutomat( author );

  if( author.parentFormation_ )
  {
    dest_ = new UniteFormation( *author.parentFormation_);
  }
}

// -----------------------------------------------------------------------------
// Name: Rapport destructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Rapport::~Rapport()
{
  delete author_;
  delete dest_;
}

// -----------------------------------------------------------------------------
// Name: Rapport QName
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
std::string Rapport::QName() const
{
  std::ostringstream os;
  os << author_->GetId();
  std::string sId = os.str();
  return "rapport-" + sId;
}

// -----------------------------------------------------------------------------
// Name: Rapport::Serialize
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Rapport::Serialize( xml::xostream& xos ) const
{
  std::string sQnameRapport = QName();

  std::string time = "TODO";//@TODO

  xos << xml::start( "mpia:MPIA_Message" )
        << xml::attribute( "xmlns:mpia", "urn:MPIA-schema" )
        << xml::attribute( "xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance" )
        << xml::attribute( "xsi:schemaLocation", "urn:MPIA-schema" )
        << xml::start( "mpia:Header" )
          << xml::content( "mpia:VersionSchemaXMLIA", "V3.0-b" )
         << xml::content( "mpia:Name", type_ )
        << xml::end()
        << xml::start( "mpia:Entities" )
          << xml::start( "mpia:Rapport" )
            << xml::attribute( "id", sQnameRapport )
            << xml::content( "mpia:Confirmation", "ASS" )//@TODOFORCE
            << xml::content( "mpia:GDHRapport", time )
            << xml::start( "mpia:EstRedigePar_EntiteOrganisationnelle" )
              << xml::content( "mpia:refid", author_->QName() )
            << xml::end()
          << xml::end();

  author_->Serialize( xos, sQnameRapport );
  dest_->Serialize( xos, sQnameRapport );
  SerializeDestinataires( xos, sQnameRapport );
  SerializeSides( xos, sQnameRapport);

  SerializeOtherEntities( xos );
  
  xos << xml::end()//</mpia:Entities>
    << xml::end();//</mpia:MPIA_Message>
}

// -----------------------------------------------------------------------------
// Name: Rapport::SerializeDestinataires
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Rapport::SerializeDestinataires( xml::xostream& xos, std::string sQnameRapport) const
{

  xos << xml::start( "mpia:AssociationInstanceObjetAdresse" )
        << xml::attribute( "id", "dest-1")//adapt if more than one dest
        << xml::content( "mpia:TransmissionEtReception", "RECEIV")
        << xml::start( "mpia:AssocieCommeObjet_Adresse")
          << xml::content( "mpia:refid", "adresse-0")
        << xml::end()
        << xml::start( "mpia:AssocieCommeSujet_InstanceObjet" )
          << xml::content( "mpia:refid", dest_->QName() )
        << xml::end()
        << xml::start( "mpia:EstRapporteePar_Rapport" )
          << xml::content( "mpia:refid", sQnameRapport )
        << xml::end()
      << xml::end()
      << xml::start( "mpia:AdresseRadio" )
      << xml::attribute( "id", "adresse-0")
      << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Rapport::SerializeSides
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Rapport::SerializeSides( xml::xostream& xos, std::string sQnameRapport ) const
{
  for( kernel::Iterator< const dispatcher::Side& > it = rapportManager_.GetModel().sides_.CreateIterator(); it.HasMoreElements(); )
  {
    SerializeSide( it.NextElement(), xos, sQnameRapport);
  }
}