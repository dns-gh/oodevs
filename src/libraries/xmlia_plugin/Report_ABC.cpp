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
Report_ABC::Report_ABC( ReportManager& manager, xml::xistream& xis,  const std::string& type )
: reportManager_( manager )
, type_( type )
{
  std::string sAuthorQname;
  std::string sDestQname;

  xis >> xml::start( "Entete" )
        >> xml::start( "Origine" );
        author_ = new UnitAutomate( xis );
    xis >> xml::end()
        >> xml::start( "DestinatairesPourAction" );
        dest_ = new UnitFormation( xis );
    xis >> xml::end() 
      >> xml::end();
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
}

// -----------------------------------------------------------------------------
// Name: Report_ABC::Serialize
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Report_ABC::Serialize( xml::xostream& xos ) const
{
  xos << xml::start( "MPIA_Message" )
      << xml::start( type_ );

  SerializeHeader( xos );
  SerializeContent( xos );

  xos << xml::end()
      << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Report_ABC::SerializeHeader
// Created: MGD 2009-07-06
// -----------------------------------------------------------------------------
void Report_ABC::SerializeHeader( xml::xostream& xos ) const
{
  std::string time = CurrentTime();

  xos << xml::start( "Entete" )
        << xml::start( "Origine" );
          author_->Serialize( xos );
    xos << xml::end()
        << xml::start( "DestinatairesPourAction" );
          dest_->Serialize( xos );
    xos << xml::end()
        << xml::content( "GDHRapport", time ) 
      << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Report_ABC::GetType
// Created: MGD 2009-07-06
// -----------------------------------------------------------------------------
std::string Report_ABC::GetType() const
{
  return type_;
}