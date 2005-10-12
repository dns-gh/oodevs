// *****************************************************************************
//
// $Created: NLD 2004-05-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectCollision.inl $
// $Author: Jvt $
// $Modtime: 16/03/05 19:36 $
// $Revision: 4 $
// $Workfile: DEC_Knowledge_ObjectCollision.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCollision::IsValid
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_ObjectCollision::IsValid() const
{
    return bIsValid_;    
}


// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCollision::GetObject
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC& DEC_Knowledge_ObjectCollision::GetObject() const
{
    assert( pObject_ );
    return *pObject_;    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCollision::GetAgentColliding
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
inline
const MIL_AgentPion& DEC_Knowledge_ObjectCollision::GetAgentColliding() const
{
    assert( pAgentColliding_ );
    return *pAgentColliding_;   
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCollision::GetPosition
// Created: NLD 2004-11-03
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& DEC_Knowledge_ObjectCollision::GetPosition() const
{
    return vPosition_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectCollision::Clean
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_ObjectCollision::Clean()
{
    return !IsValid();
}
