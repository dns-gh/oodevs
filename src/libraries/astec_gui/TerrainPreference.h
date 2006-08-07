// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TerrainPreference_h_
#define __TerrainPreference_h_

#include "GraphicPreference_ABC.h"

class TerrainData;
class ColorButton;
class SizeButton;

namespace xml { class xistream; class xostream; };

// =============================================================================
/** @class  TerrainPreference
    @brief  Terrain preference
*/
// Created: SBO 2006-04-04
// =============================================================================
class TerrainPreference : public QWidget
                        , public GraphicPreference_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit TerrainPreference( xml::xistream& xis );
    virtual ~TerrainPreference();
    //@}

    //! @name Operations
    //@{
    virtual void Display( QWidget* parent );
    void SetLineWidth() const;
    void SetColor( float alpha ) const;
    virtual void Commit();
    virtual void Revert();

    void Save( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TerrainPreference( const TerrainPreference& );            //!< Copy constructor
    TerrainPreference& operator=( const TerrainPreference& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    ColorButton* colorButton_;
    SizeButton*  sizeButton_;
    float  lineWidth_;
    QColor color_;
    //@}
};

#endif // __TerrainPreference_h_
