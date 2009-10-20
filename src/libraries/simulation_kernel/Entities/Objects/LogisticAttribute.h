// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticAttribute_h_
#define __LogisticAttribute_h_

#include "ObjectAttribute_ABC.h"

class MIL_AutomateLOG;

class LogisticAttribute : public ObjectAttribute_ABC    
{
public:
	//! @name Constructors/Destructor
    //@{
	         LogisticAttribute();
    explicit LogisticAttribute( xml::xistream& xis );
    explicit LogisticAttribute( const ASN1T_ObjectAttributes& asn );
	virtual ~LogisticAttribute();
	//@}

    //! @name Knowledge
    //@{
    void Instanciate( DEC_Knowledge_Object& object ) const;
    void SendFullState( ASN1T_ObjectAttributes& asn ) const;
    void SendUpdate( ASN1T_ObjectAttributes& asn ) const;
    void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

	//! @name virtual
    //@{
    MIL_AutomateLOG& GetTC2() const;
	//@}

    //! @name Copy
    //@{
    LogisticAttribute& operator=( const LogisticAttribute& ); //!< Assignment operator
    //@}

private:
    //! @name Copy
    //@{
    LogisticAttribute( const LogisticAttribute& from );
    //@}

private:
    //! @name 
    //@{
    MIL_AutomateLOG*    pTC2_;
    //@}
};

#endif //__LogisticAttribute_h_
