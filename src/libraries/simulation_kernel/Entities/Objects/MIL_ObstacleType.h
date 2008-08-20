// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_ObstacleType_h_
#define __MIL_ObstacleType_h_

#include "MIL.h"

#include "game_asn/Simulation.h"

// =============================================================================
// @class  MIL_ObstacleType
// Created: JVT 2004-08-03
// =============================================================================
class MIL_ObstacleType
{
    MT_COPYNOTALLOWED( MIL_ObstacleType )

public:
    //! @name Statics
    //@{
    static const MIL_ObstacleType reserved_;          
    static const MIL_ObstacleType initial_;                 
    //@}

public:
    //! @name Manager
    //@{
    static const MIL_ObstacleType* Find( const std::string& strName );
    static const MIL_ObstacleType* Find( uint nID );
    static const MIL_ObstacleType* Find( ASN1T_EnumObstacleType asn );
    //@}

    //! @name Accessors
    //@{
          uint                   GetID   () const;
          ASN1T_EnumObstacleType GetAsnID() const;
    const std::string&           GetName () const;
    //@}

    //! @name Operations
    //@{
    bool operator==( const MIL_ObstacleType& rhs ) const;
    bool operator!=( const MIL_ObstacleType& rhs ) const;
    bool CouldBeActivated() const;
    //@}

private:
    //! @name Types
    //@{
    enum E_Type
    {
        eInitial  = 0,
        eReserved = 1, // De manoeuvre
    };
    //@}

private:
            MIL_ObstacleType( const std::string& strName, E_Type nType, ASN1T_EnumObstacleType nAsnID );
    virtual ~MIL_ObstacleType();

private:
    const E_Type                  nType_;
    const ASN1T_EnumObstacleType  nAsnID_;
    const std::string             strName_;
};

#include "MIL_ObstacleType.inl"

#endif // __MIL_ObstacleType_h_
