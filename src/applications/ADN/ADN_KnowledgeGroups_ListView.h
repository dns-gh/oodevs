// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-21 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_KnowledgeGroups_ListView.h $
// $Author: Ape $
// $Modtime: 21/03/05 16:07 $
// $Revision: 1 $
// $Workfile: ADN_KnowledgeGroups_ListView.h $
//
// *****************************************************************************

#ifndef __ADN_KnowledgeGroups_ListView_h_
#define __ADN_KnowledgeGroups_ListView_h_

#include "ADN_ListView.h"


// =============================================================================
/** @class  ADN_KnowledgeGroups_ListView
    @brief  ADN_KnowledgeGroups_ListView
*/
// Created: APE 2005-01-06
// =============================================================================
class ADN_KnowledgeGroups_ListView
    : public ADN_ListView
{
    MT_COPYNOTALLOWED( ADN_KnowledgeGroups_ListView )

public:
    //! @name Constructors/Destructor
    //@{
     ADN_KnowledgeGroups_ListView( QWidget* pParent = 0, const char* szName = 0, WFlags f = 0 );
    ~ADN_KnowledgeGroups_ListView();
    //@}

    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    //@}
};

#endif // __ADN_KnowledgeGroups_ListView_h_
