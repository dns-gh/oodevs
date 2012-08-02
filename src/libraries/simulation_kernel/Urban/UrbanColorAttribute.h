// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __UrbanColorAttribute_h_
#define __UrbanColorAttribute_h_

#include "UrbanExtension_ABC.h"

// =============================================================================
/** @class  UrbanColorAttribute
    @brief  UrbanColorAttribute
*/
// Created: JSR 2012-08-01
// =============================================================================
class UrbanColorAttribute : public UrbanExtension_ABC
                          , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit UrbanColorAttribute( xml::xistream& xis );
    virtual ~UrbanColorAttribute();
    //@}

    //! @name Operations
    //@{
    unsigned short Red() const;
    unsigned short Green() const;
    unsigned short Blue() const;
    float Alpha() const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned short red_;
    unsigned short green_;
    unsigned short blue_;
    float alpha_;
    //@}
};

#endif // __UrbanColorAttribute_h_
