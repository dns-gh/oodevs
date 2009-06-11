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
    template< typename Archive > void serialize( Archive&, const uint );
    virtual void Instanciate( Object& object ) const;
    virtual void Register( Object& object );
    bool AddAnimator( Object& object, const MIL_Agent_ABC& agent );
    void ReleaseAnimator( Object& object, const MIL_Agent_ABC& agent );
    //@}

private:
    //! @name Copy
    //@{
    WorkableCapacity( const WorkableCapacity& );
    //@}

private:
    //! @name Member data
    //@{
    uint maxAnimators_;
    //@}
};

#endif // __WorkableCapacity_h_
