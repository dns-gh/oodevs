// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_GraphData_h_
#define __ADN_GraphData_h_

#include "clients_gui/GQ_PlotData.h"
#include <boost/noncopyable.hpp>

class ADN_GraphValue;
class ADN_Connector_ABC;

// =============================================================================
/** @class  ADN_GraphData
    @brief  ADN_GraphData
    @par    Using example
    @code
    ADN_GraphData;
    @endcode
*/
// Created: APE 2004-12-21
// =============================================================================
class ADN_GraphData : public gui::GQ_PlotData
                    , private boost::noncopyable
{

public:
    typedef std::vector< ADN_GraphValue* > T_GraphValue_Vector;

public:
    //! @name Constructors/Destructor
    //@{
             ADN_GraphData( uint nUserID, gui::GQ_Plot& );
    virtual ~ADN_GraphData();
    //@}

    //! @name Operations
    //@{
    void AddData( ADN_GraphValue& );
    void DeleteData( void* pRelatedObject );

    void OnDataChanged( ADN_GraphValue& );

    int GetDataIndex( const ADN_GraphValue& ) const;

    void SetConnector( ADN_Connector_ABC& connector );
    //@}

    //! @name Accessors
    //@{
    ADN_GraphValue& GetGraphValue( uint nIndex );
    //@}

private:
    //! @name Member data
    //@{
    T_GraphValue_Vector graphValueList_;
    ADN_Connector_ABC* pConnector_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ADN_GraphData::SetConnector
// Created: APE 2005-01-13
// -----------------------------------------------------------------------------
inline
void ADN_GraphData::SetConnector( ADN_Connector_ABC& connector )
{
    pConnector_ = &connector;
}

// -----------------------------------------------------------------------------
// Name: ADN_GraphData::GetGraphValue
// Created: APE 2004-12-22
// -----------------------------------------------------------------------------
inline
ADN_GraphValue& ADN_GraphData::GetGraphValue( uint nIndex )
{
    assert( nIndex < graphValueList_.size() );
    return * graphValueList_[nIndex];
}

// -----------------------------------------------------------------------------
// Name: ADN_GraphData::GetDataIndex
// Created: APE 2005-01-10
// -----------------------------------------------------------------------------
inline
int ADN_GraphData::GetDataIndex( const ADN_GraphValue& value ) const
{
    auto it = std::find( graphValueList_.begin(), graphValueList_.end(), &value );
    assert( it != graphValueList_.end() );
    return static_cast< int >( std::distance( graphValueList_.begin(), it ) );
}

#endif // __ADN_GraphData_h_
