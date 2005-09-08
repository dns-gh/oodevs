//*****************************************************************************
//
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ObjectKnowledge.inl $
// $Author: Ape $
// $Modtime: 14/06/04 16:57 $
// $Revision: 2 $
// $Workfile: MOS_ObjectKnowledge.inl $
//
//*****************************************************************************



// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge::GetID
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_INLINE
uint MOS_ObjectKnowledge::GetID() const
{
    return nID_;    
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge::IsUpdated
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_INLINE
bool MOS_ObjectKnowledge::IsValid( E_AttributeUpdated nAttr ) const
{
    return nAttrUpdated_ & nAttr;
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge::GetCenter
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_INLINE
const MT_Vector2D& MOS_ObjectKnowledge::GetCenter() const
{
    return vCenter_;    
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge::GetPointList
// Created: APE 2004-06-14
// -----------------------------------------------------------------------------
MOS_INLINE
const T_PointVector& MOS_ObjectKnowledge::GetPointList() const
{
    return points_;
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge::GetRealObject
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_INLINE
MOS_Object_ABC* MOS_ObjectKnowledge::GetRealObject() const
{
    return pRealObject_;    
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge::GetOwner
// Created: APE 2004-06-10
// -----------------------------------------------------------------------------
MOS_INLINE
MOS_Team& MOS_ObjectKnowledge::GetOwner() const
{
    return owner_;
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge::GetSiteFranchissementLargeur
// Created: APE 2004-06-14
// -----------------------------------------------------------------------------
MOS_INLINE
uint MOS_ObjectKnowledge::GetSiteFranchissementLargeur() const
{
    return nSiteFranchissementLargeur_;
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge::GetSiteFranchissementProfondeur
// Created: APE 2004-06-14
// -----------------------------------------------------------------------------
MOS_INLINE
uint MOS_ObjectKnowledge::GetSiteFranchissementProfondeur() const
{
    return nSiteFranchissementProfondeur_;
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge::GetSiteFranchissementVitesseCourant
// Created: APE 2004-06-14
// -----------------------------------------------------------------------------
MOS_INLINE
uint MOS_ObjectKnowledge::GetSiteFranchissementVitesseCourant() const
{
    return nSiteFranchissementVitesseCourant_;
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge::GetSiteFranchissementBergesAAmenager
// Created: APE 2004-06-14
// -----------------------------------------------------------------------------
MOS_INLINE
bool MOS_ObjectKnowledge::GetSiteFranchissementBergesAAmenager() const
{
    return nSiteFranchissementBergesAAmenager_;
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge::GetNuageNBCAgentNbcId
// Created: APE 2004-06-14
// -----------------------------------------------------------------------------
MOS_INLINE
uint MOS_ObjectKnowledge::GetNuageNBCAgentNbcId() const
{
    return nNuageNBCAgentNbcID_;
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge::GetROTADanger
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
MOS_INLINE
uint MOS_ObjectKnowledge::GetROTADanger() const
{
    return nROTADanger_;
}

// -----------------------------------------------------------------------------
// Name: std::vector< uint >& MOS_ObjectKnowledge::GetROTANBCAgents
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
MOS_INLINE
const std::vector< uint >& MOS_ObjectKnowledge::GetROTANBCAgents() const
{
    return rotaNBCAgents_;
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge::GetLogRouteFlow
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
MOS_INLINE
uint MOS_ObjectKnowledge::GetLogRouteFlow() const
{
    return nLogRouteFlow_;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge::GetLogRouteWidth
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
MOS_INLINE
uint MOS_ObjectKnowledge::GetLogRouteWidth() const
{
    return nLogRouteWidth_;
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge::GetLogRouteLength
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
MOS_INLINE
uint MOS_ObjectKnowledge::GetLogRouteLength() const
{
    return nLogRouteLength_;
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge::GetLogRouteMaxWeight
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
MOS_INLINE
uint MOS_ObjectKnowledge::GetLogRouteMaxWeight() const
{
    return nLogRouteMaxWeight_;
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge::GetLogRouteEquipped
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
MOS_INLINE
bool MOS_ObjectKnowledge::GetLogRouteEquipped() const
{
    return bLogRouteEquipped_;
}

// -----------------------------------------------------------------------------
// Name: MOS_ObjectKnowledge::GetCampTC2ID
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
MOS_INLINE
uint MOS_ObjectKnowledge::GetCampTC2ID() const
{
    return nCampTC2ID_;
}
