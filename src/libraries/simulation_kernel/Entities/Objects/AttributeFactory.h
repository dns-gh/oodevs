// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AttributeFactory_h_
#define __AttributeFactory_h_

class Object;

#include "ObjectComponentRegistry_ABC.h"

class AttributeFactory : public ObjectComponentRegistry_ABC< Object >
{
public:	
	//! @name Constructors/Destructor
    //@{
			AttributeFactory();
	virtual ~AttributeFactory();
	//@}

    //! @name 
    //@{    
    void Create( Object& object, const std::string& attribute, xml::xistream& xis ) const;
    void Create( Object& object, const ASN1T_ObjectAttributes& asn ) const;
    //@}

private:
    //! @name 
    //@{
    void Register( const std::string& attribute, const T_CallBack& callback );
    //@}

private:
	//! @name Members
    //@{
	T_CallBacks callbacks_;
	//@}
};

#endif