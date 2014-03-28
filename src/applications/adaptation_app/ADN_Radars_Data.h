// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Radars_Data_h_
#define __ADN_Radars_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_RefWithName.h"
#include "ADN_Enums.h"
#include "ADN_Sensors_Modificators.h"
#include "ADN_Types.h"
#include "ADN_Type_Vector_ABC.h"
#include <boost/scoped_ptr.hpp>

// =============================================================================
/** @class  ADN_Radars_Data
    @brief  ADN_Radars_Data
*/
// Created: APE 2005-05-03
// =============================================================================
class ADN_Radars_Data : public ADN_Data_ABC
{
public:
//*****************************************************************************
    class DetectTimes : private boost::noncopyable
    {
    public:
                 DetectTimes();
        virtual ~DetectTimes();

        void ReadArchive( xml::xistream& input, bool bHq );
        void ReadAcquisitionTime( xml::xistream& input, bool bHq );
        void WriteArchive( xml::xostream& output, bool bHq ) const;

    public:
        ADN_Type_Bool bDetectTime_;
        ADN_Type_Time detectTime_;
        ADN_Type_Bool bIdentTime_;
        ADN_Type_Time recoTime_;
        ADN_Type_Bool bRecoTime_;
        ADN_Type_Time identTime_;
    };

//*****************************************************************************
    class RadarInfos : public ADN_RefWithLocalizedName
    {
    public:
                 RadarInfos();
        virtual ~RadarInfos();

        RadarInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void ReadDetectableActivity( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Enum< E_RadarType, eNbrRadarType >  nType_;
        ADN_Type_Double rRange_;
        ADN_Type_Bool bHasMaxHeight_;
        ADN_Type_Double rMaxHeight_;
        ADN_Type_Bool bHasMinHeight_;
        ADN_Type_Double rMinHeight_;
        ADN_Type_Bool bHasDetectableActivities_;
        ADN_Type_Bool detectableActivities_[ eNbrConsumptionType ];
        ADN_Type_Bool bHasDetectTimes_;
        DetectTimes detectTimes_;
        ADN_Type_Bool bHasHQDetectTimes_;
        DetectTimes hqDetectTimes_;
        boost::scoped_ptr< ADN_Sensors_Modificators > modificators_;
    };

    typedef ADN_Type_Vector_ABC< RadarInfos > T_RadarInfos_Vector;

//*****************************************************************************
public:
             ADN_Radars_Data();
    virtual ~ADN_Radars_Data();

    void FilesNeeded( tools::Path::T_Paths& files ) const;

    T_RadarInfos_Vector& GetRadars();
    RadarInfos* FindRadar( const std::string& strName );

public:
    void ReadArchive( xml::xistream& input );
    void ReadRadar( xml::xistream& input );
    void WriteArchive( xml::xostream& output ) const;

public:
    T_RadarInfos_Vector  vRadars_;
};

#endif // __ADN_Radars_Data_h_
