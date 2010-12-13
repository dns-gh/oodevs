// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __WiseEntity_h_
#define __WiseEntity_h_

#include "Entity_ABC.h"
#include <map>

// =============================================================================
/** @class  WiseEntity
    @brief  WiseEntity
*/
// Created: SEB 2010-12-13
// =============================================================================
class WiseEntity : public Entity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             WiseEntity( unsigned long id, const std::wstring& label );
    virtual ~WiseEntity();
    //@}

    //! @name Accessors
    //@{
    virtual unsigned long GetId() const;
    virtual WISE_HANDLE GetHandle() const;
    //@}

    //! @name Operations
    //@{
    virtual void Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime ) const = 0;
    virtual void Destroy( CWISEDriver& driver, const WISE_HANDLE& database ) const;
    //@}

protected:
    //! @name Helpers
    //@{
    virtual std::wstring MakeIdentifier() const = 0;
    std::wstring FormatMessage( const std::wstring& message ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    WiseEntity( const WiseEntity& );            //!< Copy constructor
    WiseEntity& operator=( const WiseEntity& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const unsigned long id_;
    const std::wstring label_;
    //@}

protected:
    //! @name Member data
    //@{
    mutable WISE_HANDLE handle_;
    mutable std::map< std::wstring, WISE_HANDLE > attributes_;
    //@}
};

#endif // __WiseEntity_h_
