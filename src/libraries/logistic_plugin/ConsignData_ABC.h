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
            ConsignData_ABC() : separator_( " , " ), subSeparator_( ", " )
                              , noValue_( -1 ), noField_( "_" ) {}
    virtual ~ConsignData_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Write( std::ofstream& output ) = 0;
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
    const std::string separator_;
    const std::string subSeparator_;
    const std::string noField_;
    const int noValue_;
    //@}
};
}
}

#endif // __ConsignData_ABC_h_
