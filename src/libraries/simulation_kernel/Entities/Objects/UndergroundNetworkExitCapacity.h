// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __UndergroundNetworkExitCapacity_h_
#define __UndergroundNetworkExitCapacity_h_

#include "ObjectCapacity_ABC.h"

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  UndergroundNetworkExitCapacity
    @brief  UndergroundNetworkExitCapacity
*/
// Created: JSR 2011-05-30
// =============================================================================
class UndergroundNetworkExitCapacity : public ObjectCapacity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             UndergroundNetworkExitCapacity();
    explicit UndergroundNetworkExitCapacity( xml::xistream& xis );
    virtual ~UndergroundNetworkExitCapacity();
    //@}

    //! @name Operations
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UndergroundNetworkExitCapacity( const UndergroundNetworkExitCapacity& );            //!< Copy constructor
    UndergroundNetworkExitCapacity& operator=( const UndergroundNetworkExitCapacity& ); //!< Assignment operator
    //@}

};

BOOST_CLASS_EXPORT_KEY( UndergroundNetworkExitCapacity )

#endif // __UndergroundNetworkExitCapacity_h_
