// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Maintenance_Data_h_
#define __ADN_Maintenance_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Types.h"
#include "ADN_Enums.h"
#include "ADN_AvailabilityWarning.h"

class ADN_XmlInput_Helper;


// =============================================================================
/** @class  ADN_Maintenance_Data
    @brief  ADN_Maintenance_Data
*/
// Created: APE 2005-03-14
// =============================================================================
class ADN_Maintenance_Data
: public ADN_Data_ABC
{
    MT_COPYNOTALLOWED( ADN_Maintenance_Data )

public:
// *****************************************************************************
    class WorkingSchemeInfo
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
    {
        MT_COPYNOTALLOWED( WorkingSchemeInfo )
    public:
        WorkingSchemeInfo( unsigned int nIdx );

        virtual std::string GetNodeName();
        std::string GetItemName();

        void ReadArchive( ADN_XmlInput_Helper& input );
        void WriteArchive( MT_OutputArchive_ABC& output );

    public:
        ADN_Type_Int  nIdx_; 
        ADN_Type_Int  nWorkTime_;
        ADN_Type_Time warningDelay_;
    };

    typedef ADN_Type_Vector_ABC< WorkingSchemeInfo >  T_WorkingSchemeInfo_Vector;
    typedef T_WorkingSchemeInfo_Vector::iterator     IT_WorkingSchemeInfo_Vector;

// *****************************************************************************
public:
    //! @name Constructors/Destructor
    //@{
    ADN_Maintenance_Data();
    ~ADN_Maintenance_Data();
    //@}

    //! @name Operations
    //@{
    void FilesNeeded( T_StringList& vFiles ) const;
    void Reset();
    //@}

private:
    void ReadArchive( ADN_XmlInput_Helper& input );
    void WriteArchive( MT_OutputArchive_ABC& output );

public:
    T_WorkingSchemeInfo_Vector vWorkingSchemes_;
    T_AvailabilityWarning_Vector vRepairerWarnings_;
    T_AvailabilityWarning_Vector vHaulerWarnings_;
};

#endif // __ADN_Maintenance_Data_h_
