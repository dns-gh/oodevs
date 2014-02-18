// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FontSize_h_
#define __FontSize_h_

#include "Property_ABC.h"

namespace svg
{

// =============================================================================
/** @class  FontSize
    @brief  FontSize
*/
// Created: AGE 2006-10-20
// =============================================================================
class FontSize : public Property_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit FontSize( const std::string& content );
    virtual ~FontSize();
    //@}

    //! @name Operations
    //@{
    void ApplyScale() const;
    virtual FontSize& Clone() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FontSize( const FontSize& );            //!< Copy constructor
    FontSize& operator=( const FontSize& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    float size_;
    //@}
};

}

#endif // __FontSize_h_
