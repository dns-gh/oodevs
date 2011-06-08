// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Formation_h_
#define __Formation_h_

#include <boost/noncopyable.hpp>

namespace hla
{
    class UpdateFunctor_ABC;
}

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  Formation
    @brief  Formation
*/
// Created: AGE 2008-02-25
// =============================================================================
class Formation : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Formation( bool isOnRoad );
    virtual ~Formation();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive )
    {
        archive << formation_;
    }
    //@}

private:
    //! @name Member data
    //@{
    long formation_;
    //@}
};

}
}

#endif // __Formation_h_
