// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
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
    explicit ADN_NBC_NbcAgentListView( QWidget* pParent );
    virtual ~ADN_NBC_NbcAgentListView();

private:
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
};

#endif // __ADN_NBC_NbcAgentListView_h_
