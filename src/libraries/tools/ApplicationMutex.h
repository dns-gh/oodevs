// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ApplicationMutex_h_
#define __ApplicationMutex_h_

class ApplicationMutexInternal ; 


namespace tools
{

// =============================================================================
/** @class  ApplicationMutex
    @brief  ApplicationMutex
*/
// Created: RDS 2008-08-18
// =============================================================================
class ApplicationMutex
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ApplicationMutex( const std::string& name );
    virtual ~ApplicationMutex();
    //@}

    //! @name members 
    //@{
    bool IsOwned(); 
    //@}
        
private:
    //! @name Copy/Assignment
    //@{
    ApplicationMutex( const ApplicationMutex& );            //!< Copy constructor
    ApplicationMutex& operator=( const ApplicationMutex& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ApplicationMutexInternal> internal_ ; 
    //@}
};

}

#endif // __ApplicationMutex_h_
