// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ADN_UnitSymbols_Data_h_
#define __ADN_UnitSymbols_Data_h_

#include "ADN_Data_ABC.h"
#include "ADN_RefWithName.h"
#include "ADN_Ref_ABC.h"
#include "ADN_Types.h"
#include "clients_kernel/Types.h"
#include "svgl/TextRenderer.h"

namespace svg {
    class Node_ABC;
    class References;
    class Property_ABC;
    class RenderingContext;
}

namespace kernel
{
    class SymbolFactory;
}

namespace xml
{
    class xistream;
}
namespace gui
{
    class DrawingTemplate;
    class SvglRenderer;
    class GLSymbols;
}
namespace
{
    class GlToolsSymbols;
}

namespace zip
{
    class izipfile;
}

// =============================================================================
/** @class  ADN_UnitSymbols_Data
@brief  ADN_UnitSymbols_Data
*/
// Created: MMC 2011-0-07
// =============================================================================
class ADN_UnitSymbols_Data : public ADN_Data_ABC
{
public:
    class UnitSymbolInfo : public ADN_RefWithName
    {
    public:
        //! @name Constructors/Destructor
        //@{
        UnitSymbolInfo( const std::string& name, const std::string& symbol, gui::GLSymbols& symbols );
        virtual ~UnitSymbolInfo();
        //@}

        //! @name Accessors
        //@{
        const QPixmap& GetSamplePixmap() const;
        const std::string GetCode() const;
        const std::string GetGeometry() const;
        //@}

    private:
        //! @name Helpers
        //@{
        void Initialize();
        void Draw();
        //@}

    private:
        //! @name Private member data
        //@{
        gui::DrawingTemplate* template_;
        QPixmap* pixmap_;
        gui::GLSymbols& symbols_;
        //@}

    public:
        //! @name Public member data
        //@{
        std::string fileName_;
        //@}
    };

public:
    //! @name Constructors/Destructor
    //@{
    ADN_UnitSymbols_Data();
    virtual ~ADN_UnitSymbols_Data();
    //@}

    //! @name Types
    //@{
    typedef ADN_Type_Vector_ABC< UnitSymbolInfo >            T_UnitSymbolInfoVector;
    typedef T_UnitSymbolInfoVector::iterator                IT_UnitSymbolInfoVector;
    typedef T_UnitSymbolInfoVector::const_iterator         CIT_UnitSymbolInfoVector;
    //@}

    //! @name Operations
    //@{
    virtual void Reset();
    UnitSymbolInfo* const GetSymbol() const;
    gui::GLSymbols& GetGlSymbols() const;
    T_UnitSymbolInfoVector& GetSymbols();
    virtual void FilesNeeded( tools::Path::T_Paths& vFiles ) const;
    virtual void Load( const tools::Loader_ABC& fileLoader );
    virtual void Save();
    virtual void CheckDatabaseValidity( ADN_ConsistencyChecker& ) const;
    //@}

protected:

    virtual void ReadArchive( xml::xistream& input );
    virtual void WriteArchive( xml::xostream& output );

private:
    //! @name Member data
    //@{
    T_UnitSymbolInfoVector symbols_;
    kernel::SymbolFactory* pSymbolFactory_;
    std::auto_ptr< gui::SvglRenderer > guiSvgRender_;
    std::auto_ptr< gui::GLSymbols > glSymbols_;
    //@}
};

#endif // __ADN_UnitSymbols_Data_h_
