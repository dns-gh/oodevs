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
    /*
    struct ValueConcept
    {
        typedef impl Type;
        void Prepare();
        void Receive( const ASN1T_MsgsSimToClient& message );
        void Push( ValueHandler_ABC< Type >& handler );
    };
    */
    typedef typename Value::Type Return_Type;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit ModelFunction( ValueHandler_ABC< Return_Type >& handler, Value value = Value() )
                : handler_( handler ), value_( value ) {};
    virtual ~ModelFunction() {};
    //@}

    //! @name Operations
    //@{
    virtual void BeginTick()
    {
        handler_.BeginTick();
        value_.Prepare();
    }
    virtual void Receive( const ASN1T_MsgsSimToClient& message )
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
    //! @name Copy/Assignment
    //@{
    ModelFunction( const ModelFunction& );            //!< Copy constructor
    ModelFunction& operator=( const ModelFunction& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    ValueHandler_ABC< Return_Type >& handler_;
    Value value_;
    //@}
};

#endif // __ModelFunction_h_
