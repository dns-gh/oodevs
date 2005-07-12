//*****************************************************************************
//
// $Created: AGN 03-08-07 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Meteo_ListView.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 4 $
// $Workfile: MOS_Meteo_ListView.h $
//
//*****************************************************************************

#ifndef __MOS_Meteo_ListView_h_
#define __MOS_Meteo_ListView_h_

#include <qlistview.h>
#include <QObjectDefs.h>

class MOS_Meteo_Local_GUI;
class MT_OutputArchive_ABC;
class MT_InputArchive_ABC;

//*****************************************************************************
// Created: AGN 03-08-07
//*****************************************************************************
class MOS_Meteo_ListView
: public QListView
{
    Q_OBJECT

    enum E_MenuItem
    {
        eMenu_AddWeather,
        eMenu_UpdateWeather,
        eMenu_RemoveWeather
    };

public:
    explicit MOS_Meteo_ListView( QWidget* pParent );
    virtual ~MOS_Meteo_ListView();

    void FillRandomParameters();
    void ReloadParameters( MT_InputArchive_ABC& archive );
    void ResaveParameters( MT_OutputArchive_ABC& archive );

     

private slots:
    void OnContextMenu( QListViewItem* pItem, const QPoint& pos, int );

private:
    MOS_Meteo_Local_GUI* pWeatherEditor_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_Meteo_ListView.inl"
#endif


#endif // __MOS_Meteo_ListView_h_