// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __DEC_Knowledge_IObjectAttributeProxy_h_
#define __DEC_Knowledge_IObjectAttributeProxy_h_

namespace Common
{
    class ObjectAttributes;
}

class PHY_PerceptionLevel;
class DEC_Knowledge_Object;
class DEC_Knowledge_ObjectPerception;
class DEC_Knowledge_ObjectCollision;
class DEC_Knowledge_ObjectMagicPerception;

// =============================================================================
// Created: NLD 2010-10-26
// =============================================================================
class DEC_Knowledge_IObjectAttributeProxy : private boost::noncopyable
{
public:
    //! @name Updates
    //@{
    virtual bool UpdateOnPerceptionLevel( const DEC_Knowledge_Object& knowledge, const DEC_Knowledge_ObjectMagicPerception& magicPerception ) = 0;
    virtual bool UpdateOnPerception     ( const DEC_Knowledge_Object& knowledge, const DEC_Knowledge_ObjectPerception& perception ) = 0;
    virtual bool UpdateOnCollision      ( const DEC_Knowledge_Object& knowledge, const DEC_Knowledge_ObjectCollision& collision  ) = 0;
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState( Common::ObjectAttributes& asn ) const = 0;
    virtual void SendFullState   ( Common::ObjectAttributes& asn ) const = 0;
    //@}
    
    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int ) {} 
    virtual void Register( DEC_Knowledge_Object& knowledge ) = 0;
    //@}
};

#endif // __DEC_Knowledge_IObjectAttributeProxy_h_
