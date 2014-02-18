// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FontFamily_h_
#define __FontFamily_h_

#include "Property_ABC.h"

namespace svg
{

// =============================================================================
/** @class  FontFamily
    @brief  FontFamily
*/
// Created: AGE 2006-10-20
// =============================================================================
class FontFamily : public Property_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit FontFamily( const std::string& familyName );
    virtual ~FontFamily();
    //@}

    //! @name Operations
    //@{
    const std::string& GetName() const;
    virtual FontFamily& Clone() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FontFamily( const FontFamily& );            //!< Copy constructor
    FontFamily& operator=( const FontFamily& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    std::string familyName_;
    //@}
};

}

#endif // __FontFamily_h_
