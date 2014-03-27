// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Sensors_Modificators_h_
#define __ADN_Sensors_Modificators_h_

#include "ADN_CrossedRef.h"
#include "ADN_Type_VectorFixed_ABC.h"
#include "ADN_Urban_Data.h"
#include "ADN_Volumes_Data.h"

class ADN_ConsistencyChecker;

// =============================================================================
/** @class  ADN_Sensors_Modificators
    @brief  ADN_Sensors_Modificators
*/
// Created: JSR 2014-03-27
// =============================================================================
class ADN_Sensors_Modificators
{
public:
    // =============================================================================
    /** @class  SizeInfos
        @brief  SizeInfos
    */
    // Created: JSR 2014-03-27
    // =============================================================================
    class SizeInfos : public ADN_CrossedRef< ADN_Volumes_Data::VolumeInfos >
    {
    public:
        explicit SizeInfos( ADN_Volumes_Data::VolumeInfos* ptr );

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Double rCoeff_;

    public:
        typedef ADN_Volumes_Data::VolumeInfos T_Item;

        class Cmp : public std::unary_function< SizeInfos*, bool >
        {
        public:
            explicit Cmp( const std::string& val ) : val_( val ) {}
            virtual ~Cmp() {}

            bool operator()( SizeInfos* tgtnfos ) const
            {
                return tgtnfos->GetCrossedElement() && tgtnfos->GetCrossedElement()->strName_ == val_;
            }

        private:
            std::string val_;
        };

        class CmpRef : public std::unary_function< SizeInfos*, bool >
        {
        public:
            explicit CmpRef( ADN_Volumes_Data::VolumeInfos* val ) : val_( val ) {}
            virtual ~CmpRef(){}

            bool operator()( SizeInfos* tgtnfos ) const
            {
                return tgtnfos->GetCrossedElement() == val_;
            }

        private:
            ADN_Volumes_Data::VolumeInfos* val_;
        };
    };

    // =============================================================================
    /** @class  IlluminationInfos
        @brief  IlluminationInfos
    */
    // Created: JSR 2014-03-27
    // =============================================================================
    class IlluminationInfos : public ADN_Ref_ABC
    {
    public:
        explicit IlluminationInfos( const E_LightingType& e );

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        E_LightingType eType_;
        ADN_Type_Double rCoeff_;

    public:
        class Cmp : public std::unary_function< IlluminationInfos*, bool >
        {
        public:
            explicit Cmp( const E_LightingType& val ) : val_( val ) {}
            virtual ~Cmp() {}

            bool operator()( IlluminationInfos* tgtnfos ) const
            {
                return tgtnfos->eType_==val_;
            }

        private:
            E_LightingType val_;
        };
    };

    // =============================================================================
    /** @class  MeteoInfos
        @brief  MeteoInfos
    */
    // Created: JSR 2014-03-27
    // =============================================================================
    class MeteoInfos : public ADN_Ref_ABC
    {
    public:
        explicit MeteoInfos( const E_SensorWeatherModifiers& e );

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        E_SensorWeatherModifiers eType_;
        ADN_Type_Double rCoeff_;

    public:
        class Cmp : public std::unary_function< MeteoInfos*, bool >
        {
        public:
            explicit Cmp( const E_SensorWeatherModifiers& val ) : val_( val ) {}
            virtual ~Cmp() {}

            bool operator()( MeteoInfos* tgtnfos ) const
            {
                return tgtnfos->eType_ == val_;
            }

        private:
            E_SensorWeatherModifiers val_;
        };
    };

    // =============================================================================
    /** @class  EnvironmentInfos
        @brief  EnvironmentInfos
    */
    // Created: JSR 2014-03-27
    // =============================================================================
    class EnvironmentInfos : public ADN_Ref_ABC
    {
    public:
        explicit EnvironmentInfos( const E_VisionObject& e );

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        E_VisionObject eType_;
        ADN_Type_Double rCoeff_;

    public:
        class Cmp : public std::unary_function< EnvironmentInfos* , bool >
        {
        public:
            explicit Cmp( const E_VisionObject& val ) : val_( val ) {}
            virtual ~Cmp() {}

            bool operator()( EnvironmentInfos* tgtnfos ) const
            {
                return tgtnfos->eType_ == val_;
            }

        private:
            E_VisionObject val_;
        };
    };


    // =============================================================================
    /** @class  UrbanBlockInfos
        @brief  UrbanBlockInfos
    */
    // Created: JSR 2014-03-27
    // =============================================================================
    class UrbanBlockInfos : public ADN_CrossedRef< ADN_Urban_Data::UrbanMaterialInfos >
    {
    public:
        explicit UrbanBlockInfos( ADN_Urban_Data::UrbanMaterialInfos* ptr );

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
        ADN_Type_Double rCoeff_;

    public:
        typedef ADN_Urban_Data::UrbanMaterialInfos T_Item;

        class Cmp : public std::unary_function< UrbanBlockInfos*, bool >
        {
        public:
            explicit Cmp( const std::string& val ) : val_( val ) {}
            virtual ~Cmp() {}

            bool operator()( UrbanBlockInfos* tgtnfos ) const
            {
                return tgtnfos->GetCrossedElement() && tgtnfos->GetCrossedElement()->strName_ == val_;
            }

        private:
            std::string val_;
        };
        class CmpRef : public std::unary_function< UrbanBlockInfos* , bool >
        {
        public:
            explicit CmpRef( ADN_Urban_Data::UrbanMaterialInfos* val ) : val_( val ) {}
            virtual ~CmpRef() {}

            bool operator()( UrbanBlockInfos* tgtnfos ) const
            {
                return tgtnfos->GetCrossedElement() == val_;
            }

        private:
            ADN_Urban_Data::UrbanMaterialInfos* val_;
        };
    };

public:
             ADN_Sensors_Modificators();
    virtual ~ADN_Sensors_Modificators();

    void CopyFrom( const ADN_Sensors_Modificators& src );

    void ReadSizeModifiers( xml::xistream& xis );
    void ReadMeteoModifiers( xml::xistream& xis );
    void ReadIlluminationModifiers( xml::xistream& xis );
    void ReadEnvironmentModifiers( xml::xistream& xis );
    void ReadUrbanBlocksModifiers( xml::xistream& xis );

    void WriteSizeModifiers( xml::xostream& xos ) const;
    void WriteMeteoModifiers( xml::xostream& xos ) const;
    void WriteIlluminationModifiers( xml::xostream& xos ) const;
    void WriteEnvironmentModifiers( xml::xostream& xos ) const;
    void WriteUrbanBlocksModifiers( xml::xostream& xos ) const;

    void CheckDatabaseValidity( ADN_ConsistencyChecker& checker, const std::string& name ) const;

private:
    typedef ADN_Type_VectorFixed_ABC< SizeInfos > T_SizeInfos_Vector;
    typedef ADN_Type_Vector_ABC< MeteoInfos > T_MeteoInfos_Vector;
    typedef ADN_Type_Vector_ABC< IlluminationInfos > T_IlluminationInfos_Vector;
    typedef ADN_Type_Vector_ABC< EnvironmentInfos > T_EnvironmentInfos_Vector;
    typedef ADN_Type_VectorFixed_ABC< UrbanBlockInfos > T_UrbanBlockInfos_Vector;

public:
    T_SizeInfos_Vector vModifSizes_;
    T_MeteoInfos_Vector vModifWeather_;
    T_IlluminationInfos_Vector vModifIlluminations_;
    T_EnvironmentInfos_Vector vModifEnvironments_;
    T_UrbanBlockInfos_Vector vModifUrbanBlocks_;
};

#endif // __ADN_Sensors_Modificators_h_
