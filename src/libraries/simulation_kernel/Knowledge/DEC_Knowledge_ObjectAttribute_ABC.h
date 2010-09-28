// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DEC_Knowledge_ObjectAttribute_ABC_h_
#define __DEC_Knowledge_ObjectAttribute_ABC_h_

namespace Common
{
    class ObjectAttributes;
}

class PHY_PerceptionLevel;
class DEC_Knowledge_Object;
class DEC_Knowledge_ObjectPerception;
class DEC_Knowledge_ObjectCollision;

// =============================================================================
// Created: NLD 2004-05-04
// =============================================================================
class DEC_Knowledge_ObjectAttribute_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_ObjectAttribute_ABC() {}
    virtual ~DEC_Knowledge_ObjectAttribute_ABC() {}
    //@}

    //! @name CheckPoints
    //@{
    virtual void UpdateOnPerceptionLevel( const PHY_PerceptionLevel& currentPerceptionLevel ) = 0;
    virtual void UpdateOnPerception( const DEC_Knowledge_ObjectPerception& perception ) = 0;
    virtual void UpdateOnCollision( const DEC_Knowledge_ObjectCollision& collision  ) = 0;
    virtual void Send( Common::ObjectAttributes& asn ) const = 0;
    //@}

    //! @name CheckPoint / boost deserialize
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int ) {}
    virtual void Register( DEC_Knowledge_Object& knObject ) = 0;
    //@}
};

#endif // __DEC_Knowledge_ObjectAttribute_ABC_h_
