// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Resources_GenericListView_h_
#define __ADN_Resources_GenericListView_h_

#include "ADN_ListView.h"
#include "ADN_Enums.h"

namespace sword
{
    enum DotationType;
}

// =============================================================================
/** @class  ADN_Resources_GenericListView
    @brief  ADN_Resources_GenericListView
*/
// Created: APE 2004-12-29
// =============================================================================
class ADN_Resources_GenericListView : public ADN_ListView
{
public:
    //! @name Constructors/Destructor
    //@{
    ADN_Resources_GenericListView( QWidget* pParent, sword::DotationType nType );
    virtual ~ADN_Resources_GenericListView();
    //@}

private:
    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    std::string GetToolTipFor( const QModelIndex& index );
    //@}

private:
    sword::DotationType nType_;
};

#endif // __ADN_Resources_GenericListView_h_
