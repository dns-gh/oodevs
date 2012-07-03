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

#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

namespace moving
{
    class PHY_RoleAction_InterfaceMoving;
}

class TerrainData;

// =============================================================================
/** @class  PHY_Speeds
    @brief  PHY_Speeds
*/
// Created: AGE 2005-02-02
// =============================================================================
class PHY_Speeds : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_Speeds( xml::xistream& xis, unsigned int timeStepDuration );
    explicit PHY_Speeds( const moving::PHY_RoleAction_InterfaceMoving& role );
             PHY_Speeds( const moving::PHY_RoleAction_InterfaceMoving& role, bool loaded );
    virtual ~PHY_Speeds();
    //@}

    //! @name Operations
    //@{
    double GetMaxSpeed() const;
    double GetMaxSpeed( const TerrainData& data ) const;
    bool   IsPassable ( const TerrainData& data ) const;
    bool HasConstructionSpeeds() const;
    double GetConstructionSpeed( const TerrainData& data ) const;
    //@}

    //! @name Accessors
    //@{
    void DumpSpeeds( const TerrainData& data ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void Initialize( const moving::PHY_RoleAction_InterfaceMoving& role );
    void ReadSpeed          ( xml::xistream& xis, unsigned int timeStepDuration );
    void ReadTerrain        ( xml::xistream& xis );
    void CheckInitialization( xml::xistream& xis, unsigned int timeStepDuration );
    double& SpeedFor( const TerrainData& data );
    void GenerateMasks();
    //@}

private:
    //!@name Types
    //@{
    typedef std::vector< double >     T_Speed;
    typedef T_Speed::iterator        IT_Speed;
    typedef T_Speed::const_iterator CIT_Speed;

    typedef std::map< std::string, double >       T_ConstructionSpeed;
    typedef T_ConstructionSpeed::iterator        IT_ConstructionSpeed;
    typedef T_ConstructionSpeed::const_iterator CIT_ConstructionSpeed;
    //@}

private:
    //! @name Member data
    //@{
    double rMaxSpeed_;
    double rBaseSpeed_;
    T_Speed rAreaSpeeds_;
    T_Speed rBorderSpeeds_;
    T_Speed rLinearSpeeds_;
    T_ConstructionSpeed rConstructionSpeeds_;

    unsigned short nLinearPassabilityMask_;
    unsigned short nLinearImpassabilityMask_;
    unsigned char  nBorderPassabilityMask_;
    unsigned char  nBorderImpassabilityMask_;
    unsigned char  nAreaPassabilityMask_;
    unsigned char  nAreaImpassabilityMask_;
    //@}
};

#endif // __PHY_Speeds_h_
