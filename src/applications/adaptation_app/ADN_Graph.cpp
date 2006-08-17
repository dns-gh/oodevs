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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Graph.cpp $
// $Author: Ape $
// $Modtime: 12/01/05 15:03 $
// $Revision: 2 $
// $Workfile: ADN_Graph.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_Graph.h"
#include "moc_ADN_Graph.cpp"

#include "ADN_GraphData.h"
#include "ADN_GraphValue.h"
#include "ADN_ListView.h"


// -----------------------------------------------------------------------------
// Name: ADN_Graph constructor
// Created: APE 2004-12-15
// -----------------------------------------------------------------------------
ADN_Graph::ADN_Graph( QWidget* pParent )
: GQ_Plot           ( pParent )
, bSlotsLocked_     ( false )
, pConnector_       ( 0 )
{
    connect( this, SIGNAL( SelectionChanged( const T_DataItemVector& ) ), this, SLOT( OnSelectionChanged( const T_DataItemVector& ) ) );
}


// -----------------------------------------------------------------------------
// Name: ADN_Graph destructor
// Created: APE 2004-12-15
// -----------------------------------------------------------------------------
ADN_Graph::~ADN_Graph()
{
    delete pConnector_;
}


// -----------------------------------------------------------------------------
// Name: ADN_Graph::SetSelectedItem
// Created: APE 2004-12-16
// -----------------------------------------------------------------------------
void ADN_Graph::SetSelectedItem( void* pObject )
{
    if( bSlotsLocked_ )
        return;

    iterator it( *this );
    for( it = this->begin(); it != this->end(); ++it )
    {
        ADN_GraphData& graphData = (ADN_GraphData&)(*it);
        graphData.SelectRelatedData( pObject );
    }
    /*
    for( int n = 0; n < (int)values_.size(); ++n )
    {
        if( values_[n]->GetRelatedObject() == pObject )
        {
            
            this->Update();
            return;
        }
    }
    nSelectedItem_ = -1;
    this->update();
    return;
    */
}

// -----------------------------------------------------------------------------
// Name: ADN_Graph::OnSelectionChanged
// Created: APE 2004-12-20
// -----------------------------------------------------------------------------
void ADN_Graph::OnSelectionChanged( const T_DataItemVector& selection )
{
    if( selection.empty() )
    {
        bSlotsLocked_ = true;
        emit ItemSelected( 0 );
        bSlotsLocked_ = false;
    }
    else
    {
        assert( selection.size() == 1 );
        int nDataIndex = selection.at(0).second;
        ADN_GraphData* pGraphData = (ADN_GraphData*)(selection.at(0).first);
        bSlotsLocked_ = true;    
        emit ItemSelected( pGraphData->GetGraphValue( nDataIndex ).GetRelatedObject() );
        bSlotsLocked_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Graph::Connect
// Created: APE 2004-12-16
// -----------------------------------------------------------------------------
void ADN_Graph::Connect( ADN_ListView& listView )
{
    connect( this,      SIGNAL( ItemSelected( void* ) ), &listView, SLOT( SetCurrentItem( void* ) ) );
    connect( &listView, SIGNAL( ItemSelected( void* ) ),      this, SLOT( SetSelectedItem( void* ) ) );
}
