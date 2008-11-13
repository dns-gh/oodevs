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

namespace tools
{
class ApplicationMutex_ABC
{
public:
             ApplicationMutex_ABC() {}
    virtual ~ApplicationMutex_ABC() {}

    virtual bool IsOwned() const = 0;
};

// =============================================================================
/** @class  ApplicationMutex
    @brief  ApplicationMutex
*/
// Created: RDS 2008-08-18
// =============================================================================
class ApplicationMutex : public ApplicationMutex_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ApplicationMutex( const std::string& name );
    virtual ~ApplicationMutex();
    //@}

    //! @name members 
    //@{
    bool IsOwned() const;
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
    std::auto_ptr< ApplicationMutex_ABC > internal_;
    //@}
};

}

#endif // __ApplicationMutex_h_
