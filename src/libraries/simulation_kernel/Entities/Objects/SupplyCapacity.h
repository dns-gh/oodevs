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

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  SupplyCapacity
    @brief  SupplyCapacity
*/
// Created: MGD 2009-03-05
// =============================================================================
class SupplyCapacity : public ObjectCapacity_ABC
                     , public MIL_InteractiveContainer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyCapacity();
    explicit SupplyCapacity( xml::xistream& xis );
    virtual ~SupplyCapacity();
    //@}

     //! @name Operations
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    virtual void ProcessAgentEntering( MIL_Object_ABC& object, MIL_Agent_ABC& agent );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SupplyCapacity( const SupplyCapacity& );  //!< Copy constructor
    //@}
};

BOOST_CLASS_EXPORT_KEY( SupplyCapacity )

#endif // __SupplyCapacity_h_
