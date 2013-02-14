// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __BorderCapacity_h_
#define __BorderCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"
#include "tools/Set.h"

namespace xml
{
    class xistream;
}

class MIL_Population;

// =============================================================================
/** @class  BorderCapacity
    @brief  BorderCapacity
*/
// Created: JSR 2011-08-31
// =============================================================================
class BorderCapacity : public ObjectCapacity_ABC
                     , public MIL_InteractiveContainer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             BorderCapacity();
    explicit BorderCapacity( xml::xistream& xis );
    virtual ~BorderCapacity();
    //@}

    //! @name Operations
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    virtual void Update( MIL_Object_ABC& object, unsigned int time );
    //@}

    //! @name From MIL_InteractiveContainer_ABC
    //@{
    virtual void ProcessAgentEntering( MIL_Object_ABC& object, MIL_Agent_ABC& agent );
    virtual void ProcessPopulationInside( MIL_Object_ABC& object, MIL_PopulationElement_ABC& population );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    BorderCapacity( const BorderCapacity& );            //!< Copy constructor
    BorderCapacity& operator=( const BorderCapacity& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    tools::Set< MIL_Population* > populationsInside_;
    tools::Set< MIL_Population* > populationsNotified_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( BorderCapacity )

#endif // __BorderCapacity_h_
