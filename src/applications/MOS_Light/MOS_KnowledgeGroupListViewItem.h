//*****************************************************************************
//
// This file is part of MASA DIN library.
//
// Refer to the included end-user license agreement (License.rtf) for
// restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
// All Rights Reserved. DIN is a trademark of MASA Corporation.
//
//*****************************************************************************
//
// $Created: NLD 2002-07-16 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_KnowledgeGroupListViewItem.h $
// $Author: Nld $
// $Modtime: 25/10/04 16:30 $
// $Revision: 1 $
// $Workfile: MOS_KnowledgeGroupListViewItem.h $
//
//*****************************************************************************

#ifndef __MOS_KnowledgeGroupListViewItem_h_
#define __MOS_KnowledgeGroupListViewItem_h_

#include "MOS_Types.h"

#include "MOS_ListViewItem_ABC.h"

class MOS_Gtia;
class QListViewItem;
class MOS_Agent;
class MOS_AgentListView;

//=============================================================================
// Created: NLD 2002-07-16
//=============================================================================
class MOS_KnowledgeGroupListViewItem : public MOS_ListViewItem_ABC
{
    MT_COPYNOTALLOWED( MOS_KnowledgeGroupListViewItem );


public:
    MOS_KnowledgeGroupListViewItem( MOS_Gtia& gtia, QListViewItem& item, const std::string& strField );
    virtual ~MOS_KnowledgeGroupListViewItem();

    //! @name 
    //@{
    MOS_Gtia& GetGtia() const;
    //@}
   

private:
    MOS_Gtia& gtia_;
};

#include "MOS_KnowledgeGroupListViewItem.inl"

#endif // __MOS_KnowledgeGroupListViewItem_h_
