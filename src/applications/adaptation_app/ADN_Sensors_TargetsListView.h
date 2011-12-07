// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-01-18 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Sensors_TargetsListView.h $
// $Author: Ape $
// $Modtime: 5/04/05 10:53 $
// $Revision: 3 $
// $Workfile: ADN_Sensors_TargetsListView.h $
//
// *****************************************************************************

#ifndef __ADN_Sensors_TargetsListView_h_
#define __ADN_Sensors_TargetsListView_h_

#include "ADN_ListView.h"
#include "ADN_Objects_Data.h"


// =============================================================================
/** @class  ADN_Sensors_TargetsListView
    @brief  ADN_Sensors_TargetsListView
*/
// Created: APE 2005-01-18
// =============================================================================
class ADN_Sensors_TargetsListView
: public ADN_ListView
{
public:
    //! @name Constructors/Destructor
    //@{
     ADN_Sensors_TargetsListView( QWidget* pParent = 0, const char* szName = 0, Qt::WFlags f = 0 );
    virtual ~ADN_Sensors_TargetsListView();
    //@}

private:
    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt);

    bool Contains( const ADN_Objects_Data_ObjectInfos* pInfo );
    //@}
};


#endif // __ADN_Sensors_TargetsListView_h_
