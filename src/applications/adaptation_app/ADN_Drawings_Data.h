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
#include "ADN_RefWithName.h"
#include "ADN_Types.h"
#include "clients_gui/DrawingTemplate.h"

namespace svg
{
    class TextRenderer;
}

namespace gui
{
    class GLView_ABC;
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
    class DrawingInfo : public ADN_RefWithName
                      , public gui::DrawingTemplate
    {
    public:
        //! @name Constructors/Destructor
        //@{
        DrawingInfo( xml::xistream& xis, const QString& category, const QString& id, svg::TextRenderer& renderer )
            : ADN_RefWithName()
            , gui::DrawingTemplate( xis, category, id, renderer )
        {
            strName_ = GetName().toStdString();
        }
        ~DrawingInfo() {}
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
    typedef ADN_Type_Vector_ABC< DrawingInfo > T_DrawingInfoVector;
    typedef std::map< std::string, T_DrawingInfoVector >  T_DrawingsMap;
    //@}

    //! @name Operations
    //@{
    virtual void FilesNeeded( tools::Path::T_Paths& files ) const;
    DrawingInfo* const GetDrawing( const std::string& code ) const;
    T_DrawingInfoVector& GetGeometryDrawings( const std::string& geometries, const std::string& category );
    T_DrawingInfoVector& GetCategoryDrawings( const std::string& category );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadArchive( xml::xistream& xis );
    void ReadCategory( xml::xistream& xis );
    void ReadTemplate( xml::xistream& xis, const std::string& name, const std::string& id );
    //@}

private:
    //! @name Member data
    //@{
    T_DrawingsMap geometryMap_;
    T_DrawingsMap categoryMap_;
    T_DrawingInfoVector drawings_;
    std::unique_ptr< svg::TextRenderer > renderer_;
    std::unique_ptr< gui::GLView_ABC > tools_;
    //@}
};

#endif // __ADN_Drawings_Data_h_
