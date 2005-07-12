// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-05-06 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_NBC_NbcAgentListView.h $
// $Author: Ape $
// $Modtime: 1/03/05 10:45 $
// $Revision: 3 $
// $Workfile: ADN_NBC_NbcAgentListView.h $
//
// *****************************************************************************

#ifndef __ADN_NBC_NbcAgentListView_h_
#define __ADN_NBC_NbcAgentListView_h_

#include "ADN_ListView.h"


// =============================================================================
/** @class  ADN_NBC_NbcAgentListView
    @brief  ADN_NBC_NbcAgentListView
*/
// Created: AGN 2004-05-06
// =============================================================================
class ADN_NBC_NbcAgentListView
: public ADN_ListView
{
public:
    explicit ADN_NBC_NbcAgentListView( QWidget* pParent = 0, const char* szName = 0, WFlags f = 0 );
    virtual ~ADN_NBC_NbcAgentListView();

private:
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
};


#endif // __ADN_NBC_NbcAgentListView_h_
