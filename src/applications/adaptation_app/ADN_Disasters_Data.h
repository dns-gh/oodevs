// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Disasters_Data_h_
#define __ADN_Disasters_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_Tools.h"
#include "ADN_RefWithName.h"

// =============================================================================
/** @class  ADN_Disasters_Data
    @brief  ADN_Disasters_Data
*/
// Created: LGY 2012-11-13
// =============================================================================
class ADN_Disasters_Data : public ADN_Data_ABC
{
public:
    //*****************************************************************************
    class NbcSuitRatioInfos : public ADN_Ref_ABC
    {
    public:
        //! @name Constructors/Destructor
        //@{
        explicit NbcSuitRatioInfos( const E_AgentNbcSuit& eType );
        virtual ~NbcSuitRatioInfos();
        //@}

    public:
        //! @name Operations
        //@{
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;
        //@}

    public:
        //! @name Member Data
        //@{
        E_AgentNbcSuit eType_;
        ADN_Type_Double rCoeff_;
        //@}
    };
    typedef ADN_Type_Vector_ABC< NbcSuitRatioInfos > T_NbcSuitRatioInfosVector;

public:
//*****************************************************************************
    class AttritionThresholdInfos : public ADN_Ref_ABC
    {
    public:
        //! @name Constructors/Destructor
        //@{
                 AttritionThresholdInfos();
        virtual ~AttritionThresholdInfos();
        //@}

    public:
        //! @name Operations
        //@{
        AttritionThresholdInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;
        //@}

    public:
        //! @name Member Data
        //@{
        ADN_Type_String name_;
        ADN_Type_Double threshold_;
        ADN_Type_Int u1_;
        ADN_Type_Int u2_;
        ADN_Type_Int u3_;
        ADN_Type_Int ue_;
        ADN_Type_Int dead_;
        ADN_Type_Bool contamination_;
        //@}
    };
    typedef ADN_Type_Vector_ABC< AttritionThresholdInfos > T_AttritionThresholdInfosVector;

public:
//*****************************************************************************
    class ConcentrationThresholdInfos : public ADN_Ref_ABC
    {
    public:
        //! @name Constructors/Destructor
        //@{
                 ConcentrationThresholdInfos();
        virtual ~ConcentrationThresholdInfos();
        //@}

        //! @name Operations
        //@{
        ConcentrationThresholdInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;
        //@}

    public:
        //! @name Member Data
        //@{
        ADN_Type_String color_;
        ADN_Type_String name_;
        ADN_Type_Double threshold_;
        ADN_Type_Bool display_;
        //@}
    };
    typedef ADN_Type_Vector_ABC< ConcentrationThresholdInfos > T_ConcentrationThresholdInfosVector;

public:
//*****************************************************************************
    class DisasterInfos : public ADN_RefWithName
    {
    public:
        //! @name Constructors/Destructor
        //@{
                 DisasterInfos();
        virtual ~DisasterInfos();
        //@}

    public:
        //! @name Operations
        //@{
        DisasterInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void ReadContamination( xml::xistream& input );
        void ReadAttrition( xml::xistream& input );
        void ReadProtection( xml::xistream& input );
        void WriteArchive( xml::xostream& output ) const;
        //@}

    public:
        //! @name Member Data
        //@{
        T_ConcentrationThresholdInfosVector concentrationThresholds_;
        T_AttritionThresholdInfosVector attritionThresholds_;
        T_NbcSuitRatioInfosVector nbcSuitRatio_;
        ADN_Type_Double toxicityExponent_;
        //@}
    };
public:
//*****************************************************************************
    //! @name Constructors/Destructor
    //@{
             ADN_Disasters_Data();
    virtual ~ADN_Disasters_Data();
    //@}

public:
    //! @name Types
    //@{
    typedef ADN_Type_Vector_ABC< DisasterInfos > T_DisasterInfos_Vector;
    //@}

public:
    //! @name Operations
    //@{
    virtual bool IsActivated() const;
    virtual void FilesNeeded(tools::Path::T_Paths& l) const;
    //@}

public:
    //! @name Accessors
    //@{
    T_DisasterInfos_Vector& GetDisastersInfos();
    DisasterInfos* FindDisaster( const std::string& strName );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadDisaster( xml::xistream& input );
    void ReadArchive( xml::xistream& input );
    void WriteArchive( xml::xostream& output ) const;
    //@}

private:
    //! @name Member Data
    //@{
    T_DisasterInfos_Vector vDisasters_;
    bool activated_;
    //@}
};

//-----------------------------------------------------------------------------
// Name: ADN_Disasters_Data::GetDisastersInfos
// Created: LGY 2012-11-13
//-----------------------------------------------------------------------------
inline
ADN_Disasters_Data::T_DisasterInfos_Vector& ADN_Disasters_Data::GetDisastersInfos()
{
    return vDisasters_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Disasters_Data::FindLauncher
// Created: LGY 2012-11-13
// -----------------------------------------------------------------------------
inline
ADN_Disasters_Data::DisasterInfos* ADN_Disasters_Data::FindDisaster( const std::string& strName )
{
    auto it = std::find_if( vDisasters_.begin(), vDisasters_.end(), ADN_Tools::NameCmp( strName ) );
    if( it == vDisasters_.end() )
        return 0;
    return *it;
}

#endif // __ADN_Disasters_Data_h_