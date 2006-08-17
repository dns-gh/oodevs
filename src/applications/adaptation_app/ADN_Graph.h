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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Graph.h $
// $Author: Ape $
// $Modtime: 12/01/05 15:41 $
// $Revision: 2 $
// $Workfile: ADN_Graph.h $
//
// *****************************************************************************

#ifndef __ADN_Graph_h_
#define __ADN_Graph_h_

#include "ADN_Type_Vector_ABC.h"

#include "GQ_Plot.h"


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
    MT_COPYNOTALLOWED( ADN_Graph )

public:
    //! @name Constructors/Destructor
    //@{
     ADN_Graph( QWidget* pParent = 0 );
    ~ADN_Graph();
    //@}

    //! @name Operations
    //@{
    void Connect( ADN_ListView& listView );
    void SetConnector( ADN_Connector_Vector_ABC& connector );
    //@}

    //! @name Accessors
    //@{
    //@}

    //! @name Modifiers
    //@{
    //@}

    //! @name Operators
    //@{
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
