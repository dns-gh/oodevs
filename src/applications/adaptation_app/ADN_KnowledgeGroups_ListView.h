// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
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
class ADN_KnowledgeGroups_ListView : public ADN_ListView
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_KnowledgeGroups_ListView( QWidget* pParent = 0, const char* szName = 0, Qt::WFlags f = 0 );
    virtual ~ADN_KnowledgeGroups_ListView();
    //@}

    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    //@}
};

#endif // __ADN_KnowledgeGroups_ListView_h_
