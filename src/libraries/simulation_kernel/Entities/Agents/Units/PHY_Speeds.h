// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PHY_Speeds_h_
#define __PHY_Speeds_h_

namespace xml
{
    class xistream;
}

namespace moving
{
    class PHY_RoleAction_Moving;
}

class TerrainData;

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
             PHY_Speeds( xml::xistream& xis, unsigned int timeStepDuration );
    explicit PHY_Speeds( const moving::PHY_RoleAction_Moving& role );
             PHY_Speeds( const moving::PHY_RoleAction_Moving& role, bool loaded );
    virtual ~PHY_Speeds();
    //@}

    //! @name Operations
    //@{
    double GetMaxSpeed() const;
    double GetMaxSpeed( const TerrainData& data ) const;
    bool   IsPassable ( const TerrainData& data ) const;
    //@}

    //! @name Accessors
    //@{
    void DumpSpeeds( const TerrainData& data ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PHY_Speeds( const PHY_Speeds& );            //!< Copy constructor
    PHY_Speeds& operator=( const PHY_Speeds& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadSpeed          ( xml::xistream& xis, unsigned int timeStepDuration );
    void ReadTerrain        ( xml::xistream& xis );
    void CheckInitialization( xml::xistream& xis, unsigned int timeStepDuration );
    double& SpeedFor( const TerrainData& data );
    void GenerateMasks();
    //@}

private:
    //! @name Member data
    //@{
    double rMaxSpeed_;
    double rBaseSpeed_;
    double* rAreaSpeeds_  ;
    double* rBorderSpeeds_;
    double* rLinearSpeeds_;

    unsigned short nLinearPassabilityMask_;
    unsigned short nLinearImpassabilityMask_;
    unsigned char  nBorderPassabilityMask_;
    unsigned char  nBorderImpassabilityMask_;
    unsigned char  nAreaPassabilityMask_;
    unsigned char  nAreaImpassabilityMask_;
    //@}
};

#endif // __PHY_Speeds_h_
