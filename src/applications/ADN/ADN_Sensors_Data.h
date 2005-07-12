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

#include "ADN_Types.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Type_VectorFixed_ABC.h"
#include "ADN_Categories_Data.h"
#include "ADN_Objects_Data.h"
#include "ADN_DataTreeNode_ABC.h"
#include "ADN_Radars_Data.h"


//*****************************************************************************
// Created: JDY 03-06-25
//*****************************************************************************
class ADN_Sensors_Data : public ADN_Data_ABC
{
    MT_COPYNOTALLOWED( ADN_Sensors_Data )

public:
//*****************************************************************************
    class ModificatorSizeInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( ModificatorSizeInfos )

    public:
        ModificatorSizeInfos(ADN_Categories_Data::SizeInfos* ptr);

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );
    
    public:
        ADN_TypePtr_InVector_ABC<ADN_Categories_Data::SizeInfos>       ptrSize_;
        ADN_Type_Double                                                rCoeff_;
        
    public:
        typedef ADN_Categories_Data::SizeInfos                  T_Item;

        class Cmp : public std::unary_function< ModificatorSizeInfos* , bool >
        {
        public:
            Cmp(const std::string& val) : val_(val) {}
            ~Cmp() {}

            bool operator()( ModificatorSizeInfos* tgtnfos ) const 
            {   return tgtnfos->ptrSize_.GetData() && tgtnfos->ptrSize_.GetData()->GetData()==val_;}

        private:
            std::string val_;
        };

        class CmpRef : public std::unary_function< ModificatorSizeInfos* , bool >
        {
        public:
            CmpRef(ADN_Categories_Data::SizeInfos* val) : val_(val) {}
            ~CmpRef(){}

            bool operator()( ModificatorSizeInfos* tgtnfos ) const 
            {   return tgtnfos->ptrSize_.GetData() == val_;}

        private:
            ADN_Categories_Data::SizeInfos* val_;
        };
    };

    typedef ADN_Type_VectorFixed_ABC<ModificatorSizeInfos> T_ModificatorSizeInfos_Vector;
    typedef T_ModificatorSizeInfos_Vector::iterator        IT_ModificatorSizeInfos_Vector;


//*****************************************************************************
    class ModificatorIlluminationInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( ModificatorIlluminationInfos )

    public:
        ModificatorIlluminationInfos(const E_TimeCategory& e);

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );

    public:
        E_TimeCategory      eType_;
        ADN_Type_Double     rCoeff_;

    public:
        class Cmp : public std::unary_function< ModificatorIlluminationInfos* , bool >
        {
        public:
            Cmp(const E_TimeCategory& val) : val_(val) {}
            ~Cmp() {}

            bool operator()( ModificatorIlluminationInfos* tgtnfos ) const 
            { return tgtnfos->eType_==val_; }

        private:
            E_TimeCategory val_;
        };
    };

    typedef ADN_Type_Vector_ABC<ModificatorIlluminationInfos > T_ModificatorIlluminationInfos_Vector;
    typedef T_ModificatorIlluminationInfos_Vector::iterator    IT_ModificatorIlluminationInfos_Vector;

    
//*****************************************************************************
    class ModificatorMeteoInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( ModificatorMeteoInfos )

    public:
        ModificatorMeteoInfos(const E_SensorWeatherModifiers& e);

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );

    public:
        E_SensorWeatherModifiers       eType_;
        ADN_Type_Double     rCoeff_;

    public:
        class Cmp : public std::unary_function< ModificatorMeteoInfos* , bool >
        {
        public:
            Cmp(const E_SensorWeatherModifiers& val) : val_(val) {}
            ~Cmp() {}

            bool operator()( ModificatorMeteoInfos* tgtnfos ) const 
            { return tgtnfos->eType_==val_; }

        private:
            E_SensorWeatherModifiers val_;
        };
    };

    typedef ADN_Type_Vector_ABC<ModificatorMeteoInfos>        T_ModificatorMeteoInfos_Vector;
    typedef T_ModificatorMeteoInfos_Vector::iterator          IT_ModificatorMeteoInfos_Vector;
    
    
//*****************************************************************************
    class ModificatorEnvironmentInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( ModificatorEnvironmentInfos )

    public:
        ModificatorEnvironmentInfos(const E_VisionObject& e);

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );

    public:
        E_VisionObject      eType_;
        ADN_Type_Double     rCoeff_;

    public:
        class Cmp : public std::unary_function< ModificatorEnvironmentInfos* , bool >
        {
        public:
            Cmp(const E_VisionObject& val) : val_(val) {}
            ~Cmp() {}

            bool operator()( ModificatorEnvironmentInfos* tgtnfos ) const 
            { return tgtnfos->eType_==val_; }

        private:
            E_VisionObject val_;
        };
    };

    typedef ADN_Type_Vector_ABC<ModificatorEnvironmentInfos>  T_ModificatorEnvironmentInfos_Vector;
    typedef T_ModificatorEnvironmentInfos_Vector::iterator    IT_ModificatorEnvironmentInfos_Vector;


//*****************************************************************************
    class ModificatorPostureInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( ModificatorPostureInfos )

    public:
        ModificatorPostureInfos(const E_UnitPosture& e );

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );

    public:
        E_UnitPosture       eType_;
        ADN_Type_Double     rCoeff_;

    public:
        class Cmp : public std::unary_function< ModificatorPostureInfos* , bool >
        {
        public:
            Cmp(const E_UnitPosture& val) : val_(val) {}
            ~Cmp() {}

            bool operator()( ModificatorPostureInfos* tgtnfos ) const 
            { return tgtnfos->eType_==val_; }

        private:
            E_UnitPosture val_;
        };
    };

    typedef ADN_Type_Vector_ABC<ModificatorPostureInfos>  T_ModificatorPostureInfos_Vector;
    typedef T_ModificatorPostureInfos_Vector::iterator    IT_ModificatorPostureInfos_Vector;


//*****************************************************************************
    class TargetInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( TargetInfos )

    public:
        TargetInfos();
        ~TargetInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        TargetInfos* CreateCopy();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );

    public:
        ADN_TypePtr_InVector_ABC<ADN_Objects_Data::ObjectInfos>   ptrObject_;
        ADN_Type_String                                           strName_;  //!< For code convenience, not for editing.
        ADN_Type_Double                                           rDistanceDetection_;
        T_ModificatorPostureInfos_Vector                          vModifStance_;

    public:
        class Cmp : public std::unary_function< TargetInfos* , bool >
        {
        public:
            Cmp(const E_ObjectType& val) : val_(val) {}
            ~Cmp() {}

            bool operator()( TargetInfos* tgtnfos ) const
            {   return tgtnfos->ptrObject_.GetData() && tgtnfos->ptrObject_.GetData()->nObjectType_.GetData()==val_;}

        private:
            E_ObjectType val_;
        };
    };   

    typedef ADN_Type_Vector_ABC<TargetInfos> T_TargetsInfos_Vector;
    typedef T_TargetsInfos_Vector::iterator  IT_TargetsInfos_Vector;


//*****************************************************************************
    class SensorInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( SensorInfos )

    public:
        SensorInfos();
        ~SensorInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();
    
        SensorInfos* CreateCopy();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );
    
    public:
        ADN_Type_String                         strName_;

        ADN_Type_Bool                           bCanDetectAgents_;
        ADN_Type_Bool                           bCanScan_;
        ADN_Type_Double                         rAngle_;
        ADN_Type_Double                         rDistProximity_;
        ADN_Type_Double                         rDistDetection_;
        ADN_Type_Double                         rDistReco_;
        ADN_Type_Double                         rDistIdent_;
        T_ModificatorSizeInfos_Vector           vModifSizes_;
        T_ModificatorMeteoInfos_Vector          vModifWeather_;
        T_ModificatorIlluminationInfos_Vector   vModifIlluminations_;
        T_ModificatorPostureInfos_Vector        vModifStance_;
        T_ModificatorPostureInfos_Vector        vModifTargetStance_;
        T_ModificatorEnvironmentInfos_Vector    vModifEnvironments_;


        ADN_Type_Bool                           bCanDetectObjects_;
        T_TargetsInfos_Vector                   vTargets_;
    };
    
    typedef ADN_Type_Vector_ABC<SensorInfos> T_SensorsInfos_Vector;
    typedef T_SensorsInfos_Vector::iterator  IT_SensorsInfos_Vector;


//*****************************************************************************
    class ALATInfos
    {
        MT_COPYNOTALLOWED( ALATInfos )

    public:
        ALATInfos();
        ~ALATInfos();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );

    public:
        ADN_Type_Double surveyTimes_[eNbrVisionObjects-1];
    };



//*****************************************************************************
public:
    ADN_Sensors_Data();
    virtual ~ADN_Sensors_Data();

    void            FilesNeeded(T_StringList& l) const;
    void            Reset();

    
    T_SensorsInfos_Vector& GetSensorsInfos();
    SensorInfos*           FindSensor( const std::string& strName );

    ALATInfos& GetAlatInfos();

private:
    void ReadArchive( ADN_XmlInput_Helper& input );
    void WriteArchive( MT_OutputArchive_ABC& output );

public:
    T_SensorsInfos_Vector  vSensors_;

    ALATInfos              alatInfos_;

    ADN_Radars_Data&       radarData_;
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
    IT_SensorsInfos_Vector it = std::find_if( vSensors_.begin(), vSensors_.end(), ADN_Tools::NameCmp<SensorInfos>( strName ) );
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


#endif // __ADN_Sensors_Data_h_