// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_RADAR_TYPE_H
#define SWORD_PERCEPTION_RADAR_TYPE_H

#include <boost/noncopyable.hpp>
#include <map>
#include <vector>
#include <string>

namespace xml
{
    class xistream;
}

namespace sword
{
namespace wrapper
{
    class View;
}
namespace perception
{
    class RadarClass;
    class PerceptionLevel;

// =============================================================================
/** @class  RadarType
    @brief  Radar type
*/
// Created: SLI 2012-04-12
// =============================================================================
class RadarType : private boost::noncopyable
{
public:
    //! @name Manager
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate ();

    static const RadarType* Find( const std::string& strType );
    //@}

    //! @name Operations
    //@{
          bool              CanAcquire             ( const wrapper::View& perceiver, const wrapper::View& target ) const;
    const PerceptionLevel&  ComputeAcquisitionLevel( const wrapper::View& target   , unsigned int nFirstAcquisitionTimeStep, unsigned int currentTick ) const;
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName  () const;
    const RadarClass&  GetClass () const;
          double       GetRadius() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const RadarType* > T_RadarTypeMap;
    typedef T_RadarTypeMap::const_iterator             CIT_RadarTypeMap;

    typedef std::vector< bool > T_ActivityVector;
    //@}

private:
     RadarType( const std::string& strName, const RadarClass& type, xml::xistream& xis );
    ~RadarType();

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
    static void ReadRadar( xml::xistream& xis );
    //@}

private:
    const std::string strName_;
    const RadarClass& class_;
    const unsigned int nID_;

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

}
}

#endif // SWORD_PERCEPTION_RADAR_TYPE_H
