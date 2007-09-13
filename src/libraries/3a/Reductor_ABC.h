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
    Reductor_ABC() : count_( 0 ) {}
    //@}

    //! @name Operations
    //@{
    virtual void OnBeginTick() = 0;
    virtual void SetKey( const K& key ) = 0;
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
    //! @name Copy/Assignment
    //@{
    Reductor_ABC( const Reductor_ABC& );            //!< Copy constructor
    Reductor_ABC& operator=( const Reductor_ABC& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual std::string GetName() const { return "Reductor"; }
    //@}

private:
    //! @name Member data
    //@{
    unsigned count_;
    //@}
};

#endif // __Reductor_ABC_h_
