// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Reports_Data_h_
#define __ADN_Reports_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Types.h"
#include "ADN_Enums.h"

class ADN_XmlInput_Helper;

// =============================================================================
/** @class  ADN_Reports_Data
    @brief  ADN_Reports_Data
*/
// Created: SBO 2006-12-14
// =============================================================================
class ADN_Reports_Data : public ADN_Data_ABC
{
// =============================================================================
// Report parameters
// =============================================================================
public:
    class ReportParameterValue : public ADN_Ref_ABC
                               , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( ReportParameterValue )
    public:
                 ReportParameterValue();
        virtual ~ReportParameterValue();

        std::string GetItemName();
        ReportParameterValue* CreateCopy();

        void ReadArchive ( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output, unsigned int id );

    public:
        ADN_Type_String name_;
    };

    typedef ADN_Type_Vector_ABC<ReportParameterValue>  T_ReportParameterValue_Vector;
    typedef T_ReportParameterValue_Vector::iterator   IT_ReportParameterValue_Vector;

public:
    class ReportParameter : public ADN_Ref_ABC
                          , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( ReportParameter )
    public:
                 ReportParameter();
        virtual ~ReportParameter();
        
        std::string GetItemName();
        ReportParameter* CreateCopy();

        void ReadArchive ( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );

    public:
        ADN_Type_String                                                   strName_;
        ADN_Type_Enum< E_MissionParameterType, eNbrMissionParameterType > type_;
        T_ReportParameterValue_Vector                                     values_;
    };

    typedef ADN_Type_Vector_ABC<ReportParameter>  T_ReportParameter_Vector;
    typedef T_ReportParameter_Vector::iterator   IT_ReportParameter_Vector;

// *****************************************************************************
// Report info
// *****************************************************************************

public:
    class ReportInfo : public ADN_Ref_ABC
                     , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( ReportInfo )
    public:
                 ReportInfo();
        virtual ~ReportInfo();

        std::string GetItemName();
        ReportInfo* CreateCopy();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output, unsigned long id );

    public:
        ADN_Type_String message_;
        T_ReportParameter_Vector parameters_;
    };

    typedef ADN_Type_Vector_ABC<ReportInfo>  T_ReportInfo_Vector;
    typedef T_ReportInfo_Vector::iterator   IT_ReportInfo_Vector;

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Reports_Data();
    virtual ~ADN_Reports_Data();
    //@}

    //! @name Operations
    //@{
    void FilesNeeded( T_StringList& l ) const;
    void Reset();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ADN_Reports_Data( const ADN_Reports_Data& );            //!< Copy constructor
    ADN_Reports_Data& operator=( const ADN_Reports_Data& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void ReadArchive( ADN_XmlInput_Helper& input );
    void WriteArchive( MT_OutputArchive_ABC& output );
    //@}

private:
    //! @name Member data
    //@{
    T_ReportInfo_Vector reports_;
    //@}
};

#endif // __ADN_Reports_Data_h_
