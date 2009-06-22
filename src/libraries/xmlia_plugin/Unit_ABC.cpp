// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "Unit_ABC.h"

#include <xeumeuleu/xml.h>

using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: Unit_ABC constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Unit_ABC::Unit_ABC( unsigned int id, const std::string& name )
: id_( id )
, name_( name )
{}

// -----------------------------------------------------------------------------
// Name: Unit_ABC constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Unit_ABC::Unit_ABC( xml::xistream& xis )
{
  std::string qName;
  xis >> xml::attribute( "id", qName )
      >> xml::content( "mpia:Nom", name_ );
  std::string sId = qName.substr( 6 );
  std::istringstream is( sId );
  is >> id_;
}

// -----------------------------------------------------------------------------
// Name: Unit_ABC destructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Unit_ABC::~Unit_ABC(){}

// -----------------------------------------------------------------------------
// Name: Unite_ABC QName
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
std::string Unit_ABC::QName() const
{
  std::ostringstream os;
  os << id_;
  std::string sId = os.str();
  return "unite-" + sId;
}

// -----------------------------------------------------------------------------
// Name: Unit_ABC QNameToId
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
unsigned int Unit_ABC::QNameToId( std::string sQname )
{
  unsigned int id;
  std::string sId = sQname.substr( 6 );
  std::istringstream is( sId );
  is >> id;
  return id;
}

// -----------------------------------------------------------------------------
// Name: Unit_ABC QName
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
unsigned int Unit_ABC::GetId() const
{
  return id_;
}

// -----------------------------------------------------------------------------
// Name: Unit_ABC QName
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
std::string Unit_ABC::GetName()
{
  return name_;
}

// -----------------------------------------------------------------------------
// Name: Unit_ABC Serialize
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Unit_ABC::Serialize( xml::xostream& xos, const std::string& sQnameRapport  ) const
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