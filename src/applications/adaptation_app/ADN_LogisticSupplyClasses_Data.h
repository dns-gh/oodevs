// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_LogisticSupplyClasses_Data_h_
#define __ADN_LogisticSupplyClasses_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_RefWithName.h"
#include "tools/IdManager.h"

// =============================================================================
/** @class  ADN_LogisticSupplyClasses_Data
    @brief  ADN_LogisticSupplyClasses_Data
*/
// Created: ABR 2013-07-10
// =============================================================================
class ADN_LogisticSupplyClasses_Data : public ADN_Data_ABC
{
public:
    //! @name LogisticSupplyClass
    //@{
    class LogisticSupplyClass : public ADN_RefWithName
    {
    public:
        LogisticSupplyClass();
        LogisticSupplyClass( const std::string& name, int id );
        LogisticSupplyClass* CreateCopy();
        std::string GetItemName() { return strName_.GetData(); }

    public:
        ADN_Type_Int nId_;
    };
    typedef ADN_Type_Vector_ABC< LogisticSupplyClass > T_LogisticSupplyClass_Vector;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             ADN_LogisticSupplyClasses_Data();
    virtual ~ADN_LogisticSupplyClasses_Data();
    //@}

    //! @name Operations
    //@{
    void FilesNeeded( tools::Path::T_Paths& vFiles ) const;
    void Reset();

    T_LogisticSupplyClass_Vector& GetLogisticSupplyClasses();
    LogisticSupplyClass* FindLogisticSupplyClass( const std::string& strName );
    //@}

private:
    //! @name Serialization
    //@{
    void ReadArchive( xml::xistream& input );
    void ReadLogisticSupplyClass( xml::xistream& input );
    void WriteArchive( xml::xostream& output );
    //@}

private:
    //! @name Member data
    //@{
    ADN_LogisticSupplyClasses_Data::T_LogisticSupplyClass_Vector vLogisticSupplyClasses_;
    static tools::IdManager idManager_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ADN_LogisticSupplyClasses_Data::GetLogisticSupplyClasses
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
inline
ADN_LogisticSupplyClasses_Data::T_LogisticSupplyClass_Vector& ADN_LogisticSupplyClasses_Data::GetLogisticSupplyClasses()
{
    return vLogisticSupplyClasses_;
}

// -----------------------------------------------------------------------------
// Name: ADN_LogisticSupplyClasses_Data::FindLogisticSupplyClass
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
inline
ADN_LogisticSupplyClasses_Data::LogisticSupplyClass* ADN_LogisticSupplyClasses_Data::FindLogisticSupplyClass( const std::string& strName )
{
    for( auto it = vLogisticSupplyClasses_.begin(); it != vLogisticSupplyClasses_.end(); ++it )
        if( ADN_Tools::CaselessCompare( ( *it )->strName_.GetData(), strName ) )
            return *it;
    return 0;
}

#endif // __ADN_LogisticSupplyClasses_Data_h_
