// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef PostureTime_ABC_h
#define PostureTime_ABC_h

#include <boost/noncopyable.hpp>

class PHY_Posture;

namespace posture
{
// =============================================================================
/** @class  PostureTime_ABC
    @brief  PostureTime definition
*/
// Created: SLI 2013-01-23
// =============================================================================
class PostureTime_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PostureTime_ABC() {}
    virtual ~PostureTime_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual double GetPostureSetupTime( const PHY_Posture& posture ) const = 0;
    virtual double GetPostureTearDownTime( const PHY_Posture& posture ) const = 0;
    //@}
};

}

#endif // PostureTime_ABC_h
