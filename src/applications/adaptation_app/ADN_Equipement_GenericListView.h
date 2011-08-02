// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-12-29 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Equipement_GenericListView.h $
// $Author: Ape $
// $Modtime: 7/04/05 11:21 $
// $Revision: 5 $
// $Workfile: ADN_Equipement_GenericListView.h $
//
// *****************************************************************************

#ifndef __ADN_Equipement_GenericListView_h_
#define __ADN_Equipement_GenericListView_h_

#include "ADN_ListView.h"
#include "ADN_Enums.h"

// =============================================================================
/** @class  ADN_Equipement_GenericListView
    @brief  ADN_Equipement_GenericListView
*/
// Created: APE 2004-12-29
// =============================================================================
class ADN_Equipement_GenericListView
: public ADN_ListView
{
public:
    //! @name Constructors/Destructor
    //@{
     ADN_Equipement_GenericListView( E_DotationFamily nType, QWidget* pParent = 0, const char* szName = 0, Qt::WFlags f = 0 );
    virtual ~ADN_Equipement_GenericListView();
    //@}

private:
    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    //@}

private:
    E_DotationFamily nType_;
};

#endif // __ADN_Equipement_GenericListView_h_
