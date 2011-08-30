// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Drawings_Data_h_
#define __ADN_Drawings_Data_h_

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
/** @class  ADN_Drawings_Data
    @brief  ADN_Drawings_Data
*/
// Created: SBO 2011-04-18
// =============================================================================
class ADN_Drawings_Data : public ADN_Data_ABC
{
public:
    class DrawingInfo : public ADN_Ref_ABC
                      , public ADN_DataTreeNode_ABC
    {
    public:
        //! @name Constructors/Destructor
        //@{
                 DrawingInfo( xml::xistream& xis, svg::TextRenderer& renderer, kernel::GlTools_ABC& tools, const std::string& category );
        virtual ~DrawingInfo();
        //@}

        //! @name Accessors
        //@{
        virtual std::string GetNodeName();
        std::string GetItemName();
        const QPixmap& GetPixmap() const;
        const std::string GetCode() const;
        const std::string GetGeometry() const;
        const std::string& GetCategory() const;
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
        std::string category_;
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
             ADN_Drawings_Data();
    virtual ~ADN_Drawings_Data();
    //@}

    //! @name Types
    //@{
    typedef ADN_Type_Vector_ABC< DrawingInfo >            T_DrawingInfoVector;
    typedef T_DrawingInfoVector::iterator                IT_DrawingInfoVector;
    typedef T_DrawingInfoVector::const_iterator         CIT_DrawingInfoVector;
    typedef std::map< std::string, T_DrawingInfoVector >  T_DrawingsMap;
    typedef T_DrawingsMap::iterator                      IT_DrawingsMap;
    //@}

    //! @name Operations
    //@{
    virtual void FilesNeeded( T_StringList& files ) const;
    virtual void Load( const tools::Loader_ABC& fileLoader );
    virtual void Reset();
    DrawingInfo* const GetDrawing( const std::string& code ) const;
    T_DrawingInfoVector& GetGeometryDrawings( const std::string geometries );
    T_DrawingInfoVector& GetCategoryDrawings( const std::string& category );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadArchive( xml::xistream& xis );
    void ReadCategory( xml::xistream& xis );
    void ReadTemplate( xml::xistream& xis, const std::string& name );
    //@}

private:
    //! @name Member data
    //@{
    T_DrawingsMap geometryMap_;
    T_DrawingsMap categoryMap_;
    T_DrawingInfoVector drawings_;
    svg::TextRenderer renderer_;
    ::GlToolsSymbols* tools_;
    //@}
};

#endif // __ADN_Drawings_Data_h_
