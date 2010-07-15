// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TimeLimitedCapacity_h_
#define __TimeLimitedCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"

class Object;

// =============================================================================
/** @class  ProtectionCapacity
    @brief  ProtectionCapacity
*/
// Created: JCR 2008-06-02
// =============================================================================
class TimeLimitedCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             TimeLimitedCapacity();
    explicit TimeLimitedCapacity( xml::xistream& xis );
    virtual ~TimeLimitedCapacity();
    //@}

    //! @name Operations
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    virtual void Update( MIL_Object_ABC& object, unsigned int time );
    //@}

private:
    //! @name Copy
    //@{
    TimeLimitedCapacity( const TimeLimitedCapacity& from );
    //@}
};

BOOST_CLASS_EXPORT_KEY( TimeLimitedCapacity )

#endif
