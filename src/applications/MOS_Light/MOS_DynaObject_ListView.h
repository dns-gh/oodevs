//*****************************************************************************
//
// $Created: AGN 03-07-18 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_DynaObject_ListView.h $
// $Author: Nld $
// $Modtime: 7/03/05 13:32 $
// $Revision: 6 $
// $Workfile: MOS_DynaObject_ListView.h $
//
//*****************************************************************************

#ifndef __MOS_DynaObject_ListView_h_
#define __MOS_DynaObject_ListView_h_

#include "MOS_Types.h"

#include "MOS_ASN_Types.h"

#include <QListView.h>
#include <QObjectDefs.h>

class MOS_DynaObject;
class MOS_DynaObject_Editor;
class MOS_DynaObject_Creator;



//*****************************************************************************
// Created: AGN 03-07-18
//*****************************************************************************
class MOS_DynaObject_ListView
: public QListView
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_DynaObject_ListView )

public:
    explicit MOS_DynaObject_ListView( QWidget* pParentWidget );
    virtual ~MOS_DynaObject_ListView();

    void AddObject   ( MOS_DynaObject& object );
    void RemoveObject( MOS_DynaObject& object );

private slots:

    //-------------------------------------------------------------------------
    /** @name Slots */
    //-------------------------------------------------------------------------
    //@{    
    void SlotOnClick             ( QListViewItem* pItem );
    void SlotContextMenuRequested( QListViewItem* pItem, const QPoint& point );
    //@}

private:
    typedef std::map< ASN1T_EnumObjectType, QListViewItem* > T_ObjectId_Item_Map;
    typedef T_ObjectId_Item_Map::iterator              IT_ObjectId_Item_Map;

private:
    T_ObjectId_Item_Map     itemObjectTypeMap_;
    MOS_DynaObject_Editor*  pDynaObjectEditor_;        
    MOS_DynaObject_Creator* pDynaObjectCreator_;        
};

#ifdef MOS_USE_INLINE
#	include "MOS_DynaObject_ListView.inl"
#endif


#endif // __MOS_DynaObject_ListView_h_