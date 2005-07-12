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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ListViewItem_ABC.h $
// $Author: Nld $
// $Modtime: 25/10/04 15:52 $
// $Revision: 11 $
// $Workfile: MOS_ListViewItem_ABC.h $
//
//*****************************************************************************

#ifndef __MOS_ListViewItem_ABC_h_
#define __MOS_ListViewItem_ABC_h_

#ifdef __GNUG__
#    pragma interface
#endif

#include "MOS_Types.h"
#include <qlistview.h>

class MOS_;

enum E_ItemType
{
    eItemTypeNone,
    eItemTypePion,
    eItemTypeAutomate,
    eItemTypeArme,
    eItemTypeKnowledgeGroup,
    eItemTypeAgent,
    eItemTypeAgentAttribute,
    eItemTypeEltRC,
    eItemTypeEltTrace,
    eItemTypeEltText,
    eItemTypeEltOrderConduite,
    eItemTypeEltOrderMission,
    eItemTypeEltCheckValue,
    eItemTypeEltLima,
    eItemTypeEltCheckLima,
    eItemTypeEltPoint,
    eItemTypeEltFuncLima,
    eItemTypeEltLimit,
    eItemTypeEltListRoot,
    eItemTypeODBPion,
    eItemTypeODBAutomate,
    eItemTypeODBArme,
    eItemTypeODBCamp,
};

//-------------------------------------------------------------------------
/** @name Types */
//-------------------------------------------------------------------------
//@{
enum E_ListViewType
{
    eListViewEditor,
    eListViewSelector,
    eListViewMission
};
//@}
//=============================================================================
/**
*   @class  MOS_ListViewItem_ABC
*   @brief  PUT THE COMMENTS ON THE CLASS HERE
*   @par    Using example
*   @code
*     PUT YOUR EXAMPLE CODE HERE
*   @endcode
*/
// Created: NLD 2002-07-16
//=============================================================================
class MOS_ListViewItem_ABC : public QListViewItem
{
    MT_COPYNOTALLOWED( MOS_ListViewItem_ABC );

public:

public:
    MOS_ListViewItem_ABC( E_ItemType nItemType, QListView& listView, const std::string& strField = "", const std::string& strValue = "" );
    MOS_ListViewItem_ABC( E_ItemType nItemType, QListViewItem& itemParent, const std::string& strField = "", const std::string& strValue = "" );
    MOS_ListViewItem_ABC( E_ItemType nItemType, QListViewItem& itemParent, QListViewItem& itemAfter,  const std::string& strField = "", const std::string& strValue = "" );
    MOS_ListViewItem_ABC( E_ItemType nItemType, QListView& itemParent, QListViewItem& itemAfter,  const std::string& strField = "", const std::string& strValue = "" );
    virtual ~MOS_ListViewItem_ABC();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    E_ItemType GetItemType() const;
    //@}

private:
    E_ItemType nItemType_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_ListViewItem_ABC.inl"
#endif

#endif // __MOS_ListViewItem_ABC_h_
