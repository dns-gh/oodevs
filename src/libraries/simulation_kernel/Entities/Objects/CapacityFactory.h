// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Object_CapacityFactory_h_
#define __Object_CapacityFactory_h_

#include "ObjectComponentRegistry_ABC.h"
#include "CapacityFactory_ABC.h"

class ObjectPrototype;
class MIL_PropagationManager;

// =============================================================================
/** @class  CapacityFactory
    @brief  CapacityFactory
*/
// Created: JCR 2008-05-30
// =============================================================================
class CapacityFactory : public ObjectComponentRegistry_ABC< ObjectPrototype >
                      , public CapacityFactory_ABC
{
public:	
	//! @name Constructors/Destructor
    //@{
             CapacityFactory();
	virtual ~CapacityFactory();
	//@}	

	//! @name Creation
    //@{
    void Create( ObjectPrototype& object, const std::string& capacity, xml::xistream& xis );
	//@}

private:
    //! @name Registration
    //@{
    void Register( const std::string& capacity, const T_CallBack& callback );
    //@}

private:
	//! @name Members
    //@{
	T_CallBacks callbacks_;
	std::auto_ptr< MIL_PropagationManager > propagation_;
	//@}
};

#endif // __Object_CapacityFactory_h_