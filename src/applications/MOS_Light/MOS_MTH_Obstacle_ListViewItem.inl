//*****************************************************************************
//
// $Created: AGN 03-08-26 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_Obstacle_ListViewItem.inl $
// $Author: Nld $
// $Modtime: 26/11/04 14:43 $
// $Revision: 5 $
// $Workfile: MOS_MTH_Obstacle_ListViewItem.inl $
//
//*****************************************************************************



//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListViewItem::GetObstacleType
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
inline
ASN1T_EnumObjectType MOS_MTH_Obstacle_ListViewItem::GetObstacleType() const
{
    return nTypeObstacle_;
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListViewItem::GetLocalisationType
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
inline
ASN1T_EnumTypeLocalisation MOS_MTH_Obstacle_ListViewItem::GetLocalisationType() const
{
    return nTypeLocalisation_;
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListViewItem::GetId
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
inline
uint MOS_MTH_Obstacle_ListViewItem::GetId() const
{
    return nID_;
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListViewItem::GetIDPlanifie
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
inline
uint MOS_MTH_Obstacle_ListViewItem::GetIDPlanifie() const
{
    return nIDPlanifie_;
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListViewItem::GetUrgence
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
inline
ASN1T_EnumMissionGenUrgence MOS_MTH_Obstacle_ListViewItem::GetUrgence() const
{
    return nUrgence_;
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListViewItem::GetSousType
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
inline
ASN1T_EnumMissionGenSousTypeObstacle MOS_MTH_Obstacle_ListViewItem::GetSousType() const
{
    return nPreliminaire_;
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListViewItem::GetListPoint
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
inline
const T_PointVector& MOS_MTH_Obstacle_ListViewItem::GetListPoint()
{
    return vLocalisationPoint_;
}



//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListViewItem::ClearList
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
inline
void MOS_MTH_Obstacle_ListViewItem::ClearList()
{
    vLocalisationPoint_.clear();
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListViewItem::AddPoint
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
inline
void MOS_MTH_Obstacle_ListViewItem::AddPoint( const MT_Vector2D& point )
{
    vLocalisationPoint_.push_back( point );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListViewItem::SetObstacleType
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
inline
void MOS_MTH_Obstacle_ListViewItem::SetObstacleType( ASN1T_EnumObjectType nType )
{
    nTypeObstacle_ = nType;
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListViewItem::SetLocalisationType
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
inline
void MOS_MTH_Obstacle_ListViewItem::SetLocalisationType( ASN1T_EnumTypeLocalisation nType )
{
    nTypeLocalisation_ = nType;
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListViewItem::SetId
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
inline
void MOS_MTH_Obstacle_ListViewItem::SetId( uint nType )
{
    nID_ = nType;
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListViewItem::SetIDPlanifie
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
inline
void MOS_MTH_Obstacle_ListViewItem::SetIDPlanifie( uint nID )
{
    nIDPlanifie_ = nID;
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListViewItem::SetUrgence
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
inline
void MOS_MTH_Obstacle_ListViewItem::SetUrgence( ASN1T_EnumMissionGenUrgence nType )
{
    nUrgence_ = nType;
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListViewItem::SetSousType
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
inline
void MOS_MTH_Obstacle_ListViewItem::SetSousType( ASN1T_EnumMissionGenSousTypeObstacle nType )
{
    nPreliminaire_ = nType;
}


// -----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListViewItem::SetPriorite
// Created: NLD 2004-04-23
// -----------------------------------------------------------------------------
inline
void MOS_MTH_Obstacle_ListViewItem::SetPriorite( ASN1T_EnumMissionGenPriorite nPriorite )
{
    nPriorite_ = nPriorite;
}

// -----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListViewItem::GetPriorite
// Created: NLD 2004-04-23
// -----------------------------------------------------------------------------
inline
ASN1T_EnumMissionGenPriorite MOS_MTH_Obstacle_ListViewItem::GetPriorite() const
{
    return nPriorite_;    
}
