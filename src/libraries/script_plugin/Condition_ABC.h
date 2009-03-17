// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Condition_ABC_h_
#define __Condition_ABC_h_

namespace directia
{
    class ScriptRef;
}

namespace plugins
{
namespace script
{

// =============================================================================
/** @class  Condition_ABC
    @brief  Condition_ABC
*/
// Created: AGE 2008-06-12
// =============================================================================
class Condition_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Condition_ABC() {};
    virtual ~Condition_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Activate( bool ) = 0;
    virtual void SetTarget( const directia::ScriptRef& function ) = 0;
    //@}
};

}
}

#endif // __Condition_ABC_h_
