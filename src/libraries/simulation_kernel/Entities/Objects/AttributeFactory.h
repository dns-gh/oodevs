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

#include "ObjectComponentRegistry_ABC.h"

class Object;

// =============================================================================
/** @class  AttributeFactory
    @brief  AttributeFactory
*/
// Created: JCR 2008-05-30
// =============================================================================
class AttributeFactory : public ObjectComponentRegistry_ABC< Object >
{
public:	
	//! @name Constructors/Destructor
    //@{
			 AttributeFactory();
	virtual ~AttributeFactory();
	//@}

    //! @name Operations
    //@{    
    void Create( Object& object, const std::string& attribute, xml::xistream& xis ) const;
    void Create( Object& object, const ASN1T_ObjectAttributes& asn ) const;
    //@}

private:
    //! @name Helpers
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