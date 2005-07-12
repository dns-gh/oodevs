//*****************************************************************************
//
// $Created: AGN 03-08-26 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_Obstacle_ListViewItem.h $
// $Author: Nld $
// $Modtime: 26/11/04 14:42 $
// $Revision: 5 $
// $Workfile: MOS_MTH_Obstacle_ListViewItem.h $
//
//*****************************************************************************

#ifndef __MOS_MTH_Obstacle_ListViewItem_h_
#define __MOS_MTH_Obstacle_ListViewItem_h_

#include <qlistview.h>

#include "MOS_ASN_Types.h"

//*****************************************************************************
// Created: AGN 03-08-26
//*****************************************************************************
class MOS_MTH_Obstacle_ListViewItem
: public QListViewItem
{
    MT_COPYNOTALLOWED( MOS_MTH_Obstacle_ListViewItem )

public:
    enum
    {
        e_rtti = 21
    };
    
public:
    explicit MOS_MTH_Obstacle_ListViewItem( QListView* pParent );
    virtual ~MOS_MTH_Obstacle_ListViewItem();

    virtual int rtti() const;

    const T_PointVector& GetListPoint();

    void ClearList();
    void AddPoint( const MT_Vector2D& point );

    ASN1T_EnumObjectType GetObstacleType() const;
    ASN1T_EnumTypeLocalisation GetLocalisationType() const;
    uint GetId() const;
    ASN1T_EnumMissionGenUrgence GetUrgence() const;
    ASN1T_EnumMissionGenSousTypeObstacle GetSousType() const;
    uint GetIDPlanifie() const;

    void SetObstacleType( ASN1T_EnumObjectType nType );
    void SetLocalisationType( ASN1T_EnumTypeLocalisation nType );
    void SetId( uint nType );
    void SetIDPlanifie( uint nType );
    void SetUrgence( ASN1T_EnumMissionGenUrgence nType );
    void SetSousType( ASN1T_EnumMissionGenSousTypeObstacle nType );

    void SetPriorite( ASN1T_EnumMissionGenPriorite nPriorite );
    ASN1T_EnumMissionGenPriorite GetPriorite() const;

    void FillRandomParameters( MT_OutputArchive_ABC& archive );
    void ReloadParameters( MT_InputArchive_ABC& archive );

private:
    ASN1T_EnumObjectType nTypeObstacle_;
    T_PointVector vLocalisationPoint_;
    ASN1T_EnumTypeLocalisation nTypeLocalisation_;
    uint nID_;
    uint nIDPlanifie_;
    ASN1T_EnumMissionGenUrgence nUrgence_;
    ASN1T_EnumMissionGenSousTypeObstacle nPreliminaire_;
    ASN1T_EnumMissionGenPriorite nPriorite_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_MTH_Obstacle_ListViewItem.inl"
#endif


#endif // __MOS_MTH_Obstacle_ListViewItem_h_