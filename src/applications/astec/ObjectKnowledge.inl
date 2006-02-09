//*****************************************************************************
//
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ObjectKnowledge.inl $
// $Author: Ape $
// $Modtime: 14/06/04 16:57 $
// $Revision: 2 $
// $Workfile: ObjectKnowledge.inl $
//
//*****************************************************************************



// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetID
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
inline
uint ObjectKnowledge::GetID() const
{
    return nID_;    
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetCenter
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& ObjectKnowledge::GetCenter() const
{
    return vCenter_;    
}


// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetPointList
// Created: APE 2004-06-14
// -----------------------------------------------------------------------------
inline
const T_PointVector& ObjectKnowledge::GetPointList() const
{
    return points_;
}


// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetRealObject
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
inline
Object_ABC* ObjectKnowledge::GetRealObject() const
{
    return pRealObject_;    
}


// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetOwner
// Created: APE 2004-06-10
// -----------------------------------------------------------------------------
inline
Team& ObjectKnowledge::GetOwner() const
{
    return owner_;
}


// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetSiteFranchissementLargeur
// Created: APE 2004-06-14
// -----------------------------------------------------------------------------
inline
uint ObjectKnowledge::GetSiteFranchissementLargeur() const
{
    return nSiteFranchissementLargeur_;
}


// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetSiteFranchissementProfondeur
// Created: APE 2004-06-14
// -----------------------------------------------------------------------------
inline
uint ObjectKnowledge::GetSiteFranchissementProfondeur() const
{
    return nSiteFranchissementProfondeur_;
}


// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetSiteFranchissementVitesseCourant
// Created: APE 2004-06-14
// -----------------------------------------------------------------------------
inline
uint ObjectKnowledge::GetSiteFranchissementVitesseCourant() const
{
    return nSiteFranchissementVitesseCourant_;
}


// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetSiteFranchissementBergesAAmenager
// Created: APE 2004-06-14
// -----------------------------------------------------------------------------
inline
bool ObjectKnowledge::GetSiteFranchissementBergesAAmenager() const
{
    return nSiteFranchissementBergesAAmenager_;
}


// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetNuageNBCAgentNbcId
// Created: APE 2004-06-14
// -----------------------------------------------------------------------------
inline
uint ObjectKnowledge::GetNuageNBCAgentNbcId() const
{
    return nNuageNBCAgentNbcID_;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetROTADanger
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
inline
uint ObjectKnowledge::GetROTADanger() const
{
    return nROTADanger_;
}

// -----------------------------------------------------------------------------
// Name: std::vector< uint >& ObjectKnowledge::GetROTANBCAgents
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
inline
const std::vector< uint >& ObjectKnowledge::GetROTANBCAgents() const
{
    return rotaNBCAgents_;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetLogRouteFlow
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
inline
uint ObjectKnowledge::GetLogRouteFlow() const
{
    return nLogRouteFlow_;
}
    
// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetLogRouteWidth
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
inline
uint ObjectKnowledge::GetLogRouteWidth() const
{
    return nLogRouteWidth_;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetLogRouteLength
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
inline
uint ObjectKnowledge::GetLogRouteLength() const
{
    return nLogRouteLength_;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetLogRouteMaxWeight
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
inline
uint ObjectKnowledge::GetLogRouteMaxWeight() const
{
    return nLogRouteMaxWeight_;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetLogRouteEquipped
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
inline
bool ObjectKnowledge::GetLogRouteEquipped() const
{
    return bLogRouteEquipped_;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetCampTC2ID
// Created: SBO 2005-09-05
// -----------------------------------------------------------------------------
inline
uint ObjectKnowledge::GetCampTC2ID() const
{
    return nCampTC2ID_;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetObjectTypeID
// Created: HME 2005-11-08
// -----------------------------------------------------------------------------
inline 
ASN1T_EnumObjectType ObjectKnowledge::GetObjectTypeID() const
{
    return nObjectTypeID_;
}