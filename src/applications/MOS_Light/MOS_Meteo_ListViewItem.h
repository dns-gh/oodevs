//*****************************************************************************
//
// $Created: AGN 03-08-07 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Meteo_ListViewItem.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 2 $
// $Workfile: MOS_Meteo_ListViewItem.h $
//
//*****************************************************************************

#ifndef __MOS_Meteo_ListViewItem_h_
#define __MOS_Meteo_ListViewItem_h_

#include "MOS_Types.h"
#include <qlistview.h>

class MOS_Meteo_Local;

//*****************************************************************************
// Created: AGN 03-08-07
//*****************************************************************************
class MOS_Meteo_ListViewItem
: public QListViewItem
{
    MT_COPYNOTALLOWED( MOS_Meteo_ListViewItem )
public:
    enum
    {
        e_rtti = 19
    };

public:
    MOS_Meteo_ListViewItem( QListView* pParent, MOS_Meteo_Local& localWeather );
    virtual ~MOS_Meteo_ListViewItem();

    virtual int rtti() const;

    MOS_Meteo_Local& GetLocalWeather();

private:
    MOS_Meteo_Local& localWeather_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_Meteo_ListViewItem.inl"
#endif


#endif // __MOS_Meteo_ListViewItem_h_