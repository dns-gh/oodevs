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

class ColorRGBA;

namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  ColorAttribute
    @brief  Color attribute
*/
// Created: RPD 2010-01-06
// =============================================================================
class ColorAttribute : public UrbanObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ColorAttribute( const MsgsSimToClient::UrbanAttributes& message );
    virtual ~ColorAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const MsgsSimToClient::UrbanAttributes& message );
    virtual void Send  ( MsgsSimToClient::UrbanAttributes& message ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ColorAttribute( const ColorAttribute& );            //!< Copy constructor
    ColorAttribute& operator=( const ColorAttribute& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    ColorRGBA* color_;
    //@}
};

}

#endif // __ColorAttribute_h_
