// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-12-14 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Connector_Graph_ABC.cpp $
// $Author: Ape $
// $Modtime: 3/03/05 14:48 $
// $Revision: 3 $
// $Workfile: ADN_Connector_Graph_ABC.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_Connector_Graph_ABC.h"

#include "ADN_Graph.h"
#include "ADN_GraphData.h"
#include "ADN_GraphValue.h"


// -----------------------------------------------------------------------------
// Name: ADN_Connector_Graph_ABC constructor
// Created: APE 2004-12-15
// -----------------------------------------------------------------------------
ADN_Connector_Graph_ABC::ADN_Connector_Graph_ABC( ADN_Graph& graph )
: ADN_Connector_Vector_ABC()
, graph_                 ( graph )
{
    graph_.SetConnector( *this );
}


// -----------------------------------------------------------------------------
// Name: ADN_Connector_Graph_ABC destructor
// Created: APE 2004-12-15
// -----------------------------------------------------------------------------
ADN_Connector_Graph_ABC::~ADN_Connector_Graph_ABC()
{
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
    connect( pTarget, SIGNAL(Cleared(bool)),      this, SLOT(Clear(bool)) );
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
    disconnect( pTarget, SIGNAL(Cleared(bool)),      this, SLOT(Clear(bool)) );

    this->ClearPrivate();
}


// -----------------------------------------------------------------------------
// Name: ADN_Connector_Graph_ABC::AddItemPrivate
// Created: APE 2004-12-15
// -----------------------------------------------------------------------------
bool ADN_Connector_Graph_ABC::AddItemPrivate( void* pObj, bool /*bCreateCommand*/ )
{
    if( pObj == 0 )
        return false;

    this->CreateValue( pObj );
    return true;
}


// -----------------------------------------------------------------------------
// Name: ADN_Connector_Graph_ABC::RemItemPrivate
// Created: APE 2004-12-15
// -----------------------------------------------------------------------------
bool ADN_Connector_Graph_ABC::RemItemPrivate(void* pObj, bool /*bCreateCommand*/ )
{
    if( pObj == 0 )
        return false;

    // Iterate over all the graphDatas.
    ADN_Graph::iterator it( graph_ );
    for( it = graph_.begin(); it != graph_.end(); ++it )
    {
        ADN_GraphData& graphData = (ADN_GraphData&)(*it);
        graphData.DeleteData( pObj );  // The graphData will delete any related graphValue.
    }
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
