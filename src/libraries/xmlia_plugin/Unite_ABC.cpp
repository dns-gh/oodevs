// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "Unite_ABC.h"

#include <xeumeuleu/xml.h>

using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: Unite_ABC constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Unite_ABC::Unite_ABC( unsigned int id, const std::string& name )
: id_( id )
, name_( name )
{}

// -----------------------------------------------------------------------------
// Name: Unite_ABC constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Unite_ABC::Unite_ABC( xml::xistream& xis )
{
  std::string qName;
  xis >> xml::attribute( "id", qName )
      >> xml::content( "mpia:Nom", name_ );
  std::string sId = qName.substr( 6 );
  std::istringstream is( sId );
  is >> id_;
}

// -----------------------------------------------------------------------------
// Name: Unite_ABC destructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Unite_ABC::~Unite_ABC(){}

// -----------------------------------------------------------------------------
// Name: Unite_ABC QName
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
std::string Unite_ABC::QName() const
{
  std::ostringstream os;
  os << id_;
  std::string sId = os.str();
  return "unite-" + sId;
}

// -----------------------------------------------------------------------------
// Name: Unite_ABC QNameToId
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
unsigned int Unite_ABC::QNameToId( std::string sQname )
{
  unsigned int id;
  std::string sId = sQname.substr( 6 );
  std::istringstream is( sId );
  is >> id;
  return id;
}

// -----------------------------------------------------------------------------
// Name: Unite_ABC QName
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
unsigned int Unite_ABC::GetId() const
{
  return id_;
}

// -----------------------------------------------------------------------------
// Name: Unite_ABC Serialize
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Unite_ABC::Serialize( xml::xostream& xos, const std::string& sQnameRapport  ) const
{
  xos << xml::start( "mpia:Unite" )
        << xml::attribute( "id", QName() )
        << xml::content( "mpia:Nom", name_ )
        << xml::start( "mpia:EstRapporteePar_Rapport" )
          << xml::content( "mpia:refid", sQnameRapport )
        << xml::end();
        SerializeExtension( xos, sQnameRapport );
  xos << xml::end();
  SerializeIncludeEntities( xos, sQnameRapport );
}