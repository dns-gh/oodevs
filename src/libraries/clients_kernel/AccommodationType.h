// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __AccommodationType_h_
#define __AccommodationType_h_

#include <boost/noncopyable.hpp>

namespace kernel
{
// =============================================================================
/** @class  AccommodationType
    @brief  AccommodationType
*/
// Created: LDC 2011-03-25
// =============================================================================
class AccommodationType : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             AccommodationType( const std::string& role, float nominalCapacity, float maxCapacity );
    virtual ~AccommodationType();
    //@}

    //! @name Operations
    //@{
    const std::string& GetRole() const;
    float GetNominalCapacity() const;
    float GetMaxCapacity() const;
    //@}

private:
    //! @name Member data
    //@{
    const std::string role_;
    float nominalCapacity_;
    float maxCapacity_;
    //@}
};
}

#endif // __AccommodationType_h_
