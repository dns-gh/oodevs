// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Experience.h $
// $Author: Age $
// $Modtime: 1/12/04 15:39 $
// $Revision: 1 $
// $Workfile: MOS_Experience.h $
//
// *****************************************************************************

#ifndef __MOS_Experience_h_
#define __MOS_Experience_h_

#include "MOS_Types.h"
#include "MOS_ASN_Types.h"

// =============================================================================
// @class  MOS_Experience
// Created: JVT 2004-08-03
// =============================================================================
class MOS_Experience
{
    MT_COPYNOTALLOWED( MOS_Experience )

public:
    //! @name Types
    //@{
    typedef std::map< std::string, const MOS_Experience*, sCaseInsensitiveLess > T_ExperienceMap;
    typedef T_ExperienceMap::const_iterator                                      CIT_ExperienceMap;
    //@}

public:
    //! @name 
    //@{
    static const MOS_Experience conscrit_;
    static const MOS_Experience experimente_;
    static const MOS_Experience veteran_;
    //@}

public:
    //! @name Manager
    //@{
    static       void             Initialize( MOS_InputArchive& archive );
    static       void             Terminate ();
    static const MOS_Experience*  Find      ( const std::string& strName );
    static const MOS_Experience*  Find      ( ASN1T_EnumUnitExperience nAsnID );    
    static const T_ExperienceMap& GetExperiences();
    //@}

    //! @name Accessors
    //@{
          int                      GetID                           () const;
    const std::string&             GetName                         () const;
          ASN1T_EnumUnitExperience GetAsnID                        () const;
          MT_Float                 GetCoefMaxSpeedModificator      () const;
          MT_Float                 GetCoefReloadingTimeModificator () const;
          MT_Float                 GetCoefPhModificator            () const;
          MT_Float                 GetCoefPostureTimeModificator   () const;
          MT_Float                 GetCoefSensorDistanceModificator() const;
    //@}

    //! @name Operators
    //@{
    bool operator==( const MOS_Experience& rhs ) const;
    bool operator!=( const MOS_Experience& rhs ) const;
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
     MOS_Experience( const std::string& strName, E_ExperienceType nType, ASN1T_EnumUnitExperience nAsnID );
    ~MOS_Experience();

    //! @name Init
    //@{
    void Read( MOS_InputArchive& archive );
    //@}

private:
    const std::string              strName_;
    const E_ExperienceType         nType_;
    const ASN1T_EnumUnitExperience nAsnID_;

    MT_Float rCoefMaxSpeedModificator_;
    MT_Float rCoefReloadingTimeModificator_;
    MT_Float rCoefPhModificator_;
    MT_Float rCoefPostureTimeModificator_;
    MT_Float rCoefSensorDistanceModificator_;

private:
    static T_ExperienceMap experiences_;
};

#include "MOS_Experience.inl"

#endif // __MOS_Experience_h_
