// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_AiEngine_Data_h_
#define __ADN_AiEngine_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Types.h"
#include "ADN_Urban_Data.h"

// =============================================================================
/** @class  ADN_AiEngine_Data
    @brief  ADN_AiEngine_Data
*/
// Created: AGN 2004-06-15
// =============================================================================
class ADN_AiEngine_Data : public ADN_Data_ABC
{
public:
    class UrbanSpeedsInfos : public ADN_Ref_ABC
    {
    public:
        explicit UrbanSpeedsInfos( ADN_Urban_Data::UrbanMaterialInfos* ptr );

        std::string GetItemName();
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );

    public:
        ADN_TypePtr_InVector_ABC< ADN_Urban_Data::UrbanMaterialInfos > ptrMaterial_;
        ADN_Type_Double searchSpeed_;
        ADN_Type_Double reconSpeed_;

    public:
        typedef ADN_Urban_Data::UrbanMaterialInfos T_Item;

        class Cmp : public std::unary_function< UrbanSpeedsInfos* , bool >
        {
        public:
            explicit Cmp( const std::string& val ) : val_( val ) {}
            virtual ~Cmp() {}

            bool operator()( UrbanSpeedsInfos* tgtnfos ) const
            {
                return tgtnfos->ptrMaterial_.GetData() && tgtnfos->ptrMaterial_.GetData()->strName_ == val_;
            }

        private:
            std::string val_;
        };

        class CmpRef : public std::unary_function< UrbanSpeedsInfos* , bool >
        {
        public:
            explicit CmpRef( ADN_Urban_Data::UrbanMaterialInfos* val ) : val_( val ) {}
            virtual ~CmpRef() {}

            bool operator()( UrbanSpeedsInfos* tgtnfos ) const
            {
                return tgtnfos->ptrMaterial_.GetData() == val_;
            }

        private:
            ADN_Urban_Data::UrbanMaterialInfos* val_;
        };
    };

    typedef ADN_Type_VectorFixed_ABC< UrbanSpeedsInfos > T_UrbanSpeedsInfos_Vector;

public:
    //! @name Constructors/Destructor
    //@{
             ADN_AiEngine_Data();
    virtual ~ADN_AiEngine_Data();
    //@}

    void FilesNeeded( tools::Path::T_Paths& l ) const;

private:
    void ReadArchive( xml::xistream& );
    void WriteArchive( xml::xostream& ) const;

public:
    ADN_Type_Double rPertinenceMaxDecrease_;
    ADN_Type_Double rOperationalStateMaxDecrease_;
    ADN_Type_Double rNeutralizedStateMaxDecrease_;
    ADN_Type_Double rMinorEquipmentWeight_;
    ADN_Type_Double rMajorEquipmentWeight_;
    ADN_Type_Double rHumanWeight_;
    ADN_Type_Double rDecisionalThreshold_;
    ADN_Type_Double rUrbanViewFactor_;
    ADN_Type_Time rDefaultFeedbackTime_;
    ADN_Type_Time rMinimumAffinity_;
    ADN_Type_Time rNeutralAffinity_;
    ADN_Type_Time rMaximumAffinity_;
    ADN_Type_Double rAvailableModificator_;
    ADN_Type_Double rUnavailableModificator_;
    ADN_Type_Double rRepairableModificator_;
    ADN_Type_Double rRepairingModificator_;
    ADN_Type_Double rCapturedModificator_;
    ADN_Type_Bool bDetectDestroyedUnits_;
    ADN_Type_Double reconSpeed_;
    ADN_Type_Double searchSpeed_;
    T_UrbanSpeedsInfos_Vector urbanSearchSpeeds_;
};

#endif // __ADN_AiEngine_Data_h_
