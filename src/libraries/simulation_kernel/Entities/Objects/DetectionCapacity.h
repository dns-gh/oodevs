// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __DetectionCapacity_h_
#define __DetectionCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"

// =============================================================================
/** @class  DetectionCapacity
    @brief  DetectionCapacity
*/
// Created: MGD 2009-03-05
// =============================================================================
class DetectionCapacity : public ObjectCapacity_ABC
                        , public MIL_InteractiveContainer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DetectionCapacity();
    explicit DetectionCapacity( xml::xistream& xis );
    virtual ~DetectionCapacity();
    //@}

     //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );    
    //@}

	//! @name Operations
    //@{
    virtual void Instanciate( Object& object ) const;	
    virtual void Register( Object& object );
    virtual void ProcessAgentInside( Object& object, MIL_Agent_ABC& agent );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DetectionCapacity( const DetectionCapacity& );  //!< Copy constructor
    //@}
};

#endif // __DetectionCapacity_h_
