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

namespace kernel
{
    class Controller;
    class Logger_ABC;
}

namespace MsgsDispatcherToClient
{
    class MsgServicesDescription;
}

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
             Services( kernel::Controller& controller, kernel::Logger_ABC& logger );
    virtual ~Services();
    //@}

    //! @name Operations
    //@{
    void Update( const MsgsDispatcherToClient::MsgServicesDescription& message );

    template< typename T >
    bool HasService() const
    {
        return HasService( typeid( T ).name() );
    }

    template< typename T >
    bool RequireService() const
    {
        return RequireService( typeid( T ).name() );
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
    bool HasService    ( const std::string& name ) const;
    bool RequireService( const std::string& name ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    kernel::Logger_ABC& logger_;
    std::set< std::string > services_;
    //@}
};

#endif // __Services_h_
