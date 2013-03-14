// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Symbols_Data_h_
#define __ADN_Symbols_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_RefWithName.h"
#include "ADN_Ref_ABC.h"
#include "ADN_Tools.h"
#include "ADN_Types.h"
#include "clients_kernel/Types.h"
#include "svgl/TextRenderer.h"

namespace xml
{
    class xistream;
}
namespace kernel
{
    class SymbolFactory;
    class SymbolRule;
};

// =============================================================================
/** @class  ADN_Symbols_Data
    @brief  ADN_Symbols_Data
*/
// Created: SBO 2011-04-18
// =============================================================================
class ADN_Symbols_Data : public ADN_Data_ABC
{

//*****************************************************************************
public:
    class SymbolsInfra : public ADN_RefWithName
    {
    public:
        explicit SymbolsInfra( xml::xistream& input );
        virtual ~SymbolsInfra();

        bool operator==( const std::string& str );
    };
    TYPEDEF_FULL_DECLARATION( ADN_Type_Vector_ABC< SymbolsInfra >, SymbolsInfra_Vector )

//*****************************************************************************
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Symbols_Data();
    virtual ~ADN_Symbols_Data();
    //@}

    //! @name Accessors
    //@{
    kernel::SymbolFactory& GetSymbolFactory();
    T_SymbolsInfra_Vector& GetSymbolsInfras();
    SymbolsInfra* FindSymbolInfra( const std::string& strName ) const;
    //@}

    //! @name Operations
    //@{
    virtual void FilesNeeded( tools::Path::T_Paths& files ) const;
    virtual void Reset();
    //@}

private:
    //! @name Helpers
    //@{
    void ReadArchive( xml::xistream& xis );
    void ReadInfra( xml::xistream& xis );
    void ReadRule( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    T_SymbolsInfra_Vector                  infras_;
    std::auto_ptr< kernel::SymbolFactory > factory_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ADN_Symbols_Data::SymbolsInfra::operator==
// Created: SLG 2010-12-20
// -----------------------------------------------------------------------------
inline
bool ADN_Symbols_Data::SymbolsInfra::operator==( const std::string& str )
{
    return ADN_Tools::CaselessCompare( strName_.GetData(), str );
}

#endif // __ADN_Symbols_Data_h_
