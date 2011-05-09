// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Reductor_ABC_h_
#define __Reductor_ABC_h_

#include "Functions.h"

// =============================================================================
/** @class  Reductor_ABC
    @brief  Reductor_ABC
*/
// Created: AGE 2007-09-13
// =============================================================================
template< typename K, typename T >
class Reductor_ABC : public Function1_ABC< K, T >
{
public:
    //! @name Constructors/Destructor
    //@{
    Reductor_ABC()
        : count_( 0 )
    {
        // NOTHING
    }
    virtual ~Reductor_ABC()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    virtual void OnBeginTick() = 0;
    virtual void SetKey( const K& )
    {
        // NOTHING
    };
    virtual void Apply( const T& arg ) = 0;
    virtual void OnEndTick() = 0;
    //@}

private:
    //! @name Operations
    //@{
    virtual void BeginTick()
    {
        if( count_++ == 0 )
            OnBeginTick();
    }
    virtual void EndTick()
    {
        if( --count_ == 0 )
            OnEndTick();
    }
    //@}

private:
    //! @name Helpers
    //@{
    virtual std::string GetName() const { return "Reductor"; }
    //@}

private:
    //! @name Member data
    //@{
    unsigned int count_;
    //@}
};

// =============================================================================
/** @class  Reductor2_ABC
    @brief  Reductor2_ABC
*/
// Created: SBO 2010-07-29
// =============================================================================
template< typename K, typename A1, typename A2 >
class Reductor2_ABC : public Function2_ABC< K, A1, A2 >
{
public:
    //! @name Constructors/Destructor
    //@{
    Reductor2_ABC()
        : count_( 0 )
    {
        // NOTHING
    }
    virtual ~Reductor2_ABC()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    virtual void OnBeginTick() = 0;
    virtual void SetKey( const K& )
    {
        // NOTHING
    };
    virtual void Apply( const A1& arg1, const A2& arg2 ) = 0;
    virtual void OnEndTick() = 0;
    //@}

private:
    //! @name Operations
    //@{
    virtual void BeginTick()
    {
        if( count_++ == 0 )
            OnBeginTick();
    }
    virtual void EndTick()
    {
        if( --count_ == 0 )
            OnEndTick();
    }
    //@}

private:
    //! @name Helpers
    //@{
    virtual std::string GetName() const { return "Reductor"; }
    //@}

private:
    //! @name Member data
    //@{
    unsigned int count_;
    //@}
};

#endif // __Reductor_ABC_h_
