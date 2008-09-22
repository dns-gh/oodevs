// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Services_h_
#define __Services_h_

#include <set>
#include <string>

namespace dispatcher
{
    class ClientPublisher_ABC;

// =============================================================================
/** @class  Services
    @brief  Services
*/
// Created: AGE 2008-08-13
// =============================================================================
class Services
{

public:
    //! @name Constructors/Destructor
    //@{
             Services();
    virtual ~Services();
    //@}

    //! @name Operations
    //@{
    void Send( ClientPublisher_ABC& publisher ) const;

    template< typename T >
    void Declare()
    {
        Declare( typeid( T ).name() );
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Services( const Services& );            //!< Copy constructor
    Services& operator=( const Services& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Declare( const std::string& name );
    //@}

private:
    //! @name Member data
    //@{
    std::set< std::string > services_;
    //@}
};

}

#endif // __Services_h_
