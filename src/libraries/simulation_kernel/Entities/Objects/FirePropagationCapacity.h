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

// =============================================================================
/** @class  FirePropagationCapacity
    @brief  FirePropagationCapacity
*/
// Created: RFT 2008-05-30
// =============================================================================
class FirePropagationCapacity : public PropagationCapacity_ABC    
{
public:
	//! @name Constructors/Destructor
    //@{
	         FirePropagationCapacity();
    explicit FirePropagationCapacity( xml::xistream& xis, MIL_PropagationManager& manager );
    virtual ~FirePropagationCapacity();
	//@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

	//! @name Operations
    //@{
    virtual void Instanciate( Object& object ) const;
    virtual void Register( Object& object );
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
	TER_Localisation GetLocalisation( const MT_Vector2D& pt ) const;
	//@}

private:
	//! @name Member data
    //@{
	MIL_PropagationManager* pManager_;
    //Time of creation, and time of death of the fire
    uint   timeOfCreation_;
    uint   timeOfDeath_;
    //@}
};

#endif