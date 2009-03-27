// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AfterActionModel.h"
#include "AfterActionFunction.h"
#include "AfterActionRequests.h"
#include "AfterActionIndicator.h"
#include "clients_kernel/Controller.h"
#include <xeumeuleu/xml.h>

#include <boost/bind.hpp>
#include <boost/filesystem.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AfterActionModel constructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AfterActionModel::AfterActionModel( kernel::Controller& controller, Publisher_ABC& publisher )
    : controller_( controller )
    , requests_( new AfterActionRequests( controller, publisher ) )
{
    Load( "functions.xml" ); // $$$$ AGE 2007-10-10: 
}

// -----------------------------------------------------------------------------
// Name: AfterActionModel destructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AfterActionModel::~AfterActionModel()
{
    // NOTHING
	SaveIndicators( "indicators.xml" );
}

// -----------------------------------------------------------------------------
// Name: AfterActionModel::Update
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AfterActionModel::Update( const ASN1T_MsgAarInformation& /*asn*/ )
{
    // $$$$ AGE 2007-10-10: 
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: AfterActionModel::Update
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AfterActionModel::Update( const ASN1T_MsgPlotResult& asn )
{
    requests_->Update( asn );
}

// -----------------------------------------------------------------------------
// Name: AfterActionModel::Update
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
void AfterActionModel::Update( const ASN1T_MsgIndicator& asn )
{
	std::string indicName = std::string( asn.name );
	AfterActionIndicator* indic;

	T_Indicators::const_iterator it = indicators_.find( indicName );
	if( it == indicators_.end() )
	{
		indic = indicators_[ indicName ] = new AfterActionIndicator( indicName, controller_ );
	}
	else
	{
		indic = it->second;
	}
	indic->Update( asn );
}

// -----------------------------------------------------------------------------
// Name: AfterActionModel::CreateRequest
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
AfterActionRequest& AfterActionModel::CreateRequest( const AfterActionFunction& function )
{
    return requests_->CreateRequest( function );
}

// -----------------------------------------------------------------------------
// Name: AfterActionModel::Load
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
void AfterActionModel::Load( const std::string& functions )
{
    xml::xifstream xis( functions );
    xis >> xml::start( "functions" )
            >> xml::list( "function", *this, &AfterActionModel::ReadFunction )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: AfterActionModel::ReadFunction
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
void AfterActionModel::ReadFunction( xml::xistream& xis )
{
    std::auto_ptr< AfterActionFunction > function( new AfterActionFunction( xis ) );
    const QString name = function->GetName();
    Register( name, *function.release() );
}

namespace
{
	void SaveIndicator(xml::xostream& xos, const std::pair< std::string, AfterActionIndicator* > & ptr)
	{
		xos << xml::content( "Nom_long", ptr.second->GetName() )
			<< xml::content( "Valeur", ptr.second->GetValue() );
	}
}

// -----------------------------------------------------------------------------
// Name: AfterActionModel::SaveIndicators
// Created: AGE 2007-09-20
// -----------------------------------------------------------------------------
void AfterActionModel::SaveIndicators( const std::string& fileName ) const
{
	if( !indicators_.empty() )
	{
		xml::xostringstream xos;

		xos << xml::start( "Root" );
		if( boost::filesystem::exists( fileName ) )
		{
			xml::xifstream xis( fileName );
			xis >> xml::start( "Root" );
			xos << xis;
		}
		WriteIndicators( xos );
        xos << xml::end();// Root
        WriteFile( fileName, xos.str() );
	}
}

// -----------------------------------------------------------------------------
// Name: AfterActionModel::WriteIndicators
// Created: JCR 2008-11-04
// -----------------------------------------------------------------------------
void AfterActionModel::WriteIndicators( xml::xostream& xos ) const
{
    xos << xml::start( "Simulation" )
	    << xml::content( "NomSimulation", "??" ) 
		    << xml::start( "ListeDonnees" );
    {
	    std::for_each( indicators_.begin(), indicators_.end(), boost::bind( &SaveIndicator, boost::ref( xos ), _1 ) );
    }
	xos	<< xml::end() // ListeDonnees
	    << xml::end(); // Simulation	
}

// -----------------------------------------------------------------------------
// Name: AfterActionModel::WriteFile
// Created: JCR 2008-11-04
// -----------------------------------------------------------------------------
void AfterActionModel::WriteFile( const std::string& fileName, const std::string& data ) const
{
	std::ofstream os( fileName.c_str() );
	os << data;
}
