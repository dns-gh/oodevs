// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __DelayCapacity_h_
#define __DelayCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  DelayCapacity
    @brief  DelayCapacity
*/
// Created: JSR 2010-07-06
// =============================================================================
class DelayCapacity : public ObjectCapacity_ABC
                    , public MIL_InteractiveContainer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DelayCapacity();
    explicit DelayCapacity( xml::xistream& xis );
    virtual ~DelayCapacity();
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
    DelayCapacity( const DelayCapacity& );            //!< Copy constructor
    DelayCapacity& operator=( const DelayCapacity& ); //!< Assignment operator
    //@}

private:
    //! @name Members
    //@{
    std::map< const MIL_Agent_ABC* , int > agentInsideMap_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DelayCapacity )

#endif // __DelayCapacity_h_
