// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __WorkableCapacity_h_
#define __WorkableCapacity_h_

#include "ObjectCapacity_ABC.h"

namespace xml
{
    class xistream;
}

class MIL_Agent_ABC;

// =============================================================================
/** @class  WorkableCapacity
    @brief  WorkableCapacity
*/
// Created: JCR 2008-05-30
// =============================================================================
class WorkableCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             WorkableCapacity();
    explicit WorkableCapacity( xml::xistream& xis );
    virtual ~WorkableCapacity();
    //@}

    //! @name Operations
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    bool AddAnimator( MIL_Object_ABC& object, const MIL_Agent_ABC& agent );
    void ReleaseAnimator( MIL_Object_ABC& object, const MIL_Agent_ABC& agent );
    //@}

private:
    //! @name Copy
    //@{
    WorkableCapacity( const WorkableCapacity& );
    //@}

private:
    //! @name Member data
    //@{
    unsigned int maxAnimators_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( WorkableCapacity )

#endif // __WorkableCapacity_h_
