// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectMessageCallback_h_
#define __ObjectMessageCallback_h_

#include "AsnMessageDecoder.h"
#include <boost/function.hpp>
#include <boost/bind.hpp>

namespace DIN
{
    class DIN_Link;
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
    virtual void OnMessage( DIN::DIN_Link& link, DIN::DIN_Input& input ) = 0;
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
    typedef boost::function2< void, DIN::DIN_Link& , const T& > T_Callback;
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
    virtual void OnMessage( DIN::DIN_Link& link, DIN::DIN_Input& input )
    {
        AsnMessageDecoder< T > decoder( input );
        for( std::vector< T_Callback >::iterator it = callbacks_.begin(); it != callbacks_.end(); ++it )
            (*it)( link, decoder.GetAsnMsg() );
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
