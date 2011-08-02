// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __RcEntityResolver_ABC_h_
#define __RcEntityResolver_ABC_h_

#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#pragma warning( pop )

// =============================================================================
/** @class  RcEntityResolver_ABC
    @brief  Rc entity resolver declaration
*/
// Created: SBO 2006-09-18
// =============================================================================
class RcEntityResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             RcEntityResolver_ABC() {}
    virtual ~RcEntityResolver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual QString CreateLink( const QString& type, unsigned long id ) const = 0;
    //@}
};

#endif // __RcEntityResolver_ABC_h_
