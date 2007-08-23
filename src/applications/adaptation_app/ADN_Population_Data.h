// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-12-02 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Population_Data.h $
// $Author: Ape $
// $Modtime: 7/06/05 10:08 $
// $Revision: 9 $
// $Workfile: ADN_Population_Data.h $
//
// *****************************************************************************

#ifndef __ADN_Population_Data_h_
#define __ADN_Population_Data_h_

#include "ADN_Data_ABC.h"

#include "ADN_Types.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Type_VectorFixed_ABC.h"
#include "ADN_Models_Data.h"
#include "ADN_Categories_Data.h"

class xml::xistream;


// =============================================================================
/** @class  ADN_Population_Data
*/
// Created: APE 2004-12-02
// =============================================================================
class ADN_Population_Data : public ADN_Data_ABC
{
    MT_COPYNOTALLOWED( ADN_Population_Data )

public:
// *****************************************************************************
    class FireEffectProtectionInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( FireEffectProtectionInfos )

    public:
         FireEffectProtectionInfos( ADN_Categories_Data::ArmorInfos* ptr );
        ~FireEffectProtectionInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_Type_String                                           strName_;
        ADN_TypePtr_InVector_ABC<ADN_Categories_Data::ArmorInfos> ptrProtection_;
        ADN_Type_Double                                           rDestruction_;
        ADN_Type_Double                                           rFixableWithEvacuation_;
        ADN_Type_Double                                           rFixableWithoutEvacuation_;

    public:
        typedef ADN_Categories_Data::ArmorInfos T_Item;

        class CmpRef : public std::unary_function< FireEffectProtectionInfos* , bool >
        {
        public:
            CmpRef(ADN_Categories_Data::ArmorInfos* val) : val_(val) {}
            ~CmpRef(){}

            bool operator()( FireEffectProtectionInfos* tgtnfos ) const 
            {   return tgtnfos->ptrProtection_.GetData() == val_;}

        private:
            ADN_Categories_Data::ArmorInfos* val_;
        };
    };

    typedef ADN_Type_VectorFixed_ABC<FireEffectProtectionInfos>  T_FireEffectProtectionInfosVector;
    typedef T_FireEffectProtectionInfosVector::iterator          IT_FireEffectProtectionInfosVector;

// *****************************************************************************
    class FireEffectInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( FireEffectInfos )

    public:
         FireEffectInfos( E_PopulationAttitude nAttitude );
        ~FireEffectInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( xml::xistream& input );
        void ReadProtection( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_Type_String                   strName_;
        E_PopulationAttitude              nAttitude_;
        ADN_Type_Double                   rIntensityDensity_;
        ADN_Type_Double                   rIntensityFactor_;
        T_FireEffectProtectionInfosVector vProtectionInfos_;

    public:
        class CmpAttitude : public std::unary_function< FireEffectInfos* , bool >
        {
        public:
            CmpAttitude(E_PopulationAttitude val) : val_(val) {}
            ~CmpAttitude(){}

            bool operator()( FireEffectInfos* tgtnfos ) const 
            {   return tgtnfos->nAttitude_ == val_;}

        private:
            E_PopulationAttitude val_;
        };
    };

    typedef ADN_Type_Vector_ABC<FireEffectInfos>  T_FireEffectInfosVector;
    typedef T_FireEffectInfosVector::iterator     IT_FireEffectInfosVector;

// *****************************************************************************
    class FireEffectRoeInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( FireEffectRoeInfos )

    public:
         FireEffectRoeInfos( E_PopulationRoe nRoe );
        ~FireEffectRoeInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_Type_String strName_;
        E_PopulationRoe nRoe_;
        ADN_Type_Double rAttritionSurface_;
        ADN_Type_Double rPH_;
    };

    typedef ADN_Type_Vector_ABC<FireEffectRoeInfos>  T_FireEffectRoeInfosVector;
    typedef T_FireEffectRoeInfosVector::iterator     IT_FireEffectRoeInfosVector;

// *****************************************************************************
    class SpeedEffectVolumeInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( SpeedEffectVolumeInfos )

    public:
         SpeedEffectVolumeInfos( ADN_Categories_Data::SizeInfos* ptr );
        ~SpeedEffectVolumeInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_Type_String                                          strName_;
        ADN_TypePtr_InVector_ABC<ADN_Categories_Data::SizeInfos> ptrVolume_;
        ADN_Type_Double                                          rDensity_;
        ADN_Type_Double                                          rMaxSpeed_;

    public:
        typedef ADN_Categories_Data::SizeInfos T_Item;

        class CmpRef : public std::unary_function< SpeedEffectVolumeInfos* , bool >
        {
        public:
            CmpRef(ADN_Categories_Data::SizeInfos* val) : val_(val) {}
            ~CmpRef(){}

            bool operator()( SpeedEffectVolumeInfos* tgtnfos ) const 
            {   return tgtnfos->ptrVolume_.GetData() == val_;}

        private:
            ADN_Categories_Data::SizeInfos* val_;
        };
    };

    typedef ADN_Type_VectorFixed_ABC<SpeedEffectVolumeInfos>  T_SpeedEffectVolumeInfosVector;
    typedef T_SpeedEffectVolumeInfosVector::iterator          IT_SpeedEffectVolumeInfosVector;

// *****************************************************************************
    class SpeedEffectInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( SpeedEffectInfos )

    public:
        SpeedEffectInfos( E_PopulationAttitude nAttitude );
        ~SpeedEffectInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( xml::xistream& input );
        void ReadSpeedEffect( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        E_PopulationAttitude           nAttitude_;
        ADN_Type_String                strName_;
        T_SpeedEffectVolumeInfosVector vVolumeInfos_;
    };

    typedef ADN_Type_Vector_ABC<SpeedEffectInfos>  T_SpeedEffectInfosVector;
    typedef T_SpeedEffectInfosVector::iterator     IT_SpeedEffectInfosVector;

// *****************************************************************************
    class PopulationInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( PopulationInfos )

    public:
        PopulationInfos();
        ~PopulationInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        PopulationInfos* CreateCopy();

        void ReadArchive( xml::xistream& input );
        void ReadSlowingEffect( xml::xistream& input );
        void ReadAttritionEffect( xml::xistream& input );
        void ReadFireEffect( xml::xistream& input );
        void WriteArchive( xml::xostream& output, int nMosId );

    public:
        ADN_Type_String                                       strName_;
        ADN_TypePtr_InVector_ABC<ADN_Models_Data::ModelInfos> ptrModel_;
        ADN_Type_Double                                       rConcentrationDensity_;
        ADN_Type_Double                                       rMoveDensity_;
        ADN_Type_Double                                       rMoveSpeed_;

        T_SpeedEffectInfosVector                              vSpeedEffectInfos_;
        T_FireEffectInfosVector                               vFireEffectInfos_;
        T_FireEffectRoeInfosVector                            vFireEffectRoeInfos_;
    };

    typedef ADN_Type_Vector_ABC<PopulationInfos>  T_PopulationInfosVector;
    typedef T_PopulationInfosVector::iterator      IT_PopulationInfosVector;

// *****************************************************************************
    class ReloadingSpeedEffectInfos
        : public ADN_Ref_ABC
    {
        MT_COPYNOTALLOWED( ReloadingSpeedEffectInfos )

    public:
        ReloadingSpeedEffectInfos();
        ~ReloadingSpeedEffectInfos();

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_Type_Double rDensity_;
        ADN_Type_Double rModifier_;
    };

// *****************************************************************************
public:
    ADN_Population_Data();
    virtual ~ADN_Population_Data();

    void                        FilesNeeded(T_StringList& l) const;
    void                        Reset();

    T_PopulationInfosVector&  GetPopulation();
    PopulationInfos*          FindPopulation( const std::string& strName );
    std::string GetPopulationsThatUse( ADN_Models_Data::ModelInfos& model );

private:
    void ReadArchive( xml::xistream& input );
    void ReadPopulation( xml::xistream& input );
    void WriteArchive( xml::xostream& output );

public:
    T_PopulationInfosVector   vPopulation_;
    ReloadingSpeedEffectInfos reloadingSpeedEffectInfos_;
};


// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::GetPopulation
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
inline
ADN_Population_Data::T_PopulationInfosVector& ADN_Population_Data::GetPopulation()
{
    return vPopulation_;
}


// -----------------------------------------------------------------------------
// Name: ADN_Population_Data::FindPopulation
// Created: APE 2004-12-02
// -----------------------------------------------------------------------------
inline
ADN_Population_Data::PopulationInfos* ADN_Population_Data::FindPopulation( const std::string& strName )
{
    IT_PopulationInfosVector it = std::find_if( vPopulation_.begin(), vPopulation_.end(), ADN_Tools::NameCmp<PopulationInfos>( strName ) );
    if( it == vPopulation_.end() )
        return 0;
    return *it;
}


#endif // __ADN_Population_Data_h_
