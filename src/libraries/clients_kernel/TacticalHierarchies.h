// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __kernel_TacticalHierarchies_h_
#define __kernel_TacticalHierarchies_h_

#include "Hierarchies.h"

namespace kernel
{

// =============================================================================
/** @class  Symbol_ABC
    @brief  Symbol definition
*/
// Created: AGE 2006-10-04
// =============================================================================
class Symbol_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Symbol_ABC() {};
    virtual ~Symbol_ABC() {};
    //@}

    virtual const Entity_ABC& GetEntity() const = 0;
    virtual std::string GetSymbol() const = 0;
    virtual std::string GetLevel() const = 0;
    virtual void UpdateSymbol( bool up = true ) = 0;
};

// =============================================================================
/** @class  TacticalHierarchies
    @brief  Tactical hierarchies
*/
// Created: AGE 2006-10-04
// =============================================================================
class TacticalHierarchies : public Hierarchies, public Symbol_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             TacticalHierarchies() {};
    virtual ~TacticalHierarchies() {};
    //@}

    //! @name Operations
    //@{
    virtual std::string GetSymbol() const;
    virtual std::string GetLevel() const;
    virtual void UpdateSymbol( bool up = true );
    //@}
};

}

#endif // __kernel_TacticalHierarchies_h_
