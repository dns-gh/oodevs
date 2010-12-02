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

// =============================================================================
/** @class  ADN_LocalFireClass_Data
    @brief  ADN_LocalFireClass_Data
    @see    4311340-Modèles de feu.doc
*/
// Created: JSR 2010-12-01
// =============================================================================
class ADN_LocalFireClass_Data : public ADN_Data_ABC
{
    MT_COPYNOTALLOWED( ADN_LocalFireClass_Data )

public:
    class LocalFireClassInjuryInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( LocalFireClassInjuryInfos )

    public:
        explicit LocalFireClassInjuryInfos( const std::string& nodeName );
        virtual ~LocalFireClassInjuryInfos() {}

        virtual std::string GetNodeName();
        std::string GetItemName();

        void CopyFrom( LocalFireClassInjuryInfos& infos );

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

    class LocalFireClassInfos
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( ADN_LocalFireClass_Data::LocalFireClassInfos )

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
        LocalFireClassInjuryInfos injuryInfos_;
        helpers::T_UrbanAttritionInfos_Vector modifUrbanBlocks_;
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
    T_LocalFireClassInfosVector localFireClasses_;
    //@}
};

#endif // __ADN_LocalFireClass_Data_h_
