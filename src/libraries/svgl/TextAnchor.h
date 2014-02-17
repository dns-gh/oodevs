// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TextAnchor_h_
#define __TextAnchor_h_

#include "Property_ABC.h"

namespace svg
{

// =============================================================================
/** @class  TextAnchor
    @brief  TextAnchor
*/
// Created: AGE 2006-10-20
// =============================================================================
class TextAnchor : public Property_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit TextAnchor( const std::string& content );
    virtual ~TextAnchor();
    //@}

    //! @name Operations
    //@{
    virtual TextAnchor& Clone() const;
    float GetFactor() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TextAnchor( const TextAnchor& );            //!< Copy constructor
    TextAnchor& operator=( const TextAnchor& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    float factor_;
    //@}
};

}

#endif // __TextAnchor_h_
