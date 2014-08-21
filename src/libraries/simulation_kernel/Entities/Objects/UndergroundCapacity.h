// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __UndergroundCapacity_h_
#define __UndergroundCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "MIL_StructuralStateNotifier_ABC.h"

namespace xml
{
    class xistream;
}

class MIL_UrbanObject_ABC;
class UndergroundAttribute;

// =============================================================================
/** @class  UndergroundCapacity
    @brief  UndergroundCapacity
*/
// Created: JSR 2011-05-30
// =============================================================================
class UndergroundCapacity : public ObjectCapacity_ABC
                          , public MIL_StructuralStateNotifier_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UndergroundCapacity();
    explicit UndergroundCapacity( xml::xistream& xis );
    virtual ~UndergroundCapacity();
    //@}

    //! @name Operations
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    virtual void Finalize( MIL_Object_ABC& object );

    virtual void NotifyStructuralStateChanged( float structuralState, const MIL_Object_ABC& object );
    virtual void NotifyFired() {}

    void RegisterUrbanBlock( MIL_Object_ABC& object );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UndergroundCapacity( const UndergroundCapacity& );            //!< Copy constructor
    UndergroundCapacity& operator=( const UndergroundCapacity& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    MIL_UrbanObject_ABC* urbanBlock_;
    UndergroundAttribute* attribute_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( UndergroundCapacity )

#endif // __UndergroundCapacity_h_
