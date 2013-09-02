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

#include "MT_Tools/MT_String.h"

namespace xml
{
    class xistream;
}

class PHY_RadarClass;
class MIL_AgentPion;
class MIL_Agent_ABC;
class PHY_PerceptionLevel;
class MIL_Time_ABC;

// =============================================================================
// @class  PHY_RadarType
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RadarType : private boost::noncopyable
{
public:
    //! @name Manager
    //@{
    static void Initialize( xml::xistream& xis, const MIL_Time_ABC& time );
    static void Terminate ();

    static const PHY_RadarType* Find( const std::string& strType );
    //@}

    //! @name Operations
    //@{
          bool                  CanAcquire             ( const MIL_Agent_ABC& perceiver, const MIL_Agent_ABC& target ) const;
    const PHY_PerceptionLevel&  ComputeAcquisitionLevel( const MIL_Agent_ABC& target   , unsigned int nFirstAcquisitionTimeStep ) const;
    //@}

    //! @name Accessors
    //@{
    const std::string&    GetName  () const;
    const PHY_RadarClass& GetClass () const;
          double        GetRadius() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_RadarType* > T_RadarTypeMap;
    typedef T_RadarTypeMap::const_iterator             CIT_RadarTypeMap;

    typedef std::vector< bool > T_ActivityVector;
    //@}

private:
     PHY_RadarType( const std::string& strName, const PHY_RadarClass& type, const MIL_Time_ABC& time, xml::xistream& xis );
    ~PHY_RadarType();

    //! @name Init
    //@{
    void InitializeRange           ( xml::xistream& xis );
    void InitializeActivities      ( xml::xistream& xis );
    void InitializeAcquisitionTimes( xml::xistream& xis );
    void ReadDetectableActivity    ( xml::xistream& xis, bool& bIsActivity );
    void ReadActivity              ( xml::xistream& xis );
    void ReadAcquisitionTime       ( xml::xistream& xis, bool& bIsTime );
    void ReadTime                  ( xml::xistream& xis, bool& bIsPCTime );
    //@}

    //! @name Helpers
    //@{
    static void ReadRadar( xml::xistream& xis, const MIL_Time_ABC& time );
    //@}

private:
    const std::string     strName_;
    const PHY_RadarClass& class_;
    const MIL_Time_ABC&   time_;
    const unsigned int            nID_;

    // Range
    double rRadius_;
    double rMinHeight_;
    double rMaxHeight_;

    T_ActivityVector detectableActivities_;

    // Acquistion times
    double rDetectionTime_;
    double rRecognitionTime_;
    double rIdentificationTime_;
    double rPcDetectionTime_;
    double rPcRecognitionTime_;
    double rPcIdentificationTime_;

private:
    static T_RadarTypeMap radarTypes_;
    static unsigned int           nNextID_;
};

#endif // __PHY_RadarType_h_
