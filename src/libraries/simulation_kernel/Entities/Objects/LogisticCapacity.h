// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __LogisticCapacity_h_
#define __LogisticCapacity_h_

#include "ObjectCapacity_ABC.h"

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  LogisticCapacity
    @brief  LogisticCapacity
*/
// Created: MMC 2012-11-16
// =============================================================================
class LogisticCapacity : public ObjectCapacity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticCapacity();
             LogisticCapacity( const LogisticCapacity& );
    explicit LogisticCapacity( xml::xistream& xis );
    virtual ~LogisticCapacity();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void serialize( Archive&, const unsigned int );
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogisticCapacity& operator=( const LogisticCapacity& ); //!< Assignment operator
    //@}
};

BOOST_CLASS_EXPORT_KEY( LogisticCapacity )

#endif // __LogisticCapacity_h_
