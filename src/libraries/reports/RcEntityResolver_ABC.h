// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __RcEntityResolver_ABC_h_
#define __RcEntityResolver_ABC_h_

#include <qstring.h>

// =============================================================================
/** @class  RcEntityResolver_ABC
    @brief  RcEntityResolver_ABC
*/
// Created: SBO 2006-09-18
// =============================================================================
class RcEntityResolver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             RcEntityResolver_ABC() {};
    virtual ~RcEntityResolver_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual QString CreateLink( const QString& type, unsigned long id ) const = 0;
    //@}
};

#endif // __RcEntityResolver_ABC_h_
