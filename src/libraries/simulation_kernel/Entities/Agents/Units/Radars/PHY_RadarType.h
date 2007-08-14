// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Radars/PHY_RadarType.h $
// $Author: Nld $
// $Modtime: 2/05/05 17:35 $
// $Revision: 1 $
// $Workfile: PHY_RadarType.h $
//
// *****************************************************************************

#ifndef __PHY_RadarType_h_
#define __PHY_RadarType_h_

#include "MIL.h"

class PHY_RadarClass;
class MIL_AgentPion;
class MIL_Agent_ABC;
class PHY_PerceptionLevel;
class MIL_Time_ABC;

// =============================================================================
// @class  PHY_RadarType
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RadarType
{
    MT_COPYNOTALLOWED( PHY_RadarType )

public:
    //! @name Manager
    //@{
    static void Initialize( MIL_InputArchive& archive, const MIL_Time_ABC& time );
    static void Terminate ();

    static const PHY_RadarType* Find( const std::string& strType );
    //@}

    //! @name Operations
    //@{
          bool                  CanAcquire             ( const MIL_AgentPion& perceiver, const MIL_Agent_ABC& target ) const;
    const PHY_PerceptionLevel&  ComputeAcquisitionLevel( const MIL_Agent_ABC& target   , uint nFirstAcquisitionTimeStep ) const;
    //@}

    //! @name Accessors
    //@{
    const std::string&    GetName  () const;
    const PHY_RadarClass& GetClass () const;
          MT_Float        GetRadius() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_RadarType*, sCaseInsensitiveLess > T_RadarTypeMap;
    typedef T_RadarTypeMap::const_iterator                                      CIT_RadarTypeMap;

    typedef std::vector< bool >                                                 T_ActivityVector;   
    //@}

private:
     PHY_RadarType( const std::string& strName, const PHY_RadarClass& type, const MIL_Time_ABC& time, MIL_InputArchive& archive );
    ~PHY_RadarType();

    //! @name Init
    //@{
    void InitializeRange           ( MIL_InputArchive& archive );
    void InitializeActivities      ( MIL_InputArchive& archive );
    void InitializeAcquisitionTimes( MIL_InputArchive& archive );
    //@}
    
private:
    const std::string     strName_;
    const PHY_RadarClass& class_;
    const MIL_Time_ABC&   time_;
    const uint            nID_;

    // Range
    MT_Float rRadius_;
    MT_Float rMinHeight_;
    MT_Float rMaxHeight_;

    T_ActivityVector detectableActivities_;

    // Acquistion times
    MT_Float rDetectionTime_;
    MT_Float rRecognitionTime_;
    MT_Float rIdentificationTime_;
    MT_Float rPcDetectionTime_;
    MT_Float rPcRecognitionTime_;
    MT_Float rPcIdentificationTime_;

private:
    static T_RadarTypeMap radarTypes_;
    static uint           nNextID_;
};

#include "PHY_RadarType.inl"

#endif // __PHY_RadarType_h_
