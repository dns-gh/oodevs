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
template< typename C, typename T >
class ObjectMessageCallback : public ObjectMessageCallback_ABC
{
public:
    //! @name Types
    //@{
    typedef void (C::*T_Callback)( DIN::DIN_Link& input, const T& object );
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    ObjectMessageCallback( C& instance, T_Callback callback ) 
        : instance_( instance )
        , callback_( callback )
    {}
    //@}

    //! @name Operations
    //@{
    virtual void OnMessage( DIN::DIN_Link& link, DIN::DIN_Input& input )
    {
        AsnMessageDecoder< T > decoder( input );
        (instance_.*callback_)( link, decoder.GetAsnMsg() );
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
    C& instance_;
    T_Callback callback_;
    //@}
};

}

#endif // __ObjectMessageCallback_h_
