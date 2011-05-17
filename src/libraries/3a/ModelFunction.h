// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ModelFunction_h_
#define __ModelFunction_h_

#include "ModelFunction_ABC.h"
#include "ValueHandler_ABC.h"

// =============================================================================
/** @class  ModelFunction
    @brief  Value based function implementation
*/
// Created: AGE 2007-08-29
// =============================================================================
template< typename Value >
class ModelFunction : public ModelFunction_ABC
{
public:
    //! @name Types
    //@{
    typedef typename Value::Type Return_Type;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit ModelFunction( ValueHandler_ABC< Return_Type >& handler, const Value& value = Value() )
                : handler_( handler )
                , value_  ( value )
    {
        // NOTHING
    }
    virtual ~ModelFunction()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    virtual void BeginTick()
    {
        handler_.BeginTick();
        value_.Prepare();
    }
    virtual void Receive( const sword::SimToClient& message )
    {
        value_.Receive( message );
    }
    virtual void EndTick()
    {
        value_.Push( handler_ );
        handler_.EndTick();
    }
    //@}

private:
    //! @name Member data
    //@{
    ValueHandler_ABC< Return_Type >& handler_;
    Value value_;
    //@}
};

#endif // __ModelFunction_h_
