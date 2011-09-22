// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __AccommodationTypes_h_
#define __AccommodationTypes_h_

#include "tools/Resolver.h"
#include <boost/noncopyable.hpp>

namespace tools
{
    class ExerciseConfig;
}

namespace xml
{
    class xistream;
}

namespace kernel
{
    class AccommodationType;

// =============================================================================
/** @class  AccommodationTypes
    @brief  AccommodationTypes
*/
// Created: LDC 2011-03-24
// =============================================================================
class AccommodationTypes : public tools::StringResolver< AccommodationType >
                         , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             AccommodationTypes();
    virtual ~AccommodationTypes();
    //@}

    //! @name Operations
    //@{
    void Load( const tools::ExerciseConfig& config );
    void Purge();
    //@}

private:
    //! @name Helpers
    //@{
    void ReadAccommodations( xml::xistream& xis );
    void ReadAccommodation( xml::xistream& xis );
    //@}
};

}

#endif // __AccommodationTypes_h_
