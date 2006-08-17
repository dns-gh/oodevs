// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Param_ABC_h_
#define __Param_ABC_h_

#include "clients_kernel/Observer_ABC.h"
#include "clients_kernel/Drawable_ABC.h"

class ActionController;
class GlTools_ABC;

// =============================================================================
// Created: APE 2004-03-18
// =============================================================================
class Param_ABC : public Observer_ABC, public Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Param_ABC();
    virtual ~Param_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void RemoveFromController();
    virtual void RegisterIn( ActionController& controller );
    virtual void SetOptional( bool );

    virtual void Draw( const geometry::Point2f& point, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const;
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
