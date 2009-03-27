// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FirePropagationCapacity_h_
#define __FirePropagationCapacity_h_

#include "PropagationCapacity_ABC.h"

class MIL_PropagationManager;

class FirePropagationCapacity : public PropagationCapacity_ABC    
{
public:
	//! @name Constructors/Destructor
    //@{
	explicit FirePropagationCapacity( xml::xistream& xis, MIL_PropagationManager& manager );
	         FirePropagationCapacity();
    virtual ~FirePropagationCapacity();
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
    FirePropagationCapacity( const FirePropagationCapacity& from );
    //@}

	//! @name Helpers
    //@{
    int  UpdateState( Object& object, uint time );
	void Propagate( Object& object );
	void CheckPropagation( const MT_Vector2D& vOrigin, Object& object );
    //@}
	
	//! @name Helpers
    //@{
	TER_Localisation GetLocalisation( const MT_Vector2D& pt ) const;
	//@}

private:
	//! @name 
    //@{
	MIL_PropagationManager* pManager_;
	//@}

    //! @name 
    //@{
    //Time of creation, and time of death of the fire
    uint   timeOfCreation_;
    uint   timeOfDeath_;
    //@}
};

#endif