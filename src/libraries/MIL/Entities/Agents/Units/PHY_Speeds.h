// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-02-02 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/PHY_Speeds.h $
// $Author: Age $
// $Modtime: 22/03/05 11:02 $
// $Revision: 6 $
// $Workfile: PHY_Speeds.h $
//
// *****************************************************************************

#ifndef __PHY_Speeds_h_
#define __PHY_Speeds_h_

#include "MIL.h"

class PHY_RoleAction_Moving;

// =============================================================================
/** @class  PHY_Speeds
    @brief  PHY_Speeds
*/
// Created: AGE 2005-02-02
// =============================================================================
class PHY_Speeds
{

public:
    //! @name Constructors/Destructor
    //@{
             PHY_Speeds( MIL_InputArchive& archive );
             PHY_Speeds( const PHY_RoleAction_Moving& role );
    virtual ~PHY_Speeds();
    //@}

    //! @name Operations
    //@{
    MT_Float GetMaxSpeed() const;
    MT_Float GetMaxSpeed( const TerrainData& data ) const;
    bool     IsPassable( const TerrainData& data ) const;
    //@}

    //! @name Accessors
    //@{
    void DumpSpeeds( const TerrainData& data ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PHY_Speeds( const PHY_Speeds& );            //!< Copy constructor
    PHY_Speeds& operator=( const PHY_Speeds& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ReadArchive( MIL_InputArchive& archive );
    void CheckInitialization( MIL_InputArchive& archive );
    MT_Float& SpeedFor( const TerrainData& data );
    void GenerateMasks();
    //@}

private:
    //! @name Member data
    //@{
    MT_Float rMaxSpeed_;

    MT_Float  rBaseSpeed_;
    MT_Float* rAreaSpeeds_  ;
    MT_Float* rBorderSpeeds_;
    MT_Float* rLinearSpeeds_;

    unsigned short nLinearPassabilityMask_;
    unsigned char  nAreaPassabilityMask_;
    unsigned char  nAreaImpassabilityMask_;
    unsigned char  nBorderImpassabilityMask_;
    unsigned short nLinearImpassabilityMask_;

    static const TerrainData areas_  [ 7  ];
    static const TerrainData borders_[ 7  ];
    static const TerrainData linears_[ 11 ];
    //@}
};

#endif // __PHY_Speeds_h_
