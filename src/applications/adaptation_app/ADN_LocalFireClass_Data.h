// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_LocalFireClass_Data_h_
#define __ADN_LocalFireClass_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Types.h"
#include "ADN_Type_VectorFixed_ABC.h"
#include "ADN_ExtinguisherAgentInfos.h"
#include "ADN_UrbanAttritionInfos.h"
#include "ADN_WeatherFireEffects.h"

// =============================================================================
/** @class  ADN_LocalFireClass_Data
    @brief  ADN_LocalFireClass_Data
    @see    4311340-Mod�les de feu.doc
*/
// Created: JSR 2010-12-01
// =============================================================================
class ADN_LocalFireClass_Data : public ADN_Data_ABC
{

public:
    class FireInjuryInfos : public ADN_Ref_ABC
                          , public ADN_DataTreeNode_ABC
    {

    public:
        explicit FireInjuryInfos( const std::string& nodeName );
        virtual ~FireInjuryInfos() {}

        virtual std::string GetNodeName();
        std::string GetItemName();

        void CopyFrom( FireInjuryInfos& infos );

        void ReadInjury( xml::xistream& input );
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        std::string nodeName_;
        ADN_Type_Int nNbHurtHumans1_;
        ADN_Type_Int nNbHurtHumans2_;
        ADN_Type_Int nNbHurtHumans3_;
        ADN_Type_Int nNbHurtHumansE_;
        ADN_Type_Int nNbDeadHumans_;
    };

    class FireSurfaceInfos : public ADN_Ref_ABC
                           , public ADN_DataTreeNode_ABC
    {
    public:
        explicit FireSurfaceInfos( E_Location location );
        virtual ~FireSurfaceInfos() {}
        std::string GetItemName();

        void CopyFrom( FireSurfaceInfos& infos );

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_Type_String strName_;
        ADN_Type_Int ignitionThreshold_;
        ADN_Type_Int maxCombustionEnergy_;

    public:
        class Cmp : public std::unary_function< FireSurfaceInfos*, bool >
        {
        public:
             Cmp( const std::string& strName ) : strName_( strName ) {}
            ~Cmp() {}

            bool operator()( FireSurfaceInfos* infos ) const
            { 
                return infos->strName_ == strName_;
            }
        private:
            std::string strName_;
        };
    };

    typedef ADN_Type_Vector_ABC< FireSurfaceInfos > T_FireSurfaceInfos_Vector;
    typedef T_FireSurfaceInfos_Vector::iterator    IT_FireSurfaceInfos_Vector;

    class LocalFireClassInfos : public ADN_Ref_ABC
                              , public ADN_DataTreeNode_ABC
    {
    public:
        //! @name Constructors/Destructor
        //@{
                 LocalFireClassInfos();
        virtual ~LocalFireClassInfos();
        //@}

        //! @name Operations
        //@{
        virtual std::string GetNodeName();
        std::string GetItemName();
        LocalFireClassInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );
        //@}

    private:
        //! @name Helpers
        //@{
        void ReadAgent( xml::xistream& input );
        void ReadWeatherEffect( xml::xistream& input );
        void ReadUrbanModifer( xml::xistream& input );
        void ReadSurface( xml::xistream& input );
        //@}

    public:
        //! @name Member data
        //@{
        ADN_Type_String strName_;
        ADN_Type_Int initialHeat_;
        ADN_Type_Int maxHeat_;
        ADN_Type_Int increaseRate_;
        ADN_Type_Int decreaseRate_;
        T_ExtinguisherAgentInfos_Vector agents_;
        FireInjuryInfos injuryInfos_;
        helpers::T_UrbanAttritionInfos_Vector modifUrbanBlocks_;
        T_WeatherFireEffects_Vector weatherEffects_;
        ADN_Type_Bool isSurface_;
        T_FireSurfaceInfos_Vector surfaceInfos_;
        //@}
    };

    //! @name Types
    //@{
    typedef ADN_Type_Vector_ABC< LocalFireClassInfos > T_LocalFireClassInfosVector;
    typedef T_LocalFireClassInfosVector::iterator     IT_LocalFireClassInfosVector;
    //@}

    //! @name Constructors/Destructor
    //@{
             ADN_LocalFireClass_Data();
    virtual ~ADN_LocalFireClass_Data();
    //@}

    //! @name Operations
    //@{
    void FilesNeeded( T_StringList& l ) const;
    void Reset();
    ADN_Type_Int& GetCellSize();
    T_LocalFireClassInfosVector& GetLocalFireClassesInfos();
    //@}

private:
    //! @name Helpers
    //@{
    void ReadArchive( xml::xistream& input );
    void ReadLocalFireClass( xml::xistream& input );
    void WriteArchive( xml::xostream& output );
    //@}

private:
    //! @name Member data
    //@{
    ADN_Type_Int cellSize_;
    T_LocalFireClassInfosVector localFireClasses_;
    //@}
};

#endif // __ADN_LocalFireClass_Data_h_
