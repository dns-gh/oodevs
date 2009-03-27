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

#include "Entities/Objects/MIL_ObjectType_ABC.h"

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::GetID
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
inline
uint DEC_Knowledge_Object::GetID() const
{
    return nID_;    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::GetArmy
// Created: NLD 2006-01-05
// -----------------------------------------------------------------------------
inline
const MIL_Army_ABC& DEC_Knowledge_Object::GetArmy() const
{
    assert( pOwnerArmy_ );
    return *pOwnerArmy_;
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
const MIL_ObjectType_ABC& DEC_Knowledge_Object::GetType() const
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
MIL_Object_ABC* DEC_Knowledge_Object::GetObjectKnown() const
{
    return pObjectKnown_;
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

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::Clean
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
inline
bool DEC_Knowledge_Object::Clean() const
{
    return GetRelevance() <= 0.;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::GetArmyKnowing
// Created: NLD 2007-02-07
// -----------------------------------------------------------------------------
inline
const MIL_Army_ABC& DEC_Knowledge_Object::GetArmyKnowing() const
{
    assert( pArmyKnowing_ );
    return *pArmyKnowing_;
}

// -----------------------------------------------------------------------------
// Name: template< typename Extension >  void DEC_Knowledge_Object::Attach
// Created: JCR 2008-08-12
// -----------------------------------------------------------------------------
template< typename Extension >  
void DEC_Knowledge_Object::Attach( Extension& extension )
{
    attributes_.push_back( &extension );
    tools::Extendable< DEC_Knowledge_ObjectAttribute_ABC >::Attach( extension );
}

// -----------------------------------------------------------------------------
// Name: template< typename Extension >  const Extension* DEC_Knowledge_Object::Retrieve
// Created: JCR 2008-08-12
// -----------------------------------------------------------------------------
template< typename Extension >  
const Extension* DEC_Knowledge_Object::Retrieve() const
{
    return tools::Extendable< DEC_Knowledge_ObjectAttribute_ABC >::Retrieve< Extension >();
}
