// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BaseDisplayer_h_
#define __BaseDisplayer_h_

#include "Displayer_ABC.h"
class Agent;
class DotationType;

// =============================================================================
/** @class  BaseDisplayer
    @brief  BaseDisplayer
*/
// Created: AGE 2006-02-09
// =============================================================================
class BaseDisplayer : public Displayer_ABC
                  , public Caller< bool >
                  , public Caller< Agent >
                  , public Caller< DotationType >
{

public:
    //! @name Constructors/Destructor
    //@{
             BaseDisplayer();
    virtual ~BaseDisplayer();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    BaseDisplayer( const BaseDisplayer& );            //!< Copy constructor
    BaseDisplayer& operator=( const BaseDisplayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void Call( const bool& value );
    virtual void Call( const Agent& value );
    virtual void Call( const DotationType& value );
    //@}
};

#endif // __BaseDisplayer_h_
