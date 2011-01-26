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
             WiseEntity( unsigned long id, const std::wstring& type );
    virtual ~WiseEntity();
    //@}

    //! @name Accessors
    //@{
    virtual unsigned long GetId() const;
    virtual WISE_HANDLE GetHandle() const;
    template< typename T >
    bool IsA() const
    {
        return dynamic_cast< const T* >( this ) != 0;
    }
    //@}

    //! @name Operations
    //@{
    virtual void Destroy( CWISEDriver& driver, const WISE_HANDLE& database ) const;
    virtual void Update( SwordMessagePublisher_ABC& publisher, const WISE_HANDLE& attribute, const CWISEValueUnion& value );
    //@}

protected:
    //! @name Helpers
    //@{
    std::wstring FormatMessage( const std::wstring& message ) const;
    std::wstring GetAttribute( const WISE_HANDLE& handle ) const;
    virtual std::wstring GetIdentifier() const;
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
    const std::wstring identifier_;
    //@}

protected:
    //! @name Member data
    //@{
    mutable WISE_HANDLE handle_;
    mutable std::map< std::wstring, WISE_HANDLE > attributes_;
    //@}
};

#endif // __WiseEntity_h_
