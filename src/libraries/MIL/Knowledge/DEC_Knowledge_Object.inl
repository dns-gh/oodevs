// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_Object.inl $
// $Author: Jvt $
// $Modtime: 23/03/05 19:56 $
// $Revision: 6 $
// $Workfile: DEC_Knowledge_Object.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::GetMosID
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
inline
uint DEC_Knowledge_Object::GetMosID() const
{
    return nMosID_;    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::GetDiaID
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
inline
uint DEC_Knowledge_Object::GetDiaID() const
{
    return nDiaID_;    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::GetLocalisation
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
inline
const TER_Localisation& DEC_Knowledge_Object::GetLocalisation() const
{
    return localisation_;    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::GetAvoidanceLocalisation
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
inline
const TER_Localisation& DEC_Knowledge_Object::GetAvoidanceLocalisation() const
{
    return avoidanceLocalisation_;    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::GetType
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
inline
const MIL_RealObjectType& DEC_Knowledge_Object::GetType() const
{
    assert( pObjectType_ );
    return *pObjectType_;   
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::GetRelevance
// Created: NLD 2004-03-22
// -----------------------------------------------------------------------------
inline
MT_Float DEC_Knowledge_Object::GetRelevance() const
{
    return rRelevance_;    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::GetObjectKnown
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC* DEC_Knowledge_Object::GetObjectKnown() const
{
    return pObjectKnown_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::IsDissociatedFromRealObject
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_Object::IsDissociatedFromRealObject() const
{
    return pObjectKnown_ == 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::IsBypassed
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_Object::IsBypassed() const
{
    return nBypassPercentage_ >= 100;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::IsPrepared
// Created: NLD 2004-05-07
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_Object::IsPrepared() const
{
    return bIsPrepared_;    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
inline
void DEC_Knowledge_Object::UpdateSpecificAttributes( const PHY_PerceptionLevel& /*nCurrentPerceptionLevel*/ )
{
    // Nothing
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
inline
void DEC_Knowledge_Object::UpdateSpecificAttributes( const DEC_Knowledge_ObjectPerception& /*perception*/ )
{
    // Nothing
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
inline
void DEC_Knowledge_Object::UpdateSpecificAttributes( const DEC_Knowledge_ObjectCollision&  /*collision*/  )
{
    // Nothing   
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::BuildMsgSpecificAttributes
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
inline
void DEC_Knowledge_Object::BuildMsgSpecificAttributes( ASN1T_MsgObjectKnowledgeUpdate& /*asnMsg*/ )
{
    // Nothing
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::NotifyAttributeUpdated
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
inline
void DEC_Knowledge_Object::NotifyAttributeUpdated( E_Attributes nAttribute )
{
    nAttributesUpdated_ |= nAttribute;    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::IsAttributeUpdated
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_Object::IsAttributeUpdated( E_Attributes nAttribute ) const
{
    return ( nAttributesUpdated_ & nAttribute ) == nAttribute;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::IsReconBy
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_Object::IsReconBy( const MIL_AgentType_ABC& agentType ) const
{
    return reconByAgentTypes_.find( &agentType ) != reconByAgentTypes_.end();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::IsRecon
// Created: NLD 2005-01-26
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_Object::IsRecon() const
{
    return !reconByAgentTypes_.empty();
}
