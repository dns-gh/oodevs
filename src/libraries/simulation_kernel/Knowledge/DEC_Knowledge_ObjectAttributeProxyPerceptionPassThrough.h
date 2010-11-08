// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __DEC_Knowledge_ObjectAttributeProxyPerceptionPassThrough_h_
#define __DEC_Knowledge_ObjectAttributeProxyPerceptionPassThrough_h_

#include "DEC_Knowledge_ObjectAttributeProxy_ABC.h"
#include "DEC_Knowledge_ObjectMagicPerception.h"
#include "DEC_Knowledge_ObjectPerception.h"
#include "DEC_Knowledge_ObjectCollision.h"

// =============================================================================
// Created: NLD 2010-10-26
// =============================================================================
template< typename T >
class DEC_Knowledge_ObjectAttributeProxyPerceptionPassThrough : public DEC_Knowledge_ObjectAttributeProxy_ABC< T >
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
// Name: DEC_Knowledge_ObjectAttributeProxyPerceptionPassThrough::UpdateOnPerceptionLevel
// Created: NLD 2010-10-27
// -----------------------------------------------------------------------------
template< typename T >
bool DEC_Knowledge_ObjectAttributeProxyPerceptionPassThrough< T >::UpdateOnPerceptionLevel( const DEC_Knowledge_Object& /*knowledge*/, const DEC_Knowledge_ObjectMagicPerception& magicPerception )
{
    return UpdateAttributeFromSource( magicPerception );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeProxyPerceptionPassThrough::UpdateOnPerception
// Created: NLD 2010-10-27
// -----------------------------------------------------------------------------
template< typename T >
bool DEC_Knowledge_ObjectAttributeProxyPerceptionPassThrough< T >::UpdateOnPerception( const DEC_Knowledge_Object& /*knowledge*/, const DEC_Knowledge_ObjectPerception& perception )
{
    return UpdateAttributeFromSource( perception );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeProxyPerceptionPassThrough::UpdateOnCollision
// Created: NLD 2010-10-27
// -----------------------------------------------------------------------------
template< typename T >
bool DEC_Knowledge_ObjectAttributeProxyPerceptionPassThrough< T >::UpdateOnCollision( const DEC_Knowledge_Object& /*knowledge*/, const DEC_Knowledge_ObjectCollision& /*collision*/ )
{
    return false;
}

#endif // __DEC_Knowledge_ObjectAttributeProxyPassThrough_h_
