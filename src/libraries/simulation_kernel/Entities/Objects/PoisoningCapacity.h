// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __PoisoningCapacity_h_
#define __PoisoningCapacity_h_

#include "MIL.h"
#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"

class MIL_NBCType;
class MIL_Agent_ABC;

// =============================================================================
/** @class  PoisoningCapacity
    @brief  PoisoningCapacity
*/
// Created: RFT 2008-06-02
// =============================================================================
class PoisoningCapacity  
    : public ObjectCapacity_ABC
    , public MIL_InteractiveContainer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PoisoningCapacity( xml::xistream& xis );
             PoisoningCapacity();
    virtual ~PoisoningCapacity();
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

    //! @name 
    //@{
    virtual void ProcessAgentInside( Object& object, MIL_Agent_ABC& agent );
    virtual void ProcessAgentEntering( Object& object, MIL_Agent_ABC& agent );
    virtual void ProcessAgentMovingInside( Object& object, MIL_Agent_ABC& agent );
    virtual void ProcessPopulationMovingInside( Object& object, MIL_PopulationElement_ABC& agent );
    
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PoisoningCapacity( const PoisoningCapacity& );  //!< Copy constructor
    //@}

private:
    //! @name 
    //@{
    int injuryID_;
    //@}
};

#endif // __PoisoningCapacity_h_
