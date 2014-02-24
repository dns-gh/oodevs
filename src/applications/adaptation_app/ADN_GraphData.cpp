// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_GraphData.h"
#include "ADN_GraphValue.h"

// -----------------------------------------------------------------------------
// Name: ADN_GraphData constructor
/** @param  nUserID
*/
// Created: APE 2004-12-21
// -----------------------------------------------------------------------------
ADN_GraphData::ADN_GraphData( uint nUserID, gui::GQ_Plot& plot )
    : gui::GQ_PlotData( nUserID, plot )
    , pConnector_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_GraphData destructor
// Created: APE 2004-12-21
// -----------------------------------------------------------------------------
ADN_GraphData::~ADN_GraphData()
{
    clear_owned_ptrs( graphValueList_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_GraphData::AddData
// Created: APE 2004-12-21
// -----------------------------------------------------------------------------
void ADN_GraphData::AddData( ADN_GraphValue& value )
{
    graphValueList_.push_back( &value );
    this->AddPoint( value.GetPoint() );
    TouchData();
}

// -----------------------------------------------------------------------------
// Name: ADN_GraphData::DeleteData
/** @param  pRelatedObject
*/
// Created: APE 2004-12-21
// -----------------------------------------------------------------------------
void ADN_GraphData::DeleteData( void* pRelatedObject )
{
    for( auto it = graphValueList_.begin(); it != graphValueList_.end(); )
    {
        ADN_GraphValue& value = **it;
        // Check if the value is related to the object being deleted.
        if( value.GetRelatedObject() == pRelatedObject )
        {
            // It is. Delete the graphData point and the value.
            this->DeletePoint( static_cast< unsigned int >( std::distance( graphValueList_.begin(), it ) ) );
            it = graphValueList_.erase( it );
            delete &value;
            TouchData();
            continue;
        }
        // The value is not related to the object being deleted. Update it in case it has changed
        // as a result of having the other value deleted (for example, it's x coordinate
        // changing because the preceding value was deleted thus avoiding some ugly empty space).
        this->ChangePoint( static_cast< unsigned int >( std::distance( graphValueList_.begin(), it ) ), value.GetPoint() );
        ++it;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_GraphData::OnDataChanged
// Created: APE 2004-12-21
// -----------------------------------------------------------------------------
void ADN_GraphData::OnDataChanged( ADN_GraphValue& value )
{
    auto it = std::find( graphValueList_.begin(), graphValueList_.end(), &value );
    assert( it != graphValueList_.end() );
    T_Point newPoint = value.GetPoint();
    this->ChangePoint( static_cast< unsigned int >( std::distance( graphValueList_.begin(), it ) ), newPoint );
    TouchData();
}
