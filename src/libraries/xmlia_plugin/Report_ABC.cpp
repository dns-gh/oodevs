// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "Report_ABC.h"
#include "SerializationTools.h"
#include <xeumeuleu/xml.h>

#include "dispatcher/Model.h"
#include "dispatcher/Formation.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Agent.h"

#include "xmlia_plugin/ReportManager.h"
#include "xmlia_plugin/Unit_ABC.h"
#include "xmlia_plugin/UnitAutomate.h"
#include "xmlia_plugin/UnitFormation.h"
#include "xmlia_plugin/UnitAgent.h"


#pragma warning( push, 1 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )
#include <iostream>

namespace bpt = boost::posix_time;

using namespace plugins::xmlia;

namespace
{
  std::string CurrentTime()
  {
    return bpt::to_iso_extended_string( bpt::second_clock::local_time() );
  }
}

// -----------------------------------------------------------------------------
// Name: Report_ABC constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Report_ABC::Report_ABC( ReportManager& manager, xml::xistream& xis )
: reportManager_( manager )
{
  std::string sAuthorQname;
  std::string sDestQname;

  xis >> xml::start( "mpia:Rapport" )
        >> xml::start( "mpia:EstRedigePar_EntiteOrganisationnelle" )
          >> xml::content( "mpia:refid", sAuthorQname )
        >> xml::end()
      >> xml::end()
      >> xml::start( "mpia:AssociationInstanceObjetAdresse" )
        >> xml::start( "mpia:AssocieCommeSujet_InstanceObjet" )
          >> xml::content( "mpia:refid", sDestQname )
        >> xml::end()
      >> xml::end(); 

  unsigned int idAuthor = Unit_ABC::QNameToId( sAuthorQname );
  unsigned int idDest = Unit_ABC::QNameToId( sDestQname );

  cpt_ = 0;
  xis >> xml::list( "mpia:Unite", *this, &Report_ABC::ReadUnites );
}

void Report_ABC::ReadUnites( xml::xistream& xis )
{
  //Hack replace by a xmlia data like group to determine type
  if( cpt_ == 0)
  {
    author_ = new UnitAutomate( xis );
    cpt_++;
  }
  else if( cpt_ == 1)
  {
    dest_   = new UnitFormation( xis );
    cpt_++;
  }
  else
  {
    UnitAgent* unit = new UnitAgent( xis );
    unites_.insert( std::pair< unsigned, UnitAgent* >( unit->GetId(), unit ) );
  }
}

// -----------------------------------------------------------------------------
// Name: Report_ABC constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Report_ABC::Report_ABC( ReportManager& manager, const dispatcher::Automat& author,  const std::string& type  )
: reportManager_( manager )
, type_( type )
{
  author_ = new UnitAutomate( author );

  if( author.parentFormation_ )
  {
    dest_ = new UnitFormation( *author.parentFormation_);
  }
}

// -----------------------------------------------------------------------------
// Name: Report_ABC destructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Report_ABC::~Report_ABC()
{
  delete author_;
  delete dest_;

  for( std::map< unsigned, UnitAgent* >::iterator it = unites_.begin(); it != unites_.end(); it++ )
  {
    delete it->second;
  }
  unites_.clear();
}

// -----------------------------------------------------------------------------
// Name: Report_ABC QName
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
std::string Report_ABC::QName() const
{
  std::ostringstream os;
  os << author_->GetId();
  std::string sId = os.str();
  return "rapport-" + sId;
}

// -----------------------------------------------------------------------------
// Name: Report_ABC::Serialize
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Report_ABC::Serialize( xml::xostream& xos ) const
{
  std::string sQnameRapport = QName();

  std::string time = CurrentTime();

  xos   << xml::start( "mpia:MPIA_Message" )
        << xml::attribute( "xmlns:mpia", "urn:MPIA-schema" )
        //<< xml::attribute( "xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance" )
        //<< xml::attribute( "xsi:schemaLocation", "urn:MPIA-schema" )
        << xml::start( "mpia:Header" )
        << xml::content( "mpia:VersionSchemaXMLIA", "V3.0-b" )
        << xml::content( "mpia:Name", type_ )
        << xml::end()
        << xml::start( "mpia:Entities" )
        << xml::start( "mpia:Rapport" )
        << xml::attribute( "id", sQnameRapport )
        << xml::content( "mpia:Confirmation", "REP" )//@TODOFORCE
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
// Name: Report_ABC::SerializeDestinataires
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Report_ABC::SerializeDestinataires( xml::xostream& xos, std::string sQnameRapport) const
{

  xos   << xml::start( "mpia:AssociationInstanceObjetAdresse" )
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
// Name: Report_ABC::SerializeSides
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Report_ABC::SerializeSides( xml::xostream& xos, std::string sQnameRapport ) const
{
  for( kernel::Iterator< const dispatcher::Side& > it = reportManager_.GetModel().sides_.CreateIterator(); it.HasMoreElements(); )
  {
    SerializeSide( it.NextElement(), xos, sQnameRapport);
  }
}

unsigned int Report_ABC::GetAuthorID() const
{
  return author_->GetId();
}