//*****************************************************************************
//
// $Created: JDY 03-06-25 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Sensors_Data.h $
// $Author: Nld $
// $Modtime: 3/05/05 14:31 $
// $Revision: 12 $
// $Workfile: ADN_Sensors_Data.h $
//
//*****************************************************************************

#ifndef __ADN_Sensors_Data_h_
#define __ADN_Sensors_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_RefWithName.h"
#include "ADN_Types.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Objects_Data_ObjectInfos.h"
#include "ADN_Disasters_Data.h"
#include "ADN_Sensors_Modificators.h"
#include <boost/scoped_ptr.hpp>

class ADN_Radars_Data;

//*****************************************************************************
// Created: JDY 03-06-25
//*****************************************************************************
class ADN_Sensors_Data : public ADN_Data_ABC
{
public:
// LTO begin
//*****************************************************************************
    class LimitedToSensorsInfos : public ADN_RefWithName
    {
    public:
                 LimitedToSensorsInfos();
        virtual ~LimitedToSensorsInfos();

        LimitedToSensorsInfos* CreateCopy();
        void ReadArchive ( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;
    };

    typedef ADN_Type_Vector_ABC< LimitedToSensorsInfos >  T_LimitedToSensorsInfos_Vector;
// LTO end

//*****************************************************************************
    class PopulationInfos : public ADN_RefWithName
    {
    public:
                 PopulationInfos();
        virtual ~PopulationInfos();

        std::string GetItemName();
        void  CopyFrom( PopulationInfos& populationInfo );

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Double rDensity_;
        ADN_Type_Double rModifier_;
    };

//*****************************************************************************
    class TargetInfos : public ADN_CrossedRef< ADN_Objects_Data_ObjectInfos >
    {
    public:
                 TargetInfos();
        virtual ~TargetInfos();

        TargetInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void ReadPosture( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Double rDistanceDetection_;
        ADN_Type_Vector_ABC< ADN_Sensors_Modificators::PostureInfos > vModifStance_;
        PopulationInfos populationInfos_;
    };

    typedef ADN_Type_Vector_ABC< TargetInfos > T_TargetsInfos_Vector;

//*****************************************************************************

    class DisasterInfos : public ADN_CrossedRef< ADN_Disasters_Data::DisasterInfos >
    {
    public:
                 DisasterInfos();
        virtual ~DisasterInfos();

        DisasterInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Double rDetectionThreshold_;
    };

    typedef ADN_Type_Vector_ABC< DisasterInfos > T_DisasterInfos_Vector;

//*****************************************************************************

    class SensorInfos : public ADN_RefWithLocalizedName
    {
    public:
                 SensorInfos();
        virtual ~SensorInfos();

        SensorInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void ReadLimitedToSensorsList( xml::xistream& input ); // LTO
        void ReadBaseDistance( xml::xistream& input );
        void ReadObject( xml::xistream& input );
        void ReadDisaster( xml::xistream& input );
        void ReadObjectDetection( xml::xistream& input );
        void ReadDisasterDetection( xml::xistream& input );
        void ReadUnitDetection( xml::xistream& input );
        void ReadItem( const std::string& name, xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;
        void CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const;

    public:
        ADN_Type_Bool                           bCanDetectAgents_;
        ADN_Type_Bool                           bCanScan_;
        ADN_Type_Double                         rFirerDetectionRange_;
        ADN_Type_Double                         rAngle_;
        ADN_Type_Bool                           bLimitedToSensors_; // LTO
        ADN_Type_Double                         rDistProximity_;
        ADN_Type_Double                         rDistDetection_;
        ADN_Type_Double                         rDistReco_;
        ADN_Type_Double                         rDistIdent_;
        ADN_Type_Time                           detectionDelay_;
        ADN_Type_Bool                           activatedOnRequest_;
        T_LimitedToSensorsInfos_Vector          vLimitedToSensorsInfos_; // LTO
        boost::scoped_ptr< ADN_Sensors_Modificators > modificators_;
        PopulationInfos                         populationInfos_;

        ADN_Type_Bool                           bCanDetectObjects_;
        T_TargetsInfos_Vector                   vTargets_;
        ADN_Type_Bool                           bCanDetectDisasters_;
        T_DisasterInfos_Vector                  vDisasters_;
    };

    typedef ADN_Type_Vector_ABC<SensorInfos> T_SensorsInfos_Vector;

//*****************************************************************************
    class ALATInfos : private boost::noncopyable
    {
    public:
                 ALATInfos();
        virtual ~ALATInfos();

        void ReadArchive( xml::xistream& input );
        void ReadTime( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Time surveyTimes_[ eNbrVisionObject - 1 ];
    };

//*****************************************************************************
    class CobraInfos : private boost::noncopyable
    {
    public:
                 CobraInfos();
        virtual ~CobraInfos();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Double rRange_;
    };

//*****************************************************************************
public:
             ADN_Sensors_Data();
    virtual ~ADN_Sensors_Data();

    void FilesNeeded( tools::Path::T_Paths& l ) const;
    virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const;
    virtual void LoadTranslations( const tools::Path& xmlFile, kernel::XmlTranslations* translations = 0 );

    T_SensorsInfos_Vector& GetSensorsInfos();
    SensorInfos* FindSensor( const std::string& strName );

    ALATInfos&  GetAlatInfos ();
    CobraInfos& GetCobraInfos();

    QStringList GetSensorsThatUse( ADN_Objects_Data_ObjectInfos& object );
    QStringList GetSensorsThatUse( ADN_Disasters_Data::DisasterInfos& disaster );

private:
    void ReadSensor( xml::xistream& input );
    void ReadArchive( xml::xistream& input );
    void WriteArchive( xml::xostream& output ) const;

public:
    T_SensorsInfos_Vector vSensors_;
    ALATInfos alatInfos_;
    CobraInfos cobraInfos_;
    std::auto_ptr< ADN_Radars_Data > radarData_;
};

//-----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::GetSensorsInfos
// Created: JDY 03-06-30
//-----------------------------------------------------------------------------
inline
ADN_Sensors_Data::T_SensorsInfos_Vector&    ADN_Sensors_Data::GetSensorsInfos()
{
    return vSensors_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::FindSensor
// Created: APE 2004-12-01
// -----------------------------------------------------------------------------
inline
ADN_Sensors_Data::SensorInfos* ADN_Sensors_Data::FindSensor( const std::string& strName )
{
    auto it = std::find_if( vSensors_.begin(), vSensors_.end(), ADN_Tools::NameCmp( strName ) );
    if( it == vSensors_.end() )
        return 0;
    return *it;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::GetAlatInfos
// Created: APE 2005-01-19
// -----------------------------------------------------------------------------
inline
ADN_Sensors_Data::ALATInfos& ADN_Sensors_Data::GetAlatInfos()
{
    return alatInfos_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Data::GetCobraInfos
// Created: NLD 2007-02-13
// -----------------------------------------------------------------------------
inline
ADN_Sensors_Data::CobraInfos& ADN_Sensors_Data::GetCobraInfos()
{
    return cobraInfos_;
}

#endif // __ADN_Sensors_Data_h_
