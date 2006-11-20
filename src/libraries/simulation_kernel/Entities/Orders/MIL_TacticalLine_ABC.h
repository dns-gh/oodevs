// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_TacticalLine_ABC_h_
#define __MIL_TacticalLine_ABC_h_

#include "MIL.h"

#include "Network/NET_ASN_Types.h"

class MIL_Formation;
class MIL_Automate;

//=============================================================================
// Created: NLD 2002-08-06
// Last modified: JVT 03-02-27
//=============================================================================
class MIL_TacticalLine_ABC
{
    MT_COPYNOTALLOWED( MIL_TacticalLine_ABC )

public:
    MIL_TacticalLine_ABC();
    MIL_TacticalLine_ABC( const ASN1T_TacticalLine& asn );
    MIL_TacticalLine_ABC( const MIL_Automate& automateBroadcasted  , MIL_InputArchive& archive );
    MIL_TacticalLine_ABC( const MIL_Formation& formationBroadcasted, MIL_InputArchive& archive );
    virtual ~MIL_TacticalLine_ABC();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()   
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Accessors
    //@{
    uint GetID() const;
    //@}

protected:
    //! @name Initialization
    //@{
    void Initialize( MIL_InputArchive& archive );
    //@}

    //! @name Network
    //@{
    void Serialize              ( ASN1T_TacticalLine& asn ) const;
    void CleanAfterSerialization( ASN1T_TacticalLine& asn ) const;
    void Update                 ( const ASN1T_TacticalLine& asn );
    //@}

private:
    const uint             nID_;
          std::string      strName_;
    const MIL_Formation*   pFormationBroadcasted_;
    const MIL_Automate*    pAutomateBroadcasted_;
          TER_Localisation line_;
};

#include "MIL_TacticalLine_ABC.inl"

#endif // __MIL_TacticalLine_ABC_h_
