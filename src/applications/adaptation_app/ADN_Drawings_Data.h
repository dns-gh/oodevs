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
#include "clients_gui/DrawingTemplate.h"

namespace svg
{
    class TextRenderer;
}

namespace xml
{
    class xistream;
}
namespace kernel
{
    class GlTools_ABC;
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
                      , public gui::DrawingTemplate
    {
    public:
        //! @name Constructors/Destructor
        //@{
        DrawingInfo( xml::xistream& xis, const QString& category, const QString& id, svg::TextRenderer& renderer )
            : ADN_Ref_ABC()
            , gui::DrawingTemplate( xis, category, id, renderer )
        {
            strName_ = GetName().toStdString();
        }
        ~DrawingInfo() {}
        virtual std::string GetNodeName() { return ""; }
        std::string GetItemName() { return ""; }
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
    typedef ADN_Type_Vector_ABC< DrawingInfo >   T_DrawingInfoVector;
    typedef std::map< std::string, T_DrawingInfoVector >  T_DrawingsMap;
    //@}

    //! @name Operations
    //@{
    virtual void FilesNeeded( T_StringList& files ) const;
    virtual void Load( const tools::Loader_ABC& fileLoader );
    virtual void Reset();
    DrawingInfo* const GetDrawing( const std::string& code ) const;
    T_DrawingInfoVector& GetGeometryDrawings( const std::string& geometries, const std::string& category );
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
    std::auto_ptr< svg::TextRenderer > renderer_;
    std::auto_ptr< kernel::GlTools_ABC > tools_;
    //@}
};

#endif // __ADN_Drawings_Data_h_
