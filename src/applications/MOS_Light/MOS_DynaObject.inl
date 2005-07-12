//*****************************************************************************
//
// $Created: NLD 2003-01-29 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_DynaObject.inl $
// $Author: Nld $
// $Modtime: 27/04/05 14:55 $
// $Revision: 11 $
// $Workfile: MOS_DynaObject.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetID
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
inline
uint MOS_DynaObject::GetID() const
{
    return nID_;
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject::SetViewItem
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
inline
void MOS_DynaObject::SetViewItem( MOS_DynaObject_ListViewItem* pItem )
{
    pItem_ = pItem;
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetViewItem
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
inline
MOS_DynaObject_ListViewItem* MOS_DynaObject::GetViewItem()
{
    return pItem_;
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetType
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
inline
ASN1T_EnumObjectType MOS_DynaObject::GetType() const
{
    return nType_;
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject::IsPrepared
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
inline
bool MOS_DynaObject::IsPrepared() const
{
    return bPrepared_;
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetConstructionPercentage
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
inline
MT_Float MOS_DynaObject::GetConstructionPercentage() const
{
    return rConstructionPercentage_;
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetValorizationPercentage
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
inline
MT_Float MOS_DynaObject::GetValorizationPercentage() const
{
    return rValorizationPercentage_;
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetBypassConstructionPercentage
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
inline
MT_Float MOS_DynaObject::GetBypassConstructionPercentage() const
{
    return rBypassConstructionPercentage_;
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetCenter
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
inline
const MT_Vector2D& MOS_DynaObject::GetCenter() const
{
    return center_;
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::AreAttrSiteFranchissementPresent
// Created: NLD 2003-12-01
// -----------------------------------------------------------------------------
inline
bool MOS_DynaObject::AreAttrSiteFranchissementPresent() const
{
    return bAttrSiteFranchissementPresent_;
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetSiteFranchissementLargeur
// Created: NLD 2003-12-01
// -----------------------------------------------------------------------------
inline
uint MOS_DynaObject::GetSiteFranchissementLargeur() const
{
    return nSiteFranchissementLargeur_;
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetSiteFranchissementProfondeur
// Created: NLD 2003-12-01
// -----------------------------------------------------------------------------
inline
uint MOS_DynaObject::GetSiteFranchissementProfondeur() const
{
    return nSiteFranchissementProfondeur_;
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetSiteFranchissementVitesseCourant
// Created: NLD 2003-12-01
// -----------------------------------------------------------------------------
inline
uint MOS_DynaObject::GetSiteFranchissementVitesseCourant() const
{
    return nSiteFranchissementVitesseCourant_;
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetSiteFranchissementBergesAAmenager
// Created: NLD 2003-12-01
// -----------------------------------------------------------------------------
inline
bool MOS_DynaObject::GetSiteFranchissementBergesAAmenager() const
{
    return nSiteFranchissementBergesAAmenager_;
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::AreAttrNuageNBCPresent
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
inline
bool MOS_DynaObject::AreAttrNuageNBCPresent() const
{
    return bAttrNuageNBCPresent_;    
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetNuageNBCAgentNbcId
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
inline
uint MOS_DynaObject::GetNuageNBCAgentNbcId() const
{
    return nNuageNBCAgentNbcID_;    
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetNuageNBCAgentNbcId
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
inline
uint MOS_DynaObject::GetIDPlanfie() const
{
    return nIDPlanifie_;    
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::AreAttrRotaPresent
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
inline
bool MOS_DynaObject::AreAttrRotaPresent() const
{
    return bAttrRotaPresent_;
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetRotaDanger
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
inline
uint MOS_DynaObject::GetRotaDanger() const
{
    return nRotaDanger_;
}

// -----------------------------------------------------------------------------
// Name: std::vector< uint > MOS_DynaObject::GetRotaNbcAgents
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
inline
const std::vector< uint >& MOS_DynaObject::GetRotaNbcAgents() const
{
    return rotaNbcAgents_;
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::AreAttrItineraireLogistiquePresent
// Created: NLD 2005-02-18
// -----------------------------------------------------------------------------
inline
bool MOS_DynaObject::AreAttrItineraireLogistiquePresent() const
{
    return bAttrItineraireLogisticPresent_;
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::IsItineraireLogistiqueEquipped
// Created: NLD 2005-02-18
// -----------------------------------------------------------------------------
inline
bool MOS_DynaObject::IsItineraireLogistiqueEquipped() const
{
    return bItineraireLogistiqueEquipped_;
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetItineraireLogistiqueFlow
// Created: NLD 2005-04-27
// -----------------------------------------------------------------------------
inline
uint MOS_DynaObject::GetItineraireLogistiqueFlow() const
{
    return nItineraireLogistiqueFlow_;
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetItineraireLogistiqueWidth
// Created: NLD 2005-04-27
// -----------------------------------------------------------------------------
inline
uint MOS_DynaObject::GetItineraireLogistiqueWidth() const
{
    return nItineraireLogistiqueWidth_;
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetItineraireLogistiqueLength
// Created: NLD 2005-04-27
// -----------------------------------------------------------------------------
inline
uint MOS_DynaObject::GetItineraireLogistiqueLength() const
{
    return nItineraireLogistiqueLength_;
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObject::GetItineraireLogistiqueMaxWeight
// Created: NLD 2005-04-27
// -----------------------------------------------------------------------------
inline
uint MOS_DynaObject::GetItineraireLogistiqueMaxWeight() const
{
    return nItineraireLogistiqueMaxWeight_;
}
