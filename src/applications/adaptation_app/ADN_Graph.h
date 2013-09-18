// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Graph_h_
#define __ADN_Graph_h_

#include "GQ_Plot.h"

class ADN_Connector_Vector_ABC;
class ADN_ListView;

// =============================================================================
/** @class  ADN_Graph
    @brief  ADN_Graph
    @par    Using example
    @code
    ADN_Graph;
    @endcode
*/
// Created: APE 2004-12-14
// =============================================================================
class ADN_Graph : public GQ_Plot
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Graph( QWidget* pParent = 0 );
    virtual ~ADN_Graph();
    //@}

    //! @name Operations
    //@{
    void Connect( ADN_ListView& listView );
    void SetConnector( ADN_Connector_Vector_ABC& connector );
    //@}

public slots:
    void SetSelectedItem( void* pObject );

signals:
    void ItemSelected( void* pObject );

private slots:
    void OnSelectionChanged( const T_DataItemVector& );

private:
    //! @name Member data
    //@{
    ADN_Connector_Vector_ABC* pConnector_;
    bool bSlotsLocked_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ADN_Graph::SetConnector
// Created: APE 2005-01-12
// -----------------------------------------------------------------------------
inline
void ADN_Graph::SetConnector( ADN_Connector_Vector_ABC& connector )
{
    pConnector_ = &connector;
}

#endif // __ADN_Graph_h_
