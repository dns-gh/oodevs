// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Param_ABC.h $
// $Author: Ape $
// $Modtime: 3/08/04 17:31 $
// $Revision: 5 $
// $Workfile: Param_ABC.h $
//
// *****************************************************************************

#ifndef __Param_ABC_h_
#define __Param_ABC_h_

#include "Observer_ABC.h"
class ActionController;

// =============================================================================
// Created: APE 2004-03-18
// =============================================================================
class Param_ABC : private Observer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Param_ABC();
    virtual ~Param_ABC();
    //@}

    //! @name Operations
    //@{
    void RegisterIn( ActionController& controller );
    void SetOptional( bool );

    virtual bool CheckValidity();
    virtual void Commit() = 0;
    //@}

    //! @name Accessors
    //@{
    bool IsOptional() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Param_ABC( const Param_ABC& );
    Param_ABC& operator=( const Param_ABC& );
    //@}

private:
    bool bIsOptional_;
    ActionController* controller_;
};

#endif // __Param_ABC_h_
