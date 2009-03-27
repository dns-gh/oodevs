// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __IntoxicationCapacity_h_
#define __IntoxicationCapacity_h_

#include "MIL.h"
#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"

class MIL_Agent_ABC;

// =============================================================================
/** @class  IntoxicationCapacity
    @brief  IntoxicationCapacity
*/
// Created: JCR 2008-06-02
// =============================================================================
class IntoxicationCapacity  
    : public ObjectCapacity_ABC
    , public MIL_InteractiveContainer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit IntoxicationCapacity( xml::xistream& xis );
             IntoxicationCapacity();
    virtual ~IntoxicationCapacity();
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

    //! @name 
    //@{
    virtual void ProcessAgentInside( Object& object, MIL_Agent_ABC& agent );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IntoxicationCapacity( const IntoxicationCapacity& );  //!< Copy constructor
    //@}

private:
    //! @name 
    //@{
    std::string  type_;
    int          maxToxic_;
    //@}
};

#endif // __IntoxicationCapacity_h_
