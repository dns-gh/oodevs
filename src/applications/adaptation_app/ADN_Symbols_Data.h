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
}

namespace gui
{
    class SvglRenderer;
    class GLSymbols;
}

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
    class SymbolsUnit : public ADN_RefWithName
    {
    public:
                 SymbolsUnit( const std::string& nature, kernel::SymbolFactory& factory, gui::GLSymbols& glSymbols );
        virtual ~SymbolsUnit();

        const QPixmap& GetSamplePixmap() const;
        const QPixmap& GetMovePixmap() const;
        const QPixmap& GetStaticPixmap() const;

        const std::string& GetSymbol() const;
        const std::string& GetMoveSymbol() const;
        const std::string& GetStaticSymbol() const;

    private:
        QPixmap GenerateSymbol( std::string& symbol, gui::GLSymbols& glSymbols ) const;

    private:
        QPixmap sample_;
        QPixmap moveSample_;
        QPixmap staticSample_;

        std::string symbol_;
        std::string moveSymbol_;
        std::string staticSymbol_;

        static QPixmap* undefined_;
    };
    typedef ADN_Type_Vector_ABC< SymbolsUnit > T_SymbolsUnits_Vector;

//*****************************************************************************
public:
    class SymbolsInfra : public ADN_RefWithName
    {
    public:
        explicit SymbolsInfra( xml::xistream& input );
        virtual ~SymbolsInfra();
    };
    typedef ADN_Type_Vector_ABC< SymbolsInfra > T_SymbolsInfra_Vector;

//*****************************************************************************
public:
    //! @name Constructors/Destructor
    //@{
             ADN_Symbols_Data();
    virtual ~ADN_Symbols_Data();
    //@}

    //! @name Accessors
    //@{
    virtual void FilesNeeded( tools::Path::T_Paths& files ) const;
    void ReadAndCopyArchive( xml::xistream& xis );

    const kernel::SymbolFactory& GetSymbolFactory() const;
    const T_SymbolsUnits_Vector& GetSymbolsUnits() const;
    const T_SymbolsInfra_Vector& GetSymbolsInfras() const;
    SymbolsInfra* FindSymbolInfra( const std::string& strName ) const;
    SymbolsUnit* FindSymbolUnit( const std::string& strNature ) const;
    //@}

private:
    //! @name Operations
    //@{
    virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& ) const;
    //@}

    //! @name Helpers
    //@{
    void ReadArchive( xml::xistream& xis );
    void ReadInfra( xml::xistream& xis );
    void ReadRule( xml::xistream& xis );
    void WriteArchive( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    T_SymbolsUnits_Vector                  units_;
    T_SymbolsInfra_Vector                  infras_;
    std::unique_ptr< kernel::SymbolFactory > factory_;
    std::unique_ptr< gui::SvglRenderer >     svgRender_;
    std::unique_ptr< gui::GLSymbols >        glSymbols_;
    std::unique_ptr< xml::xibufferstream >   xibs_;
    //@}
};

#endif // __ADN_Symbols_Data_h_
