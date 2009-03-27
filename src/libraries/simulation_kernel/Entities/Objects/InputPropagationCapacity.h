// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InputPropagationCapacity_h_
#define __InputPropagationCapacity_h_

#include "PropagationCapacity_ABC.h"

class InputPropagationCapacity : public PropagationCapacity_ABC    
{
public:
	//! @name Constructors/Destructor
    //@{
	explicit InputPropagationCapacity( xml::xistream& xis );
	         InputPropagationCapacity();
    virtual ~InputPropagationCapacity();
	//@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    virtual void Register( Object& /*object*/ );
    //@}

	//! @name virtual
    //@{
    virtual void Instanciate( Object& object ) const;
	virtual void Update( Object& object, uint time );
	//@}

private:
    //! @name Copy
    //@{
    InputPropagationCapacity( const InputPropagationCapacity& from );
    //@}

private:
    //! @name 
    //@{
    std::string model_;
    //@}
};

#endif