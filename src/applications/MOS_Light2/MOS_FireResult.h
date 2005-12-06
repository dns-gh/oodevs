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

#ifndef __MOS_FireResult_h_
#define __MOS_FireResult_h_

#include "MOS_Types.h"
#include "MOS_ASN_Types.h"

class MOS_Agent_ABC;
class MOS_FireResult;

typedef std::vector< MOS_FireResult* > T_FireResults;
typedef T_FireResults::const_iterator  CIT_FireResults;

// =============================================================================
// Created: SBO 2005-08-30
// =============================================================================
class MOS_FireResult
{
    MT_COPYNOTALLOWED( MOS_FireResult );

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
    explicit MOS_FireResult( const ASN1T_FireDamagesPion&       asnMsg );
    explicit MOS_FireResult( const ASN1T_FireDamagesPopulation& asnMsg );
    virtual ~MOS_FireResult();
    //@}

    //! @name Accessors
    //@{
    const MOS_Agent_ABC&          GetTarget    () const;

    // target = agent
    const T_FireResultEquipments& GetEquipments() const;
    const T_FireResultHumans&     GetHumans    () const;

    // target = population
          uint                    GetDead      () const;
    //@}

private:
    //! @name Member data
    //@{
    const MOS_Agent_ABC*       pTarget_;

    // target = agent
    T_FireResultEquipments     equipments_;
    T_FireResultHumans         humans_;

    // target = population
    uint                       nDead_;
    //@}
};

#include "MOS_FireResult.inl"

#endif // __MOS_FireResult_h_
