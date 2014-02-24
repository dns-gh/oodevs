// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Connector_Graph_ABC.h"
#include "clients_gui/GQ_Plot.h"
#include "ADN_GraphData.h"
#include "ADN_GraphValue.h"

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Graph_ABC constructor
// Created: APE 2004-12-15
// -----------------------------------------------------------------------------
ADN_Connector_Graph_ABC::ADN_Connector_Graph_ABC( gui::GQ_Plot& graph )
: ADN_Connector_Vector_ABC()
, graph_( graph )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Graph_ABC destructor
// Created: APE 2004-12-15
// -----------------------------------------------------------------------------
ADN_Connector_Graph_ABC::~ADN_Connector_Graph_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Graph_ABC::ConnectPrivateSub
// Created: APE 2005-03-02
// -----------------------------------------------------------------------------
void ADN_Connector_Graph_ABC::ConnectPrivateSub( ADN_Connector_Vector_ABC* pTarget )
{
    ADN_Connector_ABC::ConnectPrivateSub( (ADN_Connector_ABC*)pTarget );
    pTarget->Initialize( *this );
    connect( pTarget, SIGNAL(ItemAdded(void*)),   this, SLOT(AddItemNoEmit(void*)) );
    connect( pTarget, SIGNAL(ItemRemoved(void*)), this, SLOT(RemItemNoEmit(void*)) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Graph_ABC::DisconnectPrivateSub
// Created: APE 2005-03-02
// -----------------------------------------------------------------------------
void ADN_Connector_Graph_ABC::DisconnectPrivateSub( ADN_Connector_Vector_ABC* pTarget )
{
    ADN_Connector_ABC::DisconnectPrivateSub( (ADN_Connector_ABC*)pTarget );
    disconnect( pTarget, SIGNAL(ItemAdded(void*)),   this, SLOT(AddItemNoEmit(void*)) );
    disconnect( pTarget, SIGNAL(ItemRemoved(void*)), this, SLOT(RemItemNoEmit(void*)) );
    ClearPrivate();
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Graph_ABC::AddItemPrivate
// Created: APE 2004-12-15
// -----------------------------------------------------------------------------
bool ADN_Connector_Graph_ABC::AddItemPrivate( void* pObj )
{
    if( pObj == 0 )
        return false;
    CreateValue( pObj );
    return true;
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Graph_ABC::RemItemPrivate
// Created: APE 2004-12-15
// -----------------------------------------------------------------------------
bool ADN_Connector_Graph_ABC::RemItemPrivate(void* pObj )
{
    if( pObj == 0 )
        return false;

    // Iterate over all the graphDatas.
    gui::GQ_Plot::iterator it( graph_ );
    for( it = graph_.begin(); it != graph_.end(); ++it )
    {
        ADN_GraphData& graphData = (ADN_GraphData&)(*it);
        graphData.DeleteData( pObj );  // The graphData will delete any related graphValue.
    }
    ADN_Workspace::GetWorkspace().SetMainWindowModified( true );
    return true;
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Graph_ABC::ClearPrivate
// Created: APE 2004-12-15
// -----------------------------------------------------------------------------
void ADN_Connector_Graph_ABC::ClearPrivate( bool /*bInConnection*/ )
{
    //$$$$
}

// -----------------------------------------------------------------------------
// Name: ADN_Connector_Graph_ABC::SetDataPrivate
// Created: APE 2004-12-15
// -----------------------------------------------------------------------------
void ADN_Connector_Graph_ABC::SetDataPrivate( void* /*pData*/ )
{
    // Nothing.
}
