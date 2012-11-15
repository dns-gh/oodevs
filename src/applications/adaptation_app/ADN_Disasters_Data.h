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
#include "ADN_RefWithName.h"

namespace xml
{
    class xistream;
}

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
    class DisasterInfos : public ADN_RefWithName
    {
    public:
        //! @name Constructors/Destructor
        //@{
                 DisasterInfos();
        virtual ~DisasterInfos();
        //@}

        //! @name Operations
        //@{
        DisasterInfos* CreateCopy();
        void ReadArchive( xml::xistream& input );
        void WriteArchive( xml::xostream& output );
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
    typedef T_DisasterInfos_Vector::iterator    IT_DisasterInfos_Vector;
    //@}

public:
    //! @name Operations
    //@{
    virtual void FilesNeeded(T_StringList& l) const;
    virtual void Reset();
    //@}

public:
    //! @name Accessors
    //@{
    T_DisasterInfos_Vector& GetDisastersInfos();
    DisasterInfos*          FindDisaster( const std::string& strName );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadDisaster( xml::xistream& input );
    void ReadArchive( xml::xistream& input );
    void WriteArchive( xml::xostream& output );
    //@}

private:
    //! @name Member Data
    //@{
    T_DisasterInfos_Vector vDisasters_;
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
    IT_DisasterInfos_Vector it = std::find_if( vDisasters_.begin(), vDisasters_.end(), ADN_Tools::NameCmp< ADN_Disasters_Data::DisasterInfos >( strName ) );
    if( it == vDisasters_.end() )
        return 0;
    return *it;
}

#endif // __ADN_Disasters_Data_h_