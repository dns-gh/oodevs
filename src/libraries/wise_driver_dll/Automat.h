// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Automat_h_
#define __Automat_h_

#include "Entity_ABC.h"

namespace MsgsSimToClient { class MsgAutomatCreation; }

class Model;

// =============================================================================
/** @class  Automat
    @brief  Automat
*/
// Created: SEB 2010-10-13
// =============================================================================
class Automat : public Entity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Automat( const Model& model, const MsgsSimToClient::MsgAutomatCreation& message );
    virtual ~Automat();
    //@}

    //! @name Accessors
    //@{
    virtual unsigned long GetId() const;
    virtual WISE_HANDLE GetHandle() const;
    //@}

    //! @name Operations
    //@{
    virtual void Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime ) const;
    virtual void Destroy( CWISEDriver& driver, const WISE_HANDLE& database ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Automat( const Automat& );            //!< Copy constructor
    Automat& operator=( const Automat& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    std::wstring MakeIdentifier() const;
    //@}

private:
    //! @name Member data
    //@{
    const unsigned long id_;
    const std::wstring name_;
    const unsigned long type_;
    const Entity_ABC* party_;
    const Entity_ABC* superior_;
    mutable WISE_HANDLE handle_;
    //@}
};

#endif // __Automat_h_
