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
#include "ADN_DataTreeNode_ABC.h"
#include "ADN_Ref_ABC.h"
#include "ADN_Tools.h"
#include "ADN_Types.h"
#include "clients_kernel/Types.h"
#include "svgl/TextRenderer.h"

#define SYMBOL_PIXMAP_SIZE           64
#define SYMBOL_ICON_SIZE             128
#define SYMBOL_ICON_MARGIN           24
#define SYMBOL_BG_MARGIN             2
#define SYMBOL_SCALE_RATIO_FOR_METER 0.1f

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
    class SymbolsInfra : public ADN_Ref_ABC
                       , public ADN_DataTreeNode_ABC
    {
    public:
        explicit SymbolsInfra( xml::xistream& input );
        virtual ~SymbolsInfra();

        virtual std::string GetNodeName();
        std::string GetItemName();
        bool operator==( const std::string& str );

    public:
        //! @name Member Data
        //@{
        ADN_Type_String strName_;
        //@}
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
    virtual void FilesNeeded( T_StringList& files ) const;
    virtual void Load( const tools::Loader_ABC& fileLoader );
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
