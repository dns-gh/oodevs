// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Experience.h $
// $Author: Age $
// $Modtime: 1/12/04 15:39 $
// $Revision: 1 $
// $Workfile: Experience.h $
//
// *****************************************************************************

#ifndef __Experience_h_
#define __Experience_h_

#include "Types.h"
#include "ASN_Types.h"
#include "MT_Tools/MT_Tools_Types.h"

namespace xml { class xistream; };

// =============================================================================
// @class  Experience
// Created: JVT 2004-08-03
// =============================================================================
class Experience
{

public:
    //! @name Types
    //@{
    typedef std::map< std::string, const Experience*, sCaseInsensitiveLess > T_ExperienceMap;
    typedef T_ExperienceMap::const_iterator                                CIT_ExperienceMap;
    //@}

public:
    //! @name 
    //@{
    static const Experience conscrit_;
    static const Experience experimente_;
    static const Experience veteran_;
    //@}

public:
    //! @name Manager
    //@{
    static       void             Initialize( xml::xistream& xis );
    static       void             Terminate ();
    static const Experience*  Find      ( const std::string& strName );
    static const Experience*  Find      ( ASN1T_EnumUnitExperience nAsnID );    
    static const T_ExperienceMap& GetExperiences();
    //@}

    //! @name Accessors
    //@{
    int                      GetID                           () const;
    const std::string&       GetName                         () const;
    ASN1T_EnumUnitExperience GetAsnID                        () const;
    float                    GetCoefMaxSpeedModificator      () const;
    float                    GetCoefReloadingTimeModificator () const;
    float                    GetCoefPhModificator            () const;
    float                    GetCoefPostureTimeModificator   () const;
    float                    GetCoefSensorDistanceModificator() const;
    //@}

    //! @name Operators
    //@{
    bool operator==( const Experience& rhs ) const;
    bool operator!=( const Experience& rhs ) const;
    //@}

private:
    //! @name Types
    //@{
    enum E_ExperienceType
    {
        eVeteran     = 0,
        eExperimente = 1,
        eConscrit    = 2
    };
    //@}

private:
     Experience( const std::string& strName, E_ExperienceType nType, ASN1T_EnumUnitExperience nAsnID );
    ~Experience();

    Experience( const Experience& );
    Experience& operator=( const Experience& );


    //! @name Init
    //@{
    void Read( xml::xistream& xis );
    //@}

private:
    const std::string              strName_;
    const E_ExperienceType         nType_;
    const ASN1T_EnumUnitExperience nAsnID_;

    float rCoefMaxSpeedModificator_;
    float rCoefReloadingTimeModificator_;
    float rCoefPhModificator_;
    float rCoefPostureTimeModificator_;
    float rCoefSensorDistanceModificator_;

private:
    static T_ExperienceMap experiences_;
};

#include "Experience.inl"

#endif // __Experience_h_
