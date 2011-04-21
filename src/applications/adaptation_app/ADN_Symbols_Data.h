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
namespace gui
{
    class DrawingTemplate;
}
namespace kernel
{
    class GlTools_ABC;
}
namespace
{
    class GlToolsSymbols;
}

// =============================================================================
/** @class  ADN_Symbols_Data
    @brief  ADN_Symbols_Data
*/
// Created: SBO 2011-04-18
// =============================================================================
class ADN_Symbols_Data : public ADN_Data_ABC
{
public:
    class SymbolInfo : public ADN_Ref_ABC
                     , public ADN_DataTreeNode_ABC
    {
    public:
        //! @name Constructors/Destructor
        //@{
                 SymbolInfo( xml::xistream& xis, svg::TextRenderer& renderer, kernel::GlTools_ABC& tools );
        virtual ~SymbolInfo();
        //@}

        //! @name Accessors
        //@{
        virtual std::string GetNodeName();
        std::string GetItemName();
        const QPixmap& GetPixmap() const;
        const std::string GetCode() const;
        const std::string GetGeometry() const;
        //@}

    private:
        //! @name Helpers
        //@{
        void Initialize();
        void Draw();
        void DrawOnPoint();
        void DrawOnLine();
        void DrawOnPolygon();
        void DrawItem( const T_PointVector& points );
        //@}

    private:
        //! @name Private member data
        //@{
        gui::DrawingTemplate* template_;
        kernel::GlTools_ABC& tools_;
        QPixmap* pixmap_;
        //@}

    public:
        //! @name Public member data
        //@{
        ADN_Type_String strName_;
        //@}
    };

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Symbols_Data();
    virtual ~ADN_Symbols_Data();
    //@}

    //! @name Types
    //@{
    typedef ADN_Type_Vector_ABC< SymbolInfo > T_SymbolInfoVector;
    //@}

    //! @name Operations
    //@{
    virtual void FilesNeeded( T_StringList& files ) const;
    virtual void Reset();
    SymbolInfo* const GetSymbol( const std::string& code ) const;
    T_SymbolInfoVector& GetSymbols( const std::string geometries );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadArchive( xml::xistream& xis );
    void ReadCategory( xml::xistream& xis );
    void ReadTemplate( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    std::map< std::string, T_SymbolInfoVector > symbolsMap_;
    T_SymbolInfoVector symbols_;
    svg::TextRenderer  renderer_;
    ::GlToolsSymbols*  tools_;
    //@}
};

#endif // __ADN_Symbols_Data_h_
