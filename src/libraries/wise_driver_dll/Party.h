// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Party_h_
#define __Party_h_

#include "WiseEntity.h"

namespace sword { class PartyCreation; }
namespace sword { class ChangeDiplomacy; }

class Model;

// =============================================================================
/** @class  Party
    @brief  Party
*/
// Created: SEB 2010-10-13
// =============================================================================
class Party : public WiseEntity
{

public:
    //! @name Constructors/Destructor
    //@{
             Party( const Model& model, const sword::PartyCreation& message );
    virtual ~Party();
    //@}

    //! @name Operations
    //@{
    virtual void Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime ) const;
    virtual void Destroy( CWISEDriver& driver, const WISE_HANDLE& database ) const;
    void Update( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const sword::ChangeDiplomacy& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Party( const Party& );            //!< Copy constructor
    Party& operator=( const Party& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual std::wstring MakeIdentifier() const;
    //@}

    //! @name Types
    //@{
    struct WiseReference
    {
        WiseReference() : handle_( WISE_INVALID_HANDLE ) {}
        WISE_HANDLE handle_;
        std::map< std::wstring, WISE_HANDLE > attributes_;
    };
    typedef std::map< unsigned long, WiseReference* > T_Diplomacies;
    //@}

private:
    //! @name Member data
    //@{
    const Model& model_;
    const std::wstring name_;
    const unsigned char alignment_;
    T_Diplomacies diplomacies_;
    //@}
};

#endif // __Party_h_
