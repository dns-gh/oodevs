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
StatusListener::StatusListener( Connector& connector, dispatcher::Publisher_ABC& publisher )
    : publisher_ ( publisher )
    , status_ ( eResumed )
{
    ITablePtr   spTable;
    ICursorPtr  spCursor;

    spTable = connector.GetTable( "SimulationProperties" );
    spTable->FindField( CComBSTR( L"PropertyValue" ), &lFieldValue_ );
    
    IQueryFilterPtr spQueryFilter;
    spQueryFilter.CreateInstance( CLSID_QueryFilter );
    spQueryFilter->put_WhereClause( CComBSTR( L"Property=Status" ) );    
    if( FAILED( spTable->Search( spQueryFilter, false, &spCursor ) ) )
        throw std::runtime_error( "Search failed" ); // $$$$ SBO 2007-05-30:     
    spCursor->NextRow( &spRow_ );    
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
void StatusListener::Listen()
{
    if ( spRow_ )
    {
        CComVariant value;
        spRow_->get_Value( lFieldValue_, &value );        
        ProcessStatus( GetString( value ) );
    }
}

// -----------------------------------------------------------------------------
// Name: StatusListener::ProcessStatus
// Created: JCR 2007-06-13
// -----------------------------------------------------------------------------
void StatusListener::ProcessStatus( const std::string& status )
{
    switch ( status_ )
    {
    case eResumed:
        if ( status == "paused" ) 
            Pause();
        break;
    case ePaused:
        if ( status == "resumed" )
            Resume();
        break;
    }
}

// -----------------------------------------------------------------------------
// Name: StatusListener::Pause
// Created: JCR 2007-06-13
// -----------------------------------------------------------------------------
void StatusListener::Pause()
{
    dispatcher::AsnMsgClientToSimControlPause asn;
    asn.Send( publisher_ );
    status_ = ePaused;
}
    
// -----------------------------------------------------------------------------
// Name: StatusListener::Resume
// Created: JCR 2007-06-13
// -----------------------------------------------------------------------------
void StatusListener::Resume()
{
    dispatcher::AsnMsgClientToSimControlResume asn;
    asn.Send( publisher_ );
    status_ = eResumed;
}
