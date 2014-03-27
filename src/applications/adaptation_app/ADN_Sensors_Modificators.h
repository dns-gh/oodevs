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

// =============================================================================
/** @class  ModificatorSizeInfos
    @brief  ModificatorSizeInfos
*/
// Created: JSR 2014-03-27
// =============================================================================
class ModificatorSizeInfos : public ADN_CrossedRef< ADN_Volumes_Data::VolumeInfos >
{
public:
    explicit ModificatorSizeInfos( ADN_Volumes_Data::VolumeInfos* ptr );

    void ReadArchive( xml::xistream& input );
    void WriteArchive( xml::xostream& output ) const;

public:
    ADN_Type_Double rCoeff_;

public:
    typedef ADN_Volumes_Data::VolumeInfos T_Item;

    class Cmp : public std::unary_function< ModificatorSizeInfos*, bool >
    {
    public:
        explicit Cmp( const std::string& val ) : val_( val ) {}
        virtual ~Cmp() {}

        bool operator()( ModificatorSizeInfos* tgtnfos ) const
        {
            return tgtnfos->GetCrossedElement() && tgtnfos->GetCrossedElement()->strName_ == val_;
        }

    private:
        std::string val_;
    };

    class CmpRef : public std::unary_function< ModificatorSizeInfos*, bool >
    {
    public:
        explicit CmpRef( ADN_Volumes_Data::VolumeInfos* val ) : val_( val ) {}
        virtual ~CmpRef(){}

        bool operator()( ModificatorSizeInfos* tgtnfos ) const
        {
            return tgtnfos->GetCrossedElement() == val_;
        }

    private:
        ADN_Volumes_Data::VolumeInfos* val_;
    };
};

// =============================================================================
/** @class  ModificatorIlluminationInfos
    @brief  ModificatorIlluminationInfos
*/
// Created: JSR 2014-03-27
// =============================================================================
class ModificatorIlluminationInfos : public ADN_Ref_ABC
{
public:
    explicit ModificatorIlluminationInfos( const E_LightingType& e );

    void ReadArchive( xml::xistream& input );
    void WriteArchive( xml::xostream& output ) const;

public:
    E_LightingType eType_;
    ADN_Type_Double rCoeff_;

public:
    class Cmp : public std::unary_function< ModificatorIlluminationInfos*, bool >
    {
    public:
        explicit Cmp( const E_LightingType& val ) : val_( val ) {}
        virtual ~Cmp() {}

        bool operator()( ModificatorIlluminationInfos* tgtnfos ) const
        {
            return tgtnfos->eType_==val_;
        }

    private:
        E_LightingType val_;
    };
};

// =============================================================================
/** @class  ModificatorMeteoInfos
    @brief  ModificatorMeteoInfos
*/
// Created: JSR 2014-03-27
// =============================================================================
class ModificatorMeteoInfos : public ADN_Ref_ABC
{
public:
    explicit ModificatorMeteoInfos( const E_SensorWeatherModifiers& e );

    void ReadArchive( xml::xistream& input );
    void WriteArchive( xml::xostream& output ) const;

public:
    E_SensorWeatherModifiers eType_;
    ADN_Type_Double rCoeff_;

public:
    class Cmp : public std::unary_function< ModificatorMeteoInfos*, bool >
    {
    public:
        explicit Cmp( const E_SensorWeatherModifiers& val ) : val_( val ) {}
        virtual ~Cmp() {}

        bool operator()( ModificatorMeteoInfos* tgtnfos ) const
        {
            return tgtnfos->eType_ == val_;
        }

    private:
        E_SensorWeatherModifiers val_;
    };
};

// =============================================================================
/** @class  ModificatorEnvironmentInfos
    @brief  ModificatorEnvironmentInfos
*/
// Created: JSR 2014-03-27
// =============================================================================
class ModificatorEnvironmentInfos : public ADN_Ref_ABC
{
public:
    explicit ModificatorEnvironmentInfos( const E_VisionObject& e );

    void ReadArchive( xml::xistream& input );
    void WriteArchive( xml::xostream& output ) const;

public:
    E_VisionObject eType_;
    ADN_Type_Double rCoeff_;

public:
    class Cmp : public std::unary_function< ModificatorEnvironmentInfos* , bool >
    {
    public:
        explicit Cmp( const E_VisionObject& val ) : val_( val ) {}
        virtual ~Cmp() {}

        bool operator()( ModificatorEnvironmentInfos* tgtnfos ) const
        {
            return tgtnfos->eType_ == val_;
        }

    private:
        E_VisionObject val_;
    };
};


// =============================================================================
/** @class  ModificatorUrbanBlockInfos
    @brief  ModificatorUrbanBlockInfos
*/
// Created: JSR 2014-03-27
// =============================================================================
class ModificatorUrbanBlockInfos : public ADN_CrossedRef< ADN_Urban_Data::UrbanMaterialInfos >
{
public:
    explicit ModificatorUrbanBlockInfos( ADN_Urban_Data::UrbanMaterialInfos* ptr );

    void ReadArchive( xml::xistream& input );
    void WriteArchive( xml::xostream& output ) const;

public:
    ADN_Type_Double rCoeff_;

public:
    typedef ADN_Urban_Data::UrbanMaterialInfos T_Item;

    class Cmp : public std::unary_function< ModificatorUrbanBlockInfos*, bool >
    {
    public:
        explicit Cmp( const std::string& val ) : val_( val ) {}
        virtual ~Cmp() {}

        bool operator()( ModificatorUrbanBlockInfos* tgtnfos ) const
        {
            return tgtnfos->GetCrossedElement() && tgtnfos->GetCrossedElement()->strName_ == val_;
        }

    private:
        std::string val_;
    };
    class CmpRef : public std::unary_function< ModificatorUrbanBlockInfos* , bool >
    {
    public:
        explicit CmpRef( ADN_Urban_Data::UrbanMaterialInfos* val ) : val_( val ) {}
        virtual ~CmpRef() {}

        bool operator()( ModificatorUrbanBlockInfos* tgtnfos ) const
        {
            return tgtnfos->GetCrossedElement() == val_;
        }

    private:
        ADN_Urban_Data::UrbanMaterialInfos* val_;
    };
};

typedef ADN_Type_VectorFixed_ABC< ModificatorSizeInfos > T_ModificatorSizeInfos_Vector;
typedef ADN_Type_Vector_ABC< ModificatorIlluminationInfos >  T_ModificatorIlluminationInfos_Vector;
typedef ADN_Type_Vector_ABC< ModificatorMeteoInfos > T_ModificatorMeteoInfos_Vector;
typedef ADN_Type_Vector_ABC< ModificatorEnvironmentInfos > T_ModificatorEnvironmentInfos_Vector;
typedef ADN_Type_VectorFixed_ABC< ModificatorUrbanBlockInfos > T_ModificatorUrbanBlockInfos_Vector;

#endif // __ADN_Sensors_Modificators_h_
