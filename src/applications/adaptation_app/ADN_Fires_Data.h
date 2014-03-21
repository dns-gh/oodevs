// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Fires_Data_h_
#define __ADN_Fires_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_RefWithName.h"
#include "ADN_Types.h"
#include "ADN_ExtinguisherAgentInfos.h"
#include "ADN_WeatherFireEffects.h"

// =============================================================================
/** @class  ADN_Fires_Data
    @brief  ADN_Fires_Data
    @see    4311340-Modèles de feu.doc
*/
// Created: JSR 2010-12-01
// =============================================================================
class ADN_Fires_Data : public ADN_Data_ABC
{

public:
    class FireInjuryInfos : public ADN_Ref_ABC
    {

    public:
        explicit FireInjuryInfos();
        virtual ~FireInjuryInfos() {}

        void CopyFrom( FireInjuryInfos& infos );

        void ReadInjury( xml::xistream& input );
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;
        void CheckDatabaseValidity( ADN_ConsistencyChecker& ) const;

    public:
        std::string parentName_;
        ADN_Type_Int nNbHurtHumans1_;
        ADN_Type_Int nNbHurtHumans2_;
        ADN_Type_Int nNbHurtHumans3_;
        ADN_Type_Int nNbHurtHumansE_;
        ADN_Type_Int nNbDeadHumans_;
    };

    class FireSurfaceInfos : public ADN_RefWithName
    {
    public:
        explicit FireSurfaceInfos( E_Location location );
        virtual ~FireSurfaceInfos() {}
        std::string GetItemName();

        void CopyFrom( FireSurfaceInfos& infos );

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;

    public:
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

    class FireClassInfos : public ADN_RefWithLocalizedName
    {
    public:
        //! @name Constructors/Destructor
        //@{
                 FireClassInfos();
        virtual ~FireClassInfos();
        //@}

        //! @name Operations
        //@{
        FireClassInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;
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
    typedef ADN_Type_Vector_ABC< FireClassInfos > T_FireClassInfosVector;
    //@}

    //! @name Constructors/Destructor
    //@{
             ADN_Fires_Data();
    virtual ~ADN_Fires_Data();
    //@}

    //! @name Operations
    //@{
    void FilesNeeded( tools::Path::T_Paths& l ) const;
    ADN_Type_Int& GetCellSize();
    T_FireClassInfosVector& GetFireClassesInfos();
    QStringList GetFireThatUse( ADN_Resources_Data::CategoryInfo& infos );
    virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadArchive( xml::xistream& input );
    void ReadFireClass( xml::xistream& input );
    void WriteArchive( xml::xostream& output ) const;
    //@}

private:
    //! @name Member data
    //@{
    ADN_Type_Int cellSize_;
    T_FireClassInfosVector fireClasses_;
    //@}
};

#endif // __ADN_Fires_Data_h_
