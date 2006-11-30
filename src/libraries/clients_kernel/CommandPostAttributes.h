// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CommandPostAttributes_h_
#define __CommandPostAttributes_h_

#include "Extension_ABC.h"

namespace kernel
{
    class Agent_ABC;

// =============================================================================
/** @class  CommandPostAttributes
    @brief  Command-Post Attributes extension
*/
// Created: SBO 2006-11-30
// =============================================================================
class CommandPostAttributes : public Extension_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit CommandPostAttributes( const Agent_ABC& holder );
    virtual ~CommandPostAttributes();
    //@}

    //! @name Operations
    //@{
    bool IsCommandPost() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    CommandPostAttributes( const CommandPostAttributes& );            //!< Copy constructor
    CommandPostAttributes& operator=( const CommandPostAttributes& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const Agent_ABC& holder_;
    //@}
};

}

#endif // __CommandPostAttributes_h_
