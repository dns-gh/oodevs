// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BurnCapacity_h_
#define __BurnCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"

class MIL_PopulationElement_ABC;
class MIL_Agent_ABC;

// =============================================================================
/** @class  BurnCapacity
    @brief  BurnCapacity
*/
// Created: RFT 2008-06-02
// =============================================================================

class BurnCapacity 
    : public ObjectCapacity_ABC
    , public MIL_InteractiveContainer_ABC
{
public:
	//! @name Constructors/Destructor
    //@{
	explicit BurnCapacity( xml::xistream& xis );
	         BurnCapacity();
    virtual ~BurnCapacity();
	//@}

    //! @name Initialize Data
    //@{
    void InitializeData( xml::xistream& xis );
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    virtual void Register( Object& object );
    //@}

	//! @name virtual
    //@{
    virtual void Instanciate( Object& object ) const;
	virtual void Update( Object& object, float time );
    //@}

    //! @name MIL_InteractiveContainer_ABC
    //@{
    //Agent
    virtual void ProcessAgentMovingInside( Object& object, MIL_Agent_ABC& agent );
    virtual void ProcessAgentInside( Object& object, MIL_Agent_ABC& agent );
    //Population
    virtual void ProcessPopulationMovingInside( Object& object, MIL_PopulationElement_ABC& agent );
    //@}


    //! @name Get
    //@{
    //@}    

private:
    //! @name Copy
    //@{
    BurnCapacity( const BurnCapacity& );
    //@}

private:
    //! @name Private member
    //@{
    int injuryID_;
    //@}
};

#endif // __BurnCapacity_h_