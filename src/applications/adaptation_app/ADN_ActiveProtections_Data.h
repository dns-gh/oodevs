// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ActiveProtections_Data_h_
#define __ADN_ActiveProtections_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_DataTreeNode_ABC.h"
#include "ADN_Types.h"
#include "ADN_Type_Vector_ABC.h"
#include "ADN_Type_VectorFixed_ABC.h"


// =============================================================================
/** @class  ADN_ActiveProtections_Data
    @brief  ADN_ActiveProtections_Data
*/
// Created: LDC 2010-01-13
// =============================================================================
class ADN_ActiveProtections_Data : public ADN_Data_ABC
{
    MT_COPYNOTALLOWED( ADN_ActiveProtections_Data );

public:

    class ActiveProtectionsInfosWeapons
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( ActiveProtectionsInfosWeapons );

    public:
        ActiveProtectionsInfosWeapons();
        virtual ~ActiveProtectionsInfosWeapons() {}
        std::string GetItemName();  

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& xos );
    
    public:
        ADN_Type_String strName_; // To use with ADN_Tools::NameCmp
        ADN_Type_Double coefficient_;
    };

    typedef ADN_Type_Vector_ABC<ActiveProtectionsInfosWeapons>  T_ActiveProtectionsInfosWeaponsVector;
    typedef T_ActiveProtectionsInfosWeaponsVector::iterator     IT_ActiveProtectionsInfosWeaponsVector;


    //! @name Types
    //@{
    class ActiveProtectionsInfos 
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( ActiveProtectionsInfos );

    public:
        ActiveProtectionsInfos();
        virtual ~ActiveProtectionsInfos() {}        
        std::string GetItemName();        

        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& xos );
        void ReadWeapon( xml::xistream& xis );
    
    public:
        ADN_Type_String                       strName_; // To use with ADN_Tools::NameCmp
        ADN_Type_Double                       coefficient_;
        ADN_Type_Bool                         hardKill_;
        ADN_Type_String                       strDotationName_;
        ADN_Type_Double                       usage_;
        T_ActiveProtectionsInfosWeaponsVector weapons_;
    };

    typedef ADN_Type_Vector_ABC<ActiveProtectionsInfos>  T_ActiveProtectionsInfosVector;
    typedef T_ActiveProtectionsInfosVector::iterator     IT_ActiveProtectionsInfosVector;


    //! @name Constructors/Destructor
    //@{
             ADN_ActiveProtections_Data();
    virtual ~ADN_ActiveProtections_Data();
    //@}

    //! @name Helpers
    //@{
    ActiveProtectionsInfos* FindActiveProtection(const std::string& stdName );
    //@}

    //! @name Operations
    //@{
    virtual void FilesNeeded( T_StringList& vFiles ) const;
    virtual void Reset();
    T_ActiveProtectionsInfosVector& GetActiveProtectionsInfos();
    //@}

    //@}
    //! @name Member data
    //@{
    T_ActiveProtectionsInfosVector activeProtections_;
    //@}
private:

    //! @name Helpers
    //@{
    virtual void ReadArchive( xml::xistream& xis );
    virtual void WriteArchive( xml::xostream& xos );
    virtual void ReadProtection( xml::xistream& xis );
    //@}


};

#endif // __ADN_ActiveProtections_Data_h_
