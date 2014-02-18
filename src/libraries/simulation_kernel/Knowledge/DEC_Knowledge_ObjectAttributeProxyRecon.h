// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DEC_Knowledge_ObjectAttributeProxyRecon_h_
#define __DEC_Knowledge_ObjectAttributeProxyRecon_h_

#include "DEC_Knowledge_ObjectAttributeProxy_ABC.h"
#include "DEC_Knowledge_Object.h"
#include "DEC_Knowledge_ObjectPerception.h"
#include "DEC_Knowledge_ObjectCollision.h"
#include "Entities/Agents/MIL_Agent_ABC.h"

// =============================================================================
// Created: NLD 2010-10-26
// =============================================================================
template< typename T >
class DEC_Knowledge_ObjectAttributeProxyRecon : public DEC_Knowledge_ObjectAttributeProxy_ABC< T >
{
public:
    //! @name Updates
    //@{
    virtual bool UpdateOnPerceptionLevel( const DEC_Knowledge_Object& knowledge, const DEC_Knowledge_ObjectMagicPerception& magicPerception );
    virtual bool UpdateOnPerception     ( const DEC_Knowledge_Object& knowledge, const DEC_Knowledge_ObjectPerception& perception );
    virtual bool UpdateOnCollision      ( const DEC_Knowledge_Object& knowledge, const DEC_Knowledge_ObjectCollision& collision  );
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive& ar, const unsigned int )
    {
        ar & boost::serialization::base_object< DEC_Knowledge_ObjectAttributeProxy_ABC< T > >( *this );
    }
    //@}
};

// =============================================================================
// Implementation
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeProxyRecon::UpdateOnPerceptionLevel
// Created: NLD 2010-10-27
// -----------------------------------------------------------------------------
template< typename T >
bool DEC_Knowledge_ObjectAttributeProxyRecon< T >::UpdateOnPerceptionLevel( const DEC_Knowledge_Object& /*knowledge*/, const DEC_Knowledge_ObjectMagicPerception& /*magicPerception*/ )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeProxyRecon::UpdateOnPerception
// Created: NLD 2010-10-27
// -----------------------------------------------------------------------------
template< typename T >
bool DEC_Knowledge_ObjectAttributeProxyRecon< T >::UpdateOnPerception( const DEC_Knowledge_Object& knowledge, const DEC_Knowledge_ObjectPerception& perception )
{
    if( !knowledge.IsReconBy( perception.GetAgentPerceiving().GetType() ) )
        return false;
    return UpdateAttributeFromSource( perception );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeProxyRecon::UpdateOnCollision
// Created: NLD 2010-10-27
// -----------------------------------------------------------------------------
template< typename T >
bool DEC_Knowledge_ObjectAttributeProxyRecon< T >::UpdateOnCollision( const DEC_Knowledge_Object& knowledge, const DEC_Knowledge_ObjectCollision& collision )
{
    if( !collision.GetAgentColliding() || !knowledge.IsReconBy( collision.GetAgentColliding()->GetType() ) )
        return false;
    return UpdateAttributeFromSource( collision );
}

#endif // __DEC_Knowledge_ObjectAttributeProxyRecon_h_
