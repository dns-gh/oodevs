// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ColorAttribute_h_
#define __ColorAttribute_h_

#include "UrbanObjectAttribute_ABC.h"
#include <boost/noncopyable.hpp>

namespace dispatcher
{
// =============================================================================
/** @class  ColorAttribute
    @brief  Color attribute
*/
// Created: RPD 2010-01-06
// =============================================================================
class ColorAttribute : public UrbanObjectAttribute_ABC
                     , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ColorAttribute( const sword::UrbanAttributes& message );
    virtual ~ColorAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const sword::UrbanAttributes& message );
    virtual void Send  ( sword::UrbanAttributes& message ) const;
    //@}

private:
    //! @name Member data
    //@{
    int red_;
    int green_;
    int blue_;
    float alpha_;
    //@}
};

}

#endif // __ColorAttribute_h_
