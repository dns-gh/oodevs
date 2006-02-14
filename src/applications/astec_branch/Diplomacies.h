// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Diplomacies_h_
#define __Diplomacies_h_

#include "ASN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "Resolver_ABC.h"

class Team;

// =============================================================================
/** @class  Diplomacies
    @brief  Diplomacies
*/
// Created: AGE 2006-02-14
// =============================================================================
class Diplomacies : public Extension_ABC
                  , public Updatable_ABC< ASN1T_MsgChangeDiplomatieAck >
                  , public Updatable_ABC< ASN1T_MsgChangeDiplomatie >
{

public:
    //! @name Constructors/Destructor
    //@{
             Diplomacies( const Resolver_ABC< Team >& resolver );
    virtual ~Diplomacies();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Diplomacies( const Diplomacies& );            //!< Copy constructor
    Diplomacies& operator=( const Diplomacies& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::map< const Diplomacies*, ASN1T_EnumDiplomatie > T_Diplomacies;
    //@}

    //! @name Helpers
    //@{
    virtual void Update( const ASN1T_MsgChangeDiplomatieAck& message );
    virtual void Update( const ASN1T_MsgChangeDiplomatie& message );
    template< typename T >
    void DoUpdate( const T& message );
    //@}

private:
    //! @name Member data
    //@{
    const Resolver_ABC< Team >& resolver_;
    T_Diplomacies diplomacies_;
    //@}
};

#endif // __Diplomacies_h_
