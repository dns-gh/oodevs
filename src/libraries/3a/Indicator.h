// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Indicator_h_
#define __Indicator_h_

#include "protocol/ClientPublisher_ABC.h"
#include "Functions.h"
#include "Result_ABC.h"

// =============================================================================
/** @class  Indicator
    @brief  Indicator
*/
// Created: AGE 2008-08-04
// =============================================================================
template< typename K, typename T >
class Indicator : public Function1_ABC< K, T >
                , public Result_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Indicator( dispatcher::ClientPublisher_ABC& publisher, const std::string& name )
                 : publisher_( publisher ), name_( name ) {};
    virtual ~Indicator() {}
    //@}

    //! @name Operations
    //@{
    virtual std::string GetName() const { return "Indicator"; }
    virtual void BeginTick() {}
    virtual void SetKey( const K& ) {}
    virtual void Apply( const T& arg )
    {
        aar::Indicator result;
        result().set_name( name_.c_str() );
        result().set_value( float( arg ) );
        result.Send( publisher_ );
    }
    virtual void EndTick() {};
    virtual void Commit() const {}
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Indicator( const Indicator& );            //!< Copy constructor
    Indicator& operator=( const Indicator& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::ClientPublisher_ABC& publisher_;
    std::string name_;
    //@}
};

#endif // __Indicator_h_
