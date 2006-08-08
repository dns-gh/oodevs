// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FiresModel_h_
#define __FiresModel_h_

#include "astec_kernel/Resolver.h"
#include "ASN_Types.h"

class Agent_ABC;
class Entity_ABC;
class Population_ABC;

// =============================================================================
/** @class  FiresModel
    @brief  FiresModel
*/
// Created: AGE 2006-03-13
// =============================================================================
class FiresModel : public Resolver< Entity_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             FiresModel( const Resolver_ABC< Agent_ABC >& agents, const Resolver_ABC< Population_ABC >& populations );
    virtual ~FiresModel();
    //@}

    //! @name Operations
    //@{
    void Purge();

    void AddFire        ( const ASN1T_MsgStartPionFire& message );
    void AddFire        ( const ASN1T_MsgStartPopulationFire& message );
    Entity_ABC* FindFirer( const ASN1T_MsgStopPionFire& message );
    Entity_ABC* FindFirer( const ASN1T_MsgStopPopulationFire& message );
    void RemoveFire     ( const ASN1T_MsgStopPionFire& message );
    void RemoveFire     ( const ASN1T_MsgStopPopulationFire& message );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    FiresModel( const FiresModel& );            //!< Copy constructor
    FiresModel& operator=( const FiresModel& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const Resolver_ABC< Agent_ABC >& agents_;
    const Resolver_ABC< Population_ABC >& populations_;
    //@}
};

#endif // __FiresModel_h_
