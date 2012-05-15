// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __InteractIfHeightCapacity_h_
#define __InteractIfHeightCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"

namespace xml
{
    class xistream;
}

class MIL_Object_ABC;
class MIL_Agent_ABC;

// =============================================================================
/** @class  InteractIfHeightCapacity
    @brief  InteractIfHeightCapacity
*/
// Created: LDC 2009-03-03
// =============================================================================
class InteractIfHeightCapacity : public ObjectCapacity_ABC
                               , public MIL_InteractiveContainer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             InteractIfHeightCapacity();
    explicit InteractIfHeightCapacity( xml::xistream& xis );
    virtual ~InteractIfHeightCapacity();
    //@}

    //! @name Operations
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    virtual void CanInteractWith( const MIL_Object_ABC& object, const MIL_Agent_ABC& agent, bool& canInteract );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InteractIfHeightCapacity( const InteractIfHeightCapacity& );            //!< Copy constructor
    InteractIfHeightCapacity& operator=( const InteractIfHeightCapacity& ); //!< Assignment operator
    //@}

private:
    double height_;
};

BOOST_CLASS_EXPORT_KEY( InteractIfHeightCapacity )

#endif // __InteractIfHeightCapacity_h_
