// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __DispatchedFunction_h_
#define __DispatchedFunction_h_

#include "ModelFunction_ABC.h"
#include "ValueHandler_ABC.h"
#include <memory>

// =============================================================================
/** @class  DispatchedFunction
    @brief  Dispatched function
*/
// Created: AGE 2007-08-29
// =============================================================================
template< typename K >
class DispatchedFunction : public ModelFunction_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DispatchedFunction( const K& key, ValueHandler_ABC< K >& keyHandler, std::auto_ptr< ModelFunction_ABC > base )
                 : key_( key ), keyHandler_( keyHandler ), base_( base ) {}
    virtual ~DispatchedFunction() {}
    //@}

    //! @name Operations
    //@{
    virtual void BeginTick()
    {
        base_->BeginTick();
    }
    virtual void Receive( const ASN1T_MsgsSimToClient& message )
    {
        base_->Receive( message );
    }
    virtual void EndTick()
    {
        keyHandler_.Handle( key_ );
        base_->EndTick();
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DispatchedFunction( const DispatchedFunction& );            //!< Copy constructor
    DispatchedFunction& operator=( const DispatchedFunction& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const K key_;
    ValueHandler_ABC< K >& keyHandler_;
    std::auto_ptr< ModelFunction_ABC > base_;
    //@}
};

#endif // __DispatchedFunction_h_
