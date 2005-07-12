//*****************************************************************************
//
// $Created: FBD 03-01-10 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LimaFuncCheckListItem.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 6 $
// $Workfile: MOS_LimaFuncCheckListItem.h $
//
//*****************************************************************************

#ifndef __MOS_LimaFuncCheckListItem_h_
#define __MOS_LimaFuncCheckListItem_h_

#include "MOS_Types.h"

#include "MOS_CheckListItem_ABC.h"

#include "MOS_Order_Def.h"

//*****************************************************************************
// Created: FBD 03-01-10
//*****************************************************************************
class MOS_LimaFuncCheckListItem : public MOS_CheckListItem_ABC
{
    MT_COPYNOTALLOWED( MOS_LimaFuncCheckListItem );

public:
    explicit MOS_LimaFuncCheckListItem( E_FuncLimaType nFuncType, QListViewItem& item, QCheckListItem::Type nCheckable = QCheckListItem::CheckBox );
    virtual ~MOS_LimaFuncCheckListItem();


    //-------------------------------------------------------------------------
    /** @name Accessors*/
    //-------------------------------------------------------------------------
    //@{
    E_FuncLimaType GetType() const;
    static std::string BuildNameFuncLima( E_FuncLimaType nFuncType );

    void stateChange ( bool bState );
    //@}

private:
    E_FuncLimaType nFuncType_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_LimaFuncCheckListItem.inl"
#endif


#endif // __MOS_LimaFuncCheckListItem_h_