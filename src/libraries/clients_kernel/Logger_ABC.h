// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Logger_ABC_h_
#define __Logger_ABC_h_

#include <sstream>
#include <boost/shared_ptr.hpp>

namespace kernel
{

// =============================================================================
/** @class  Logger_ABC
    @brief  Logger definition
*/
// Created: AGE 2008-05-16
// =============================================================================
class Logger_ABC
{
public:
    //! @name Types
    //@{
    class LogElement
    {
    private:
        struct Ender
        {
             Ender( Logger_ABC& base, std::stringstream& output );
            ~Ender();
             Logger_ABC* base_;
             std::stringstream* output_;
        };
    public:
         LogElement( Logger_ABC& base, std::stringstream& output );
        ~LogElement();
        template< typename T >
        LogElement operator<<( const T& value ) const
        {
            *ender_->output_ << value;
            return *this;
        }
    private:
        boost::shared_ptr< Ender > ender_;
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Logger_ABC() {};
    virtual ~Logger_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual LogElement Info() = 0;
    virtual LogElement Warning() = 0;
    virtual LogElement Error() = 0;

    virtual void End( std::stringstream& output ) = 0;
    //@}
};

}

#endif // __Logger_ABC_h_
