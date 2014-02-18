// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FontWeight_h_
#define __FontWeight_h_

#include "Property_ABC.h"

namespace svg
{

// =============================================================================
/** @class  FontWeight
    @brief  FontWeight
*/
// Created: AGE 2006-10-20
// =============================================================================
class FontWeight : public Property_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit FontWeight( const std::string& content );
    virtual ~FontWeight();
    //@}

    //! @name Operations
    //@{
    virtual FontWeight& Clone() const;
    int GetValue() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FontWeight( const FontWeight& );            //!< Copy constructor
    FontWeight& operator=( const FontWeight& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    int value_;
    //@}
};

}

#endif // __FontWeight_h_
