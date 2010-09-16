// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
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

    template< typename T >
    bool HasService() const
    {
        return HasService( typeid( T ).name() );
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
    bool HasService( const std::string& name ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< std::string >      T_Services;
    typedef T_Services::const_iterator CIT_Services;
    //@}

private:
    //! @name Member data
    //@{
    T_Services services_;
    //@}
};

}

#endif // __Services_h_
