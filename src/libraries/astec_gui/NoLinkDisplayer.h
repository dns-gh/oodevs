// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __NoLinkDisplayer_h_
#define __NoLinkDisplayer_h_

#include "BaseDisplayer.h"

// =============================================================================
/** @class  NoLinkDisplayer
    @brief  NoLinkDisplayer
*/
// Created: AGE 2006-06-29
// =============================================================================
class NoLinkDisplayer : public BaseDisplayer
{

public:
    //! @name Constructors/Destructor
    //@{
             NoLinkDisplayer();
    virtual ~NoLinkDisplayer();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    NoLinkDisplayer( const NoLinkDisplayer& );            //!< Copy constructor
    NoLinkDisplayer& operator=( const NoLinkDisplayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void Call( const Agent_ABC& value );
    virtual void Call( const Population_ABC& value );
    virtual void Call( const Object_ABC& value );
    //@}
};

#endif // __NoLinkDisplayer_h_
