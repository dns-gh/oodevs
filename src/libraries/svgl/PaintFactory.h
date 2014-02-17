// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PaintFactory_h_
#define __PaintFactory_h_

#include "ColorFactory.h"

namespace svg
{

// =============================================================================
/** @class  PaintFactory
    @brief  PaintFactory
*/
// Created: AGE 2006-08-23
// =============================================================================
class PaintFactory : public ColorFactory
{

public:
    //! @name Constructors/Destructor
    //@{
             PaintFactory();
    virtual ~PaintFactory();
    //@}

    //! @name Operations
    //@{
    virtual Paint_ABC* CreateProperty( const std::string& content ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PaintFactory( const PaintFactory& );            //!< Copy constructor
    PaintFactory& operator=( const PaintFactory& ); //!< Assignement operator
    //@}
};

}

#endif // __PaintFactory_h_
