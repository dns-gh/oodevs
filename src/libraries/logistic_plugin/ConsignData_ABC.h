// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ConsignData_ABC_h_
#define __ConsignData_ABC_h_

#include <fstream>
#include <sstream>


namespace plugins
{
namespace logistic
{

// =============================================================================
/** @class  ConsignData_ABC
    @brief  ConsignData_ABC
*/
// Created: MMC 2012-08-06
// =============================================================================
class ConsignData_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
            ConsignData_ABC( const std::string& requestId ) : requestId_( requestId ) {}
    virtual ~ConsignData_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void operator>>( std::stringstream& output ) const = 0;
    virtual void operator>>( std::ofstream& output ) const { std::stringstream line; *this >> line; output << line.str(); }
    inline static std::string GetSeparator() { return " ; "; }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ConsignData_ABC( const ConsignData_ABC& );            //!< Copy constructor
    ConsignData_ABC& operator=( const ConsignData_ABC& ); //!< Assignment operator
    //@}

protected:
    //! @name Member data
    //@{
    const std::string requestId_;
    //@}
};
}
}

#endif // __ConsignData_ABC_h_
