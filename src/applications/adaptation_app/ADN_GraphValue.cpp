// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-12-15 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_GraphValue.cpp $
// $Author: Ape $
// $Modtime: 12/01/05 16:11 $
// $Revision: 2 $
// $Workfile: ADN_GraphValue.cpp $
//
// *****************************************************************************

#include "ADN_pch.h"
#include "ADN_GraphValue.h"
#include "moc_ADN_GraphValue.cpp"

#include "ADN_Types.h"
#include "ADN_GraphData.h"

// -----------------------------------------------------------------------------
// Name: ADN_GraphValue constructor
// Created: APE 2005-01-12
// -----------------------------------------------------------------------------
ADN_GraphValue::ADN_GraphValue( ADN_GraphData& graphData, void* pObject, ADN_Type_Int& x, ADN_Type_Int& y )
: QObject           ()
, graphData_        ( graphData )
, pRelatedObject_   ( pObject )
, nTypes_           ( (E_XYTypes)(eXInt | eYInt) )
{
    nX_.Connect( &x );
    nY_.Connect( &y );
    this->ConstructorHelper();
}

// -----------------------------------------------------------------------------
// Name: ADN_GraphValue constructor
// Created: APE 2005-01-12
// -----------------------------------------------------------------------------
ADN_GraphValue::ADN_GraphValue( ADN_GraphData& graphData, void* pObject, ADN_Type_Double& x, ADN_Type_Int& y )
: QObject           ()
, graphData_        ( graphData )
, pRelatedObject_   ( pObject )
, nTypes_           ( (E_XYTypes)(eXDouble | eYInt) )
{
    rX_.Connect( &x );
    nY_.Connect( &y );
    this->ConstructorHelper();
}


// -----------------------------------------------------------------------------
// Name: ADN_GraphValue constructor
// Created: APE 2005-01-12
// -----------------------------------------------------------------------------
ADN_GraphValue::ADN_GraphValue( ADN_GraphData& graphData, void* pObject, ADN_Type_Double& x, ADN_Type_Double& y )
: QObject           ()
, graphData_        ( graphData )
, pRelatedObject_   ( pObject )
, nTypes_           ( (E_XYTypes)(eXDouble | eYDouble) )
{
    rX_.Connect( &x );
    rY_.Connect( &y );
    this->ConstructorHelper();
}


// -----------------------------------------------------------------------------
// Name: ADN_GraphValue constructor
// Created: APE 2005-01-12
// -----------------------------------------------------------------------------
ADN_GraphValue::ADN_GraphValue( ADN_GraphData& graphData, void* pObject, ADN_Type_Int& x, ADN_Type_Double& y )
: QObject           ()
, graphData_        ( graphData )
, pRelatedObject_   ( pObject )
, nTypes_           ( (E_XYTypes)(eXInt | eYDouble) )
{
    nX_.Connect( &x );
    rY_.Connect( &y );
    this->ConstructorHelper();
}


// -----------------------------------------------------------------------------
// Name: ADN_GraphValue constructor
// Created: APE 2005-01-12
// -----------------------------------------------------------------------------
ADN_GraphValue::ADN_GraphValue( ADN_GraphData& graphData, void* pObject, ADN_Type_Int& y )
: QObject           ()
, graphData_        ( graphData )
, pRelatedObject_   ( pObject )
, nTypes_           ( (E_XYTypes)(eNoX | eYInt) )
{
    nY_.Connect( &y );
    this->ConstructorHelper();
}


// -----------------------------------------------------------------------------
// Name: ADN_GraphValue constructor
// Created: APE 2005-01-12
// -----------------------------------------------------------------------------
ADN_GraphValue::ADN_GraphValue( ADN_GraphData& graphData, void* pObject, ADN_Type_Double& y )
: QObject           ()
, graphData_        ( graphData )
, pRelatedObject_   ( pObject )
, nTypes_           ( (E_XYTypes)(eNoX | eYDouble) )
{
    rY_.Connect( &y );
    this->ConstructorHelper();
}


// -----------------------------------------------------------------------------
// Name: ADN_GraphValue destructor
// Created: APE 2004-12-15
// -----------------------------------------------------------------------------
ADN_GraphValue::~ADN_GraphValue()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_GraphValue::ConstructorHelper
// Created: APE 2005-01-12
// -----------------------------------------------------------------------------
void ADN_GraphValue::ConstructorHelper()
{
    graphData_.AddData( *this );
    connect( &nX_, SIGNAL( DataChanged( void*) ), this, SLOT( OnDataChanged() ) );
    connect( &nY_, SIGNAL( DataChanged( void*) ), this, SLOT( OnDataChanged() ) );
    connect( &rX_, SIGNAL( DataChanged( void*) ), this, SLOT( OnDataChanged() ) );
    connect( &rY_, SIGNAL( DataChanged( void*) ), this, SLOT( OnDataChanged() ) );
}


// -----------------------------------------------------------------------------
// Name: ADN_GraphValue::OnDataChanged
// Created: APE 2005-01-12
// -----------------------------------------------------------------------------
void ADN_GraphValue::OnDataChanged()
{
    graphData_.OnDataChanged( *this );
}


// -----------------------------------------------------------------------------
// Name: ADN_GraphValue::GetPoint
// Created: APE 2004-12-20
// -----------------------------------------------------------------------------
ADN_GraphValue::T_Point ADN_GraphValue::GetPoint()
{
    T_Point point;

    if( nTypes_ & eNoX )
        point.first = graphData_.GetDataIndex( *this );
    else if( nTypes_ & eXInt )
        point.first = nX_.GetData();
    else if( nTypes_ & eXDouble )
        point.first = rX_.GetData();
    
    if( nTypes_ & eYInt )
        point.second = nY_.GetData();
    else if( nTypes_ & eYDouble )
        point.second = rY_.GetData();

    return point;
}
