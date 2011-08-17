// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SpawnObjectCapacity_h_
#define __SpawnObjectCapacity_h_

#include "ObjectCapacity_ABC.h"

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  SpawnObjectCapacity
    @brief  Spawn object capacity
*/
// Created: LGY 2011-08-17
// =============================================================================
class SpawnObjectCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SpawnObjectCapacity();
    explicit SpawnObjectCapacity( xml::xistream& xis );
    virtual ~SpawnObjectCapacity();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive >
    void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    virtual void Finalize( MIL_Object_ABC& object );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SpawnObjectCapacity( const SpawnObjectCapacity& ); //!< Copy constructor
    //@}

private:
    //! @name Member data
    //@{
    std::string type_;
    //@}
};

#endif // __SpawnObjectCapacity_h_
