// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Sensors_DisastersListView_h_
#define __ADN_Sensors_DisastersListView_h_

#include "ADN_ListView.h"
#include "ADN_Disasters_Data.h"

// =============================================================================
/** @class  ADN_Sensors_DisastersListView
    @brief  ADN_Sensors_DisastersListView
*/
// Created: LGY 2012-12-03
// =============================================================================
class ADN_Sensors_DisastersListView : public ADN_ListView
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Sensors_DisastersListView( QWidget* pParent = 0 );
    virtual ~ADN_Sensors_DisastersListView();
    //@}

private:
    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt);

    bool Contains( const ADN_Disasters_Data::DisasterInfos* pInfo );
    //@}
};

#endif // __ADN_Sensors_DisastersListView_h_
