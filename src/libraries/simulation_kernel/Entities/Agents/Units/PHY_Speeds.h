// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
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
    explicit PHY_Speeds( const moving::PHY_RoleAction_Moving& role, bool loaded );
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
    PHY_Speeds& operator=( const PHY_Speeds& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadSpeed          ( xml::xistream& xis, unsigned int timeStepDuration );
    void ReadTerrain        ( xml::xistream& xis );
    void CheckInitialization( xml::xistream& xis, unsigned int timeStepDuration );
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
    //@}
};

#endif // __PHY_Speeds_h_
