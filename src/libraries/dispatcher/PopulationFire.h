// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationFire_h_
#define __PopulationFire_h_

#include "Entity_ABC.h"
#include "game_asn/Asn.h"

namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  PopulationFire
    @brief  PopulationFire
*/
// Created: AGE 2007-04-18
// =============================================================================
class PopulationFire : public Entity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationFire( Model& model, const ASN1T_MsgStartPopulationFire& msg );
    virtual ~PopulationFire();
    //@}

    //! @name Operations
    //@{
    void Update( const ASN1T_MsgStartPopulationFire& msg );
    virtual void SendFullUpdate( Publisher_ABC& publisher ) const;
    virtual void SendCreation  ( Publisher_ABC& publisher ) const;
    virtual void CommitDestruction();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationFire( const PopulationFire& );            //!< Copy constructor
    PopulationFire& operator=( const PopulationFire& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    ASN1T_MsgStartPopulationFire msg_;
    //@}
};

}

#endif // __PopulationFire_h_
