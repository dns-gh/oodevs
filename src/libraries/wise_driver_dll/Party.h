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

namespace sword
{
    class ChangeDiplomacy;
    class ChangeDiplomacyAck;
    class PartyCreation;
}

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
    void Update( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const sword::ChangeDiplomacy& message );
    void Update( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const sword::ChangeDiplomacyAck& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Party( const Party& );            //!< Copy constructor
    Party& operator=( const Party& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    template< typename M >
    void DoUpdate( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const M& message );
    //@}

    //! @name Types
    //@{
    typedef std::map< WISE_HANDLE, unsigned char > T_Alliances;
    //@}

private:
    //! @name Member data
    //@{
    const Model& model_;
    const std::wstring name_;
    const unsigned char alignment_;
    T_Alliances alliances_;
    //@}
};

#endif // __Party_h_
