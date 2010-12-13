// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Formation_h_
#define __Formation_h_

#include "WiseEntity.h"

namespace MsgsSimToClient { class MsgFormationCreation; }

class Model;

// =============================================================================
/** @class  Formation
    @brief  Formation
*/
// Created: SEB 2010-10-13
// =============================================================================
class Formation : public WiseEntity
{

public:
    //! @name Constructors/Destructor
    //@{
             Formation( const Model& model, const MsgsSimToClient::MsgFormationCreation& message );
    virtual ~Formation();
    //@}

    //! @name Operations
    //@{
    virtual void Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Formation( const Formation& );            //!< Copy constructor
    Formation& operator=( const Formation& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual std::wstring MakeIdentifier() const;
    //@}

private:
    //! @name Member data
    //@{
    const std::wstring name_;
    const unsigned char echelon_;
    const Entity_ABC* party_;
    const Entity_ABC* superior_;
    //@}
};

#endif // __Formation_h_
