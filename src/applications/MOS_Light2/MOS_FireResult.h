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

class MOS_Agent;
class MOS_FireResult;

typedef std::vector< MOS_FireResult* > T_FireResults;
typedef T_FireResults::const_iterator  CIT_FireResults;

// =============================================================================
/** @class  MOS_FireResult
    @brief  MOS_FireResult
    @par    Using example
    @code
    MOS_FireResult;
    @endcode
*/
// Created: SBO 2005-08-30
// =============================================================================
class MOS_FireResult
{
    MT_COPYNOTALLOWED( MOS_FireResult );

public:
    //! @name Types
    //@{
    typedef struct
    {
        int nBlessesUrgence1;
        int nBlessesUrgence2;
        int nBlessesUrgence3;
        int nBlessesUrgenceExtreme;
        int nMorts;
        int nNonBlesses;

    } T_FireResultHuman;

    typedef std::vector< std::string >             T_FireResultEquipments;
    typedef T_FireResultEquipments::const_iterator CIT_FireResultEquipments;

    typedef std::map< uint, T_FireResultHuman* >   T_FireResultHumans;
    typedef T_FireResultHumans::const_iterator     CIT_FireResultHumans;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             MOS_FireResult();
    virtual ~MOS_FireResult();
    //@}

    //! @name Operations
    //@{
    void Initialize( const ASN1T_FireResult& asnMsg );
    //@}

    //! @name Accessors
    //@{
    const MOS_Agent&              GetTarget    () const;
    const T_FireResultEquipments& GetEquipments() const;
    const T_FireResultHumans&     GetHumans    () const;
    //@}

private:
    //! @name Member data
    //@{
    MOS_Agent*                 pTarget_;
    T_FireResultEquipments     equipments_;
    T_FireResultHumans         humans_;
    //@}
};

#include "MOS_FireResult.inl"

#endif // __MOS_FireResult_h_
