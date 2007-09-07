// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectMessageCallback_h_
#define __ObjectMessageCallback_h_

#include "AsnMessageDecoder.h"
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/bind/apply.hpp>
#include <vector>

namespace DIN
{
    class DIN_Input;
}

namespace tools
{

// =============================================================================
/** @class  ObjectMessageCallback_ABC
    @brief  ObjectMessageCallback_ABC
*/
// Created: AGE 2007-03-07
// =============================================================================
class ObjectMessageCallback_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectMessageCallback_ABC() {}
    virtual ~ObjectMessageCallback_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void OnMessage( const std::string& link, Message& input ) = 0;
    //@}
};

// =============================================================================
/** @class  ObjectMessageCallback
    @brief  Callback wrapper
*/
// Created: AGE 2007-03-07
// =============================================================================
template< typename T >
class ObjectMessageCallback : public ObjectMessageCallback_ABC
{
public:
    //! @name Types
    //@{
    typedef boost::function2< void, const std::string& , const T& > T_Callback;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             ObjectMessageCallback() {}
    virtual ~ObjectMessageCallback() {}
    //@}

    //! @name Operations
    //@{
    void AddCallback( const T_Callback& callback )
    {
        callbacks_.push_back( callback );
    }
    virtual void OnMessage( const std::string& link, Message& input )
    {
        AsnMessageDecoder< T > decoder( input );
        std::for_each( callbacks_.begin(), callbacks_.end(),
            boost::bind( boost::apply< void >(), _1, boost::ref( link ), boost::ref( decoder ) ) );
    }
    //@}

private:
    //! @name Copy / Assignment
    //@{
    ObjectMessageCallback( const ObjectMessageCallback& );
    ObjectMessageCallback& operator=( const ObjectMessageCallback& );
    //@}

private:
    //! @name Member data
    //@{
    std::vector< T_Callback > callbacks_;
    //@}
};

}

#endif // __ObjectMessageCallback_h_
