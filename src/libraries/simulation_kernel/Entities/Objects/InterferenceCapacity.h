// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InterferenceCapacity_h_
#define __InterferenceCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"

// =============================================================================
/** @class  InterferenceCapacity
    @brief  InterferenceCapacity
*/
// Created: RFT 2008-05-30
// =============================================================================
class InterferenceCapacity : public ObjectCapacity_ABC
                           , public MIL_InteractiveContainer_ABC
{
public:
	//! @name Constructors/Destructor
    //@{
	         InterferenceCapacity();
    explicit InterferenceCapacity( xml::xistream& xis );
    virtual ~InterferenceCapacity();
	//@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    virtual void Instanciate( Object& object ) const;
    virtual void Register( Object& object );
    virtual void ProcessAgentInside( Object& object, MIL_Agent_ABC& agent );
    virtual void ProcessAgentExiting( Object& object, MIL_Agent_ABC& agent );    
    //@}

private:
    //! @name Copy
    //@{
    InterferenceCapacity( const InterferenceCapacity& from );
    //@}
};

#endif