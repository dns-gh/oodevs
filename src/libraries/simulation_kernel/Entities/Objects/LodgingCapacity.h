// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __LodgingCapacity_h_
#define __LodgingCapacity_h_

#include "ObjectCapacity_ABC.h"

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  LodgingCapacity
    @brief  LodgingCapacity
*/
// Created: MMC 2012-11-16
// =============================================================================
class LodgingCapacity : public ObjectCapacity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             LodgingCapacity();
             LodgingCapacity( const LodgingCapacity& );
    explicit LodgingCapacity( xml::xistream& xis );
    virtual ~LodgingCapacity();
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
    LodgingCapacity& operator=( const LodgingCapacity& ); //!< Assignment operator
    //@}
};

BOOST_CLASS_EXPORT_KEY( LodgingCapacity )

#endif // __LodgingCapacity_h_
