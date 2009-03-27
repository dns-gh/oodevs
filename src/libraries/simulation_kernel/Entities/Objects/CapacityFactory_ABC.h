// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CapacityFactory_ABC_h_
#define __CapacityFactory_ABC_h_

class ObjectPrototype;

namespace xml
{
    class xistream;
}

class CapacityFactory_ABC
{
public:	
	//! @name Constructors/Destructor
    //@{
			CapacityFactory_ABC() {}
	virtual ~CapacityFactory_ABC() {}
	//@}	

	//! @name Registrer
    //@{
	virtual void Create( ObjectPrototype& protoype, const std::string& capacity, xml::xistream& xis ) = 0;
	//@}
};

#endif