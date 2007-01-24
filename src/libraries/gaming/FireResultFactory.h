// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FireResultFactory_h_
#define __FireResultFactory_h_

#include "network/Simulation_Asn.h"

class AgentFireResult;
class PopulationFireResult;
class Model;

// =============================================================================
/** @class  FireResultFactory
    @brief  FireResultFactory
*/
// Created: AGE 2006-03-10
// =============================================================================
class FireResultFactory
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit FireResultFactory( Model& model );
    virtual ~FireResultFactory();
    //@}

    //! @name Operations
    //@{
    virtual AgentFireResult*      CreateFireResult( const ASN1T_FireDamagesPion& message );
    virtual PopulationFireResult* CreateFireResult( const ASN1T_FireDamagesPopulation& message );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    FireResultFactory( const FireResultFactory& );            //!< Copy constructor
    FireResultFactory& operator=( const FireResultFactory& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    Model& model_;
    //@}
};

#endif // __FireResultFactory_h_
