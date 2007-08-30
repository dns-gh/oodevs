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
#include "dispatcher/SimulationPublisher_ABC.h"
#include "dispatcher/Network_Def.h"

using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: StatusListener constructor
// Created: JCR 2007-06-13
// -----------------------------------------------------------------------------
StatusListener::StatusListener( Connector& connector, dispatcher::SimulationPublisher_ABC& publisher )
    : publisher_( publisher )
    , paused_( false )
{
    // $$$$ SBO 2007-08-29: Create a wrapper for Properties access
    try
    {
        ITablePtr   spTable;
        ICursorPtr  spCursor;

        spTable = connector.GetTable( "SimulationProperties" );
        spTable->FindField( CComBSTR( L"PropertyValue" ), &lFieldValue_ );
        
        IQueryFilterPtr spQueryFilter( CLSID_QueryFilter );
        spQueryFilter->put_WhereClause( CComBSTR( L"Property=Status" ) );
        if( FAILED( spTable->Search( spQueryFilter, false, &spCursor ) ) )
        {
            // $$$$ SBO 2007-08-29: create property if not exist...
            //throw std::runtime_error( "Search failed" ); // $$$$ SBO 2007-05-30: !!!!!
            return;
        }
        // $$$$ SBO 2007-08-29: is keeping IRowPtr really supposed to work ?
        if( spCursor != NULL )
            spCursor->NextRow( &spRow_ );
    }
    catch( std::exception& )
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
void StatusListener::Listen()
{
    if( spRow_ )
    {
        CComVariant value;
        spRow_->get_Value( lFieldValue_, &value );        
        ChangeStatus( GetString( value ) );
    }
}

// -----------------------------------------------------------------------------
// Name: StatusListener::ChangeStatus
// Created: JCR 2007-06-13
// -----------------------------------------------------------------------------
void StatusListener::ChangeStatus( const std::string& status )
{
    if( paused_ && status == "resumed" )
    {
        dispatcher::AsnMsgClientToSimControlResume asn;
        asn.Send( publisher_ );
        paused_ = false;
    }
    else if( !paused_ && status == "paused" )
    {
        dispatcher::AsnMsgClientToSimControlPause asn;
        asn.Send( publisher_ );
        paused_ = true;
    }
}
