// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-30 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __FireResult_h_
#define __FireResult_h_

#include "Types.h"
#include "ASN_Types.h"

class Agent_ABC;
class FireResult;

typedef std::vector< FireResult* > T_FireResults;
typedef T_FireResults::const_iterator  CIT_FireResults;

// =============================================================================
// Created: SBO 2005-08-30
// =============================================================================
class FireResult
{
    MT_COPYNOTALLOWED( FireResult );

public:
    //! @name Types
    //@{
    typedef std::vector< uint >                    T_FireResultHuman;

    typedef std::vector< std::string >             T_FireResultEquipments;
    typedef T_FireResultEquipments::const_iterator CIT_FireResultEquipments;

    typedef std::map< uint, T_FireResultHuman* >   T_FireResultHumans;
    typedef T_FireResultHumans::const_iterator     CIT_FireResultHumans;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit FireResult( const ASN1T_FireDamagesPion&       asnMsg );
    explicit FireResult( const ASN1T_FireDamagesPopulation& asnMsg );
    virtual ~FireResult();
    //@}

    //! @name Accessors
    //@{
    const Agent_ABC&          GetTarget    () const;

    // target = agent
    const T_FireResultEquipments& GetEquipments() const;
    const T_FireResultHumans&     GetHumans    () const;

    // target = population
          uint                    GetDead      () const;
    //@}

private:
    //! @name Member data
    //@{
    const Agent_ABC*       pTarget_;

    // target = agent
    T_FireResultEquipments     equipments_;
    T_FireResultHumans         humans_;

    // target = population
    uint                       nDead_;
    //@}
};

#include "FireResult.inl"

#endif // __FireResult_h_
