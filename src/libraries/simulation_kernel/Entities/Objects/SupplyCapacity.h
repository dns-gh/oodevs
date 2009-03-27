// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __SupplyCapacity_h_
#define __SupplyCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"

// =============================================================================
/** @class  SupplyCapacity
    @brief  SupplyCapacity
*/
// Created: MGD 2009-03-05
// =============================================================================
class SupplyCapacity
    : public ObjectCapacity_ABC
    , public MIL_InteractiveContainer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit SupplyCapacity( xml::xistream& xis );
             SupplyCapacity();
    virtual ~SupplyCapacity();
    //@}

     //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );    
    virtual void Register( Object& /*object*/ );
    //@}

	//! @name virtual
    //@{
    virtual void Instanciate( Object& object ) const;	
	//@}

    //! @name Operations on agent 
    //@{
    virtual void ProcessAgentEntering( Object& object, MIL_Agent_ABC& agent );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SupplyCapacity( const SupplyCapacity& );  //!< Copy constructor
    //@}
};

#endif // __SupplyCapacity_h_
