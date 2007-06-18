//*****************************************************************************
//
// $Created: JDY 03-06-25 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Objects_Data.h $
// $Author: Ape $
// $Modtime: 15/04/05 18:35 $
// $Revision: 9 $
// $Workfile: ADN_Objects_Data.h $
//
//*****************************************************************************

#ifndef __ADN_Objects_Data_h_
#define __ADN_Objects_Data_h_

#include "ADN_Data_ABC.h"

#include "ADN_Types.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Equipement_Data.h"
#include "ADN_DataTreeNode_ABC.h"

class ADN_XmlInput_Helper;


//*****************************************************************************
// Created: JDY 03-06-25
//*****************************************************************************
class ADN_Objects_Data : public ADN_Data_ABC
{    
    MT_COPYNOTALLOWED( ADN_Objects_Data )

public:
//*****************************************************************************
    class ScoreLocationInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( ScoreLocationInfos )

    public:
        ScoreLocationInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );

    public:
        ADN_Type_Enum< E_Location, eNbrLocation > nLocation_;
        ADN_Type_Int nScore_;

    public:
        class Cmp : public std::unary_function< ScoreLocationInfos* , bool >
        {
        public:
            Cmp(const E_Location& val) : val_(val) {}
            virtual ~Cmp() {}

            bool operator()( ScoreLocationInfos* tgtnfos ) const
            {   return tgtnfos->nLocation_.GetData()==val_;}
        
        private:
            E_Location val_;
        };
    };

    typedef ADN_Type_Vector_ABC< ScoreLocationInfos > T_ScoreLocationInfosVector;
    typedef T_ScoreLocationInfosVector::iterator      IT_ScoreLocationInfosVector;


//*****************************************************************************
    typedef ADN_Equipement_Data::AttritionInfos AttritionInfos; 
    typedef ADN_Type_VectorFixed_ABC<AttritionInfos> T_AttritionInfosVector;
    typedef T_AttritionInfosVector::iterator        IT_AttritionInfosVector;


//*****************************************************************************
    class PopulationAttritionInfos
    {
    public:
        PopulationAttritionInfos();
        ~PopulationAttritionInfos();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );

    public:
        ADN_Type_Double rSurface_;
        ADN_Type_Double rPh_;
    };

//*****************************************************************************
    class ObjectInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( ObjectInfos )

    public:
        ObjectInfos( E_ObjectType nType );
        ~ObjectInfos();

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );

    public:
        ADN_Type_Enum< E_ObjectType, eNbrObjectType > nObjectType_;
        ADN_Type_Enum<E_ConsumptionType,eNbrConsumptionType> nDefaultConsumption_;
        ADN_Type_String                              strName_;           // do not use directly !!!
        ADN_Type_Bool                                bDangerous_;
        ADN_Type_Bool                                bCanBeValorized_;
        ADN_Type_Bool                                bCanBeReservedObstacle_;
        ADN_Type_Bool                                bCanBeBypassed_;
        ADN_Type_Double                              rAvoidDistance_;
        ADN_Type_Double                              rDefaultSpeed_;
        ADN_Type_Double                              rDefaultBypassSpeed_;
        ADN_Type_Double                              rMaxInteractionHeight_;

        ADN_Type_Int                                 nMaxNbrUsers_;

        ADN_Type_Enum< E_SpeedImpact, eNbrSpeedImpact > nSpeedImpact_;
        ADN_Type_Double                              rMaxAgentSpeedPercentage_;

        ADN_Type_Bool                                bHasOutgoingPopulationDensity_;
        ADN_Type_Double                              rOutgoingPopulationDensity_;

        T_ScoreLocationInfosVector                   vScoreLocation_;

        ADN_Type_Bool                                bAttritions_;
        T_AttritionInfosVector                       attritions_;

        ADN_Type_Int                                                  nNbrToBuild_;
        ADN_Type_Bool                                                 bToBuild_;
        ADN_TypePtr_InVector_ABC<ADN_Equipement_Data::CategoryInfo>   ptrToBuild_;
        ADN_Type_Int                                                  nNbrToReinforce_;
        ADN_Type_Bool                                                 bToReinforce_;
        ADN_TypePtr_InVector_ABC<ADN_Equipement_Data::CategoryInfo>   ptrToReinforce_;

        PopulationAttritionInfos                     populationAttrition_;
        ADN_Type_Bool                                bPopulationAttrition_;

    public:
        class Cmp : public std::unary_function< ObjectInfos* , bool >
        {
        public:
            Cmp(const E_ObjectType& val) : val_(val) {}
            virtual ~Cmp() {}

            bool operator()( ObjectInfos* tgtnfos ) const
            {   return tgtnfos->nObjectType_.GetData()==val_;}

        private:
            E_ObjectType val_;
        };
    };
    
    typedef ADN_Type_Vector_ABC<ObjectInfos> T_ObjectsInfos_Vector;
    typedef T_ObjectsInfos_Vector::iterator  IT_ObjectsInfos_Vector;


//*****************************************************************************
    class AreaControlInformations
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( AreaControlInformations )

    public:
         AreaControlInformations();
        ~AreaControlInformations();

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );

    public:
        ADN_TypePtr_InVector_ABC<ADN_Categories_Data::SizeInfos> ptrSize_;
        ADN_Type_Int                                             nPercentage_;
    };

    typedef ADN_Type_Vector_ABC<AreaControlInformations> T_AreaControlInformations_Vector;
    typedef T_AreaControlInformations_Vector::iterator  IT_AreaControlInformations_Vector;

//*****************************************************************************
public:    
    ADN_Objects_Data();
    virtual ~ADN_Objects_Data();

    void            FilesNeeded(T_StringList& l) const;
    void            Reset();
    void            SaveAttritionInfos( MT_OutputArchive_ABC& output );


    T_ObjectsInfos_Vector& GetObjectInfos();
    ObjectInfos*           FindObject( const std::string& strName );

private:
    void ReadArchive( ADN_XmlInput_Helper& input );
    void WriteArchive( MT_OutputArchive_ABC& output );

private:
    T_ObjectsInfos_Vector     vObjectInfos_;
    T_AreaControlInformations_Vector vAreaControlInformations_;
};


// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::GetObjectInfos
// Created: APE 2004-11-24
// -----------------------------------------------------------------------------
inline
ADN_Objects_Data::T_ObjectsInfos_Vector& ADN_Objects_Data::GetObjectInfos()
{
    return vObjectInfos_;
}


// -----------------------------------------------------------------------------
// Name: ADN_Objects_Data::FindObject
// Created: APE 2004-11-30
// -----------------------------------------------------------------------------
inline
ADN_Objects_Data::ObjectInfos* ADN_Objects_Data::FindObject( const std::string& strName )
{
    IT_ObjectsInfos_Vector it = std::find_if( vObjectInfos_.begin(), vObjectInfos_.end(), ADN_Tools::NameCmp<ADN_Objects_Data::ObjectInfos>(strName) );
    if( it == vObjectInfos_.end() )
        return 0;
    return *it;
}


#endif // __ADN_Objects_Data_h_
