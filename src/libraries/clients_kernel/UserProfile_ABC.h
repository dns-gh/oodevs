// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __UserProfile_ABC_h_
#define __UserProfile_ABC_h_

namespace kernel
{
    class Entity_ABC;

// =============================================================================
/** @class  UserProfile_ABC
    @brief  User Profile declaration
*/
// Created: JSR 2014-06-03
// =============================================================================
class UserProfile_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UserProfile_ABC() {}
    virtual ~UserProfile_ABC() {}
    //@}

    virtual const QString& GetLogin() const = 0;
    virtual const QString& GetPassword() const = 0;
    virtual bool IsSupervisor() const = 0;
    virtual bool HasTimeControl() const = 0;

    virtual void SetLogin( const QString& value ) = 0;
    virtual void SetPassword( const QString& value ) = 0;
    virtual void SetSupervisor( bool value ) = 0;
    virtual void SetTimeControl( bool value ) = 0;

    virtual void VisitAllAutomats( std::set< unsigned long >& elements ) const = 0;


    virtual bool IsReadable( const kernel::Entity_ABC& entity ) const = 0;
    virtual bool IsWriteable( const kernel::Entity_ABC& entity ) const = 0;

    virtual void SetReadable( const kernel::Entity_ABC& entity, bool readable ) = 0;
    virtual void SetWriteable( const kernel::Entity_ABC& entity, bool writeable ) = 0;
};

}

#endif // __kernel_UserProfile_h_
