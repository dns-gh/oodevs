// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Dimension_h_
#define __Dimension_h_

#include <boost/noncopyable.hpp>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  Dimension
    @brief  Dimension
*/
// Created: AGE 2008-02-21
// =============================================================================
class Dimension : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Dimension( bool isOnRoad );
    virtual ~Dimension();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive )
    {
        archive << xAxisLength_ << yAxisLength_ << zAxisLength_;
    }
    //@}

private:
    //! @name Member data
    //@{
    float xAxisLength_;
    float yAxisLength_;
    float zAxisLength_;
    //@}
};

}
}

#endif // __Dimension_h_
