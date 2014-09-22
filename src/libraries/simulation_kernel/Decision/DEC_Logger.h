// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef DEC_Logger_h
#define DEC_Logger_h

#include <string>
#include <boost/noncopyable.hpp>

namespace sword
{
// =============================================================================
/** @class  DEC_Logger
    @brief  DEC_Logger
*/
// Created: SLI 2013-02-07
// =============================================================================
class DEC_Logger : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Logger();
    explicit DEC_Logger( const std::string& type );
    virtual ~DEC_Logger();
    //@}

    //! @name Operations
    //@{
    void Log( const char* name, unsigned int identifier );
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive >
    void serialize( Archive& archive, const unsigned int )
    {
        archive & type_;
    }
    //@}

private:
    //! @name Member data
    //@{
    std::string type_;
    //@}
};

}

#endif // DEC_Logger_h
