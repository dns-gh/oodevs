// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Opacity_h_
#define __Opacity_h_

#include "Property_ABC.h"

namespace svg
{

// =============================================================================
/** @class  Opacity
    @brief  Opacity
*/
// Created: AGE 2006-08-23
// =============================================================================
class Opacity : public Property_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Opacity( float value = 1.f );
    explicit Opacity( const std::string& content );
    virtual ~Opacity();
    //@}

    //! @name Operations
    //@{
    float GetValue() const;
    virtual Opacity& Clone() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Opacity& operator=( const Opacity& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    float opacity_;
    //@}
};

}

#endif // __Opacity_h_
