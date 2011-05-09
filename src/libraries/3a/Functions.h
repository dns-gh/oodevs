// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Functions_h_
#define __Functions_h_

#include "Slots.h"

// =============================================================================
/** @class  Function_ABC
    @brief  Function_ABC
*/
// Created: AGE 2007-08-30
// =============================================================================
class Function_ABC : public Slot_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    Function_ABC()
    {
        // NOTHING
    }
    virtual ~Function_ABC()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    virtual std::string GetName() const { return "Function"; }
    virtual void BeginTick() = 0;
    virtual void EndTick() = 0;
    //@}
};

// =============================================================================
/** @class  Function1_ABC
    @brief  Function1_ABC
*/
// Created: AGE 2007-08-30
// =============================================================================
template< typename K, typename A >
class Function1_ABC : public Function_ABC
{
public:
    //! @name Types
    //@{
    typedef K Key_Type;
    typedef A Argument_Type;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    Function1_ABC()
    {
        // NOTHING
    }
    virtual ~Function1_ABC()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    virtual void BeginTick() = 0;
    virtual void SetKey( const K& key ) = 0;
    virtual void Apply( const A& arg ) = 0;
    virtual void EndTick() = 0;
    //@}
};

// =============================================================================
/** @class  Function2_ABC
    @brief  Function2_ABC
*/
// Created: AGE 2007-08-30
// =============================================================================
template< typename K, typename A1, typename A2 >
class Function2_ABC : public Function_ABC
{
public:
    //! @name Types
    //@{
    typedef K Key_Type;
    typedef A1 First_Argument_Type;
    typedef A2 Second_Argument_Type;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    Function2_ABC()
    {
        // NOTHING
    }
    virtual ~Function2_ABC()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    virtual void BeginTick() = 0;
    virtual void SetKey( const K& key ) = 0;
    virtual void Apply( const A1& arg1, const A2& arg2 ) = 0;
    virtual void EndTick() = 0;
    //@}
};

#endif // __Functions_h_
