// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ParamInterface_ABC_h_
#define __ParamInterface_ABC_h_

namespace actions
{
    namespace gui
    {
        class Param_ABC;

// =============================================================================
/** @class  ParamInterface_ABC
    @brief  ParamInterface_ABC
*/
// Created: ABR 2012-01-11
// =============================================================================
class ParamInterface_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamInterface_ABC() {}
    virtual ~ParamInterface_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual QString Title() const = 0;
    virtual int GetIndex( Param_ABC* param ) const = 0;
    //@}
};

    }
}

#endif // __ParamInterface_ABC_h_
