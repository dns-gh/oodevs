// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "timeline_plugin_pch.h"
#include "Task.h"
#include "ActionPublisher.h"
#include <xeumeuleu/xml.hpp>

using namespace plugins::timeline;

// -----------------------------------------------------------------------------
// Name: Task constructor
// Created: JCR 2010-09-08
// -----------------------------------------------------------------------------
Task::Task( xml::xistream& xis )
    : triggered_ ( false )
{
    xis >> xml::start( "action" );
        xis_.reset( new xml::xibufferstream( xis ) );
    xis >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: Task destructor
// Created: JCR 2010-09-08
// -----------------------------------------------------------------------------
Task::~Task()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Task::Execute
// Created: JCR 2010-09-08
// -----------------------------------------------------------------------------
void Task::Execute( ActionPublisher& publisher )
{
    if( !triggered_ )
        publisher.Execute( *xis_ );
    triggered_ = true;
}
