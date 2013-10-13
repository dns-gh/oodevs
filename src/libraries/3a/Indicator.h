// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Indicator_h_
#define __Indicator_h_

#include "Functions.h"
#include "Result_ABC.h"

namespace dispatcher
{
    class ClientPublisher_ABC;
}

void SendIndicator( dispatcher::ClientPublisher_ABC& pub, const std::string& name,
        float value );

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
        : publisher_( publisher )
        , name_     ( name )
    {
        // NOTHING
    }
    virtual ~Indicator()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    virtual std::string GetName() const { return "Indicator"; }
    virtual void BeginTick() {}
    virtual void SetKey( const K& ) {}
    virtual void Apply( const T& arg )
    {
        SendIndicator( publisher_, name_, static_cast< float >( arg ) );
    }
    virtual void EndTick() {}
    virtual void Commit( unsigned int /*skippedFrames*/, unsigned int /*firstTick*/ ) const {}
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::ClientPublisher_ABC& publisher_;
    std::string name_;
    //@}
};

#endif // __Indicator_h_
