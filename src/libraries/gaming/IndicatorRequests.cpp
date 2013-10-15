// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "IndicatorRequests.h"
#include "AfterActionModel.h"
#include "IndicatorRequest.h"
#include "clients_kernel/Controller.h"
#include "tools/FileWrapper.h"
#include "tools/Loader_ABC.h"
#include "tools/SchemaWriter.h"
#include <boost/foreach.hpp>

// -----------------------------------------------------------------------------
// Name: IndicatorRequests constructor
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
IndicatorRequests::IndicatorRequests( kernel::Controller& controller, Publisher_ABC& publisher )
    : controller_( controller )
    , publisher_( publisher )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequests destructor
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
IndicatorRequests::~IndicatorRequests()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequests::Purge
// Created: AGE 2007-10-08
// -----------------------------------------------------------------------------
void IndicatorRequests::Purge()
{
    for( auto it = requests_.begin(); it != requests_.end(); ++it )
        delete *it;
    requests_.resize( 0 );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequests::CreateRequest
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
IndicatorRequest& IndicatorRequests::CreateRequest( const IndicatorDefinition_ABC& definition, const QString& name /*= QString()*/ )
{
    requests_.push_back( new IndicatorRequest( controller_, definition, publisher_, name ) );
    controller_.Update( *this );
    return *requests_.back();
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequests::CreateIterator
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
tools::Iterator< const IndicatorRequest& > IndicatorRequests::CreateIterator()
{
    return new tools::SimpleIterator< const IndicatorRequest&, T_Requests >( requests_ );
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequests::Update
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
void IndicatorRequests::Update( const sword::PlotResult& message )
{
    BOOST_FOREACH( T_Requests::value_type& request, requests_ )
        request->Update( message );
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequests::Save
// Created: JSR 2013-10-11
// -----------------------------------------------------------------------------
void IndicatorRequests::Save( const tools::Path& path ) const
{
    tools::SchemaWriter schemaWriter;
    tools::Xofstream xos( path );
    xos << xml::start( "requests" );
    schemaWriter.WriteExerciseSchema( xos, "requests" );
    BOOST_FOREACH( const T_Requests::value_type& request, requests_ )
        request->Save( xos );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequests::Load
// Created: JSR 2013-10-11
// -----------------------------------------------------------------------------
void IndicatorRequests::Load( const tools::Loader_ABC& loader, const tools::Path& path, const AfterActionModel& model )
{
    loader.LoadFile( path, boost::bind( &IndicatorRequests::LoadRequests, this, _1, boost::cref( model ) ) );
}

// -----------------------------------------------------------------------------
// Name: IndicatorRequests::LoadRequests
// Created: JSR 2013-10-11
// -----------------------------------------------------------------------------
void IndicatorRequests::LoadRequests( xml::xistream& xis, const AfterActionModel& model )
{
    xis >> xml::start( "requests" )
        >> xml::list( "request", [ & ]( xml::xistream& xis )
        {
            const IndicatorDefinition_ABC* definition = model.FindDefinition( xis.attribute< std::string >( "definition" ) );
            if( definition )
            {
                requests_.push_back( new IndicatorRequest( xis, controller_, *definition, publisher_ ) );
                controller_.Update( *this );
                requests_.back()->Commit();
            }
        })
        >> xml::end;
}
