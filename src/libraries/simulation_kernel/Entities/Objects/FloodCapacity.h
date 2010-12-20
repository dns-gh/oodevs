// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __FloodCapacity_h_
#define __FloodCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  FloodCapacity
    @brief  FloodCapacity
*/
// Created: JSR 2010-12-15
// =============================================================================
class FloodCapacity : public ObjectCapacity_ABC
                    , public MIL_InteractiveContainer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FloodCapacity();
    explicit FloodCapacity( xml::xistream& xis );
    virtual ~FloodCapacity();
    //@}

    //! @name Operations
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    virtual void CanInteractWith( const MIL_Object_ABC& object, const MIL_Agent_ABC& agent, bool& canInteract );
    virtual void PreprocessAgent( MIL_Object_ABC& object, MIL_Agent_ABC& agent );
    virtual void ProcessAgentExiting( MIL_Object_ABC& object, MIL_Agent_ABC& agent );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FloodCapacity( const FloodCapacity& );            //!< Copy constructor
    FloodCapacity& operator=( const FloodCapacity& ); //!< Assignment operator
    //@}
};

BOOST_CLASS_EXPORT_KEY( FloodCapacity )

#endif // __FloodCapacity_h_
