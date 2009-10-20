// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TimeLimitedAttribute_h_
#define __TimeLimitedAttribute_h_

#include "ObjectAttribute_ABC.h"

class Object;

class TimeLimitedAttribute
    : public ObjectAttribute_ABC
{
public:
	//! @name Constructors/Destructor
    //@{
    	     TimeLimitedAttribute();
    explicit TimeLimitedAttribute( xml::xistream& xis );
    explicit TimeLimitedAttribute( uint nActivityTime );
    explicit TimeLimitedAttribute( const ASN1T_ObjectAttributes& asn );
    virtual ~TimeLimitedAttribute();
	//@}

    //! @name CheckPoints
    //@{    
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name ODB
    //@{
    void WriteODB( xml::xostream& /*xos*/ ) const;    
    //@}

    //! @name Network updates
    //@{
    void SendFullState( ASN1T_ObjectAttributes& asn ) const;
    void SendUpdate( ASN1T_ObjectAttributes& asn ) const;
    //@}

    //! @name Accessors
    //@{
    bool IsTimeOver( uint time ) const;
    //@}

    //! @name Copy
    //@{
    TimeLimitedAttribute& operator=( const TimeLimitedAttribute& ); //!< Assignment operator
    //@}

private:
    //! @name Copy
    //@{
    TimeLimitedAttribute( const TimeLimitedAttribute& from );
    //@}

private:
    //! @name 
    //@{
    uint nActivityTime_;
    mutable uint nDeathTimeStep_;
    //@}
};

#endif
