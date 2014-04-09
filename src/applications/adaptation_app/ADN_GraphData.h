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

#include "GQ_PlotData.h"
#include <boost/noncopyable.hpp>

class ADN_GraphValue;
class ADN_Connector_ABC;

// =============================================================================
/** @class  ADN_GraphData
    @brief  ADN_GraphData
*/
// Created: APE 2004-12-21
// =============================================================================
class ADN_GraphData : public GQ_PlotData
                    , private boost::noncopyable
{
public:
    typedef std::vector< ADN_GraphValue* > T_GraphValue_Vector;

public:
    //! @name Constructors/Destructor
    //@{
             ADN_GraphData( uint nUserID, GQ_Plot& );
    virtual ~ADN_GraphData();
    //@}

    //! @name Operations
    //@{
    void AddData( ADN_GraphValue& );
    void DeleteData( void* pRelatedObject );

    void OnDataChanged( ADN_GraphValue& );
    int GetDataIndex( const ADN_GraphValue& ) const;
    void SelectRelatedData( void* pObj );

    void SetValueCreator( const std::function< void (void*) >& valueCreator );
    ADN_Connector_ABC& GetConnector() const;
    ADN_GraphValue& GetGraphValue( uint nIndex );
    //@}

private:
    //! @name Member data
    //@{
    T_GraphValue_Vector graphValueList_;
    ADN_Connector_ABC* pConnector_;
    //@}
};

#endif // __ADN_GraphData_h_
