// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DEC_Knowledge_ABC_h_
#define __DEC_Knowledge_ABC_h_

#include <boost/serialization/export.hpp>

// =============================================================================
/** @class  DEC_Knowledge_ABC
    @brief  The base class for a knowledge
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_ABC();
    virtual ~DEC_Knowledge_ABC();
    //@}

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive& /*file*/, const unsigned int /*version*/ )
    {
        // NOTHING
    }
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_ABC )

#endif // __DEC_Knowledge_ABC_h_
