// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Formation_ABC_h_
#define __Formation_ABC_h_

#include "Entity_ABC.h"

namespace kernel
{
    class Team_ABC;

// =============================================================================
/** @class  Formation_ABC
    @brief  Formation_ABC
*/
// Created: SBO 2006-09-19
// =============================================================================
class Formation_ABC : public Entity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Formation_ABC();
    virtual ~Formation_ABC();
    //@}

    //! @name Accessors
    //@{
    virtual bool IsInTeam( const Team_ABC& team ) const = 0; // $$$$ AGE 2006-08-03: descendre dans Entity_ABC
    virtual const QString& GetLevel() const = 0;
    //@}

    //! @name Operations
    //@{
    virtual void Select( ActionController& controller ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where ) const;
    virtual void Activate( ActionController& controller ) const;
    //@}
};

}

#endif // __Formation_ABC_h_
