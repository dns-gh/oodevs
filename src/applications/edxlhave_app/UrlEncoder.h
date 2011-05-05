// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __UrlEncoder_h_
#define __UrlEncoder_h_

#include <string>

namespace edxl
{

// =============================================================================
/** @class  UrlEncoder
    @brief  UrlEncoder
*/
// Created: JCR 2010-07-23
// =============================================================================
class UrlEncoder
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit UrlEncoder( const std::string& message );
    virtual ~UrlEncoder();
    //@}

    //! @name Operations
    //@{
    void Write( std::ostream& os ) const;
    //@}

    //! @name
    //@{
    friend std::ostream& operator << ( std::ostream& os, const UrlEncoder& encoder )
    {
        encoder.Write( os );
        return os;
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UrlEncoder( const UrlEncoder& );            //!< Copy constructor
    UrlEncoder& operator=( const UrlEncoder& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Encode( const std::string& message );
    //@}

private:
    //! @name Member data
    //@{
    std::string message_;
    //@}
};

}

#endif // __UrlEncoder_h_
