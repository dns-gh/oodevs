// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "StatusListener.h"
#include "Connector.h"
#include "dispatcher/Publisher_ABC.h"
#include "dispatcher/Network_Def.h"

using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: StatusListener constructor
// Created: JCR 2007-06-13
// -----------------------------------------------------------------------------
StatusListener::StatusListener( Connector& connector )
    : status_ ( eResumed )
{    
    ITablePtr   spTable;
    ICursorPtr  spCursor;

    try
    {
        spTable = connector.GetTable( "SimulationProperties" );
        spTable->FindField( CComBSTR( L"PropertyValue" ), &lFieldValue_ );
        
        IQueryFilterPtr spQueryFilter;
        spQueryFilter.CreateInstance( CLSID_QueryFilter );
        spQueryFilter->put_WhereClause( CComBSTR( L"Property=Status" ) );
        if( FAILED( spTable->Search( spQueryFilter, false, &spCursor ) ) )
            throw std::runtime_error( "Search failed" ); // $$$$ SBO 2007-05-30:
        spCursor->NextRow( &spRow_ );
    }
    catch ( std::exception& e )
    {
        spRow_ = (IRowPtr)0;
    }
}
    
// -----------------------------------------------------------------------------
// Name: StatusListener destructor
// Created: JCR 2007-06-13
// -----------------------------------------------------------------------------
StatusListener::~StatusListener()
{
    // NOTHING
}

namespace
{
    std::string GetString( const CComVariant& value )
    {
        switch( value.vt )
        {
        case VT_BSTR:
            return std::string( _bstr_t( value.bstrVal ) ); // $$$$ SBO 2007-05-31: !!
        default:
            return "";
        }
    }
}

// -----------------------------------------------------------------------------
// Name: StatusListener::Listen
// Created: JCR 2007-06-13
// -----------------------------------------------------------------------------
void StatusListener::Listen( dispatcher::Publisher_ABC& publisher )
{
    if ( spRow_ )
    {
        CComVariant value;
        spRow_->get_Value( lFieldValue_, &value );        
        ProcessStatus( publisher, GetString( value ) );
    }
}

// -----------------------------------------------------------------------------
// Name: StatusListener::ProcessStatus
// Created: JCR 2007-06-13
// -----------------------------------------------------------------------------
void StatusListener::ProcessStatus( dispatcher::Publisher_ABC& publisher, const std::string& status )
{
    switch ( status_ )
    {
    case eResumed:
        if ( status == "paused" ) 
            Pause( publisher );
        break;
    case ePaused:
        if ( status == "resumed" )
            Resume( publisher );
        break;
    }
}

// -----------------------------------------------------------------------------
// Name: StatusListener::Pause
// Created: JCR 2007-06-13
// -----------------------------------------------------------------------------
void StatusListener::Pause( dispatcher::Publisher_ABC& publisher )
{
    dispatcher::AsnMsgClientToSimControlPause asn;
    asn.Send( publisher );
    status_ = ePaused;
}
    
// -----------------------------------------------------------------------------
// Name: StatusListener::Resume
// Created: JCR 2007-06-13
// -----------------------------------------------------------------------------
void StatusListener::Resume( dispatcher::Publisher_ABC& publisher )
{
    dispatcher::AsnMsgClientToSimControlResume asn;
    asn.Send( publisher );
    status_ = eResumed;
}
