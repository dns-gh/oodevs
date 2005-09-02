//*****************************************************************************
//
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_DynaObjectKnowledge.inl $
// $Author: Ape $
// $Modtime: 14/06/04 16:57 $
// $Revision: 2 $
// $Workfile: MOS_DynaObjectKnowledge.inl $
//
//*****************************************************************************



// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectKnowledge::GetID
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_INLINE
uint MOS_DynaObjectKnowledge::GetID() const
{
    return nID_;    
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectKnowledge::IsUpdated
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_INLINE
bool MOS_DynaObjectKnowledge::IsValid( E_AttributeUpdated nAttr ) const
{
    return nAttrUpdated_ & nAttr;
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectKnowledge::GetCenter
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_INLINE
const MT_Vector2D& MOS_DynaObjectKnowledge::GetCenter() const
{
    return vCenter_;    
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectKnowledge::GetPointList
/** @return 
*/
// Created: APE 2004-06-14
// -----------------------------------------------------------------------------
MOS_INLINE
const T_PointVector& MOS_DynaObjectKnowledge::GetPointList() const
{
    return points_;
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectKnowledge::GetRealObject
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
MOS_INLINE
MOS_DynaObject_ABC* MOS_DynaObjectKnowledge::GetRealObject() const
{
    return pRealObject_;    
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectKnowledge::GetOwner
/** @return 
*/
// Created: APE 2004-06-10
// -----------------------------------------------------------------------------
MOS_INLINE
MOS_Team& MOS_DynaObjectKnowledge::GetOwner() const
{
    return owner_;
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectKnowledge::GetSiteFranchissementLargeur
/** @return 
*/
// Created: APE 2004-06-14
// -----------------------------------------------------------------------------
MOS_INLINE
uint MOS_DynaObjectKnowledge::GetSiteFranchissementLargeur() const
{
    return nSiteFranchissementLargeur_;
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectKnowledge::GetSiteFranchissementProfondeur
/** @return 
*/
// Created: APE 2004-06-14
// -----------------------------------------------------------------------------
MOS_INLINE
uint MOS_DynaObjectKnowledge::GetSiteFranchissementProfondeur() const
{
    return nSiteFranchissementProfondeur_;
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectKnowledge::GetSiteFranchissementVitesseCourant
/** @return 
*/
// Created: APE 2004-06-14
// -----------------------------------------------------------------------------
MOS_INLINE
uint MOS_DynaObjectKnowledge::GetSiteFranchissementVitesseCourant() const
{
    return nSiteFranchissementVitesseCourant_;
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectKnowledge::GetSiteFranchissementBergesAAmenager
/** @return 
*/
// Created: APE 2004-06-14
// -----------------------------------------------------------------------------
MOS_INLINE
bool MOS_DynaObjectKnowledge::GetSiteFranchissementBergesAAmenager() const
{
    return nSiteFranchissementBergesAAmenager_;
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectKnowledge::GetNuageNBCAgentNbcId
/** @return 
*/
// Created: APE 2004-06-14
// -----------------------------------------------------------------------------
MOS_INLINE
uint MOS_DynaObjectKnowledge::GetNuageNBCAgentNbcId() const
{
    return nNuageNBCAgentNbcID_;
}
