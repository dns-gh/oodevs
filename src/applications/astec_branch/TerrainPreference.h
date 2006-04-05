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

// =============================================================================
/** @class  TerrainPreference
    @brief  Terrain preference
*/
// Created: SBO 2006-04-04
// =============================================================================
class TerrainPreference : public QWidget
                        , public GraphicPreference_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit TerrainPreference( const std::string& name );
    virtual ~TerrainPreference();
    //@}

    //! @name Operations
    //@{
    virtual void Read();
    virtual void Write() const;
    virtual void Display( QWidget* parent );

    float GetLineWidth() const { return lineWidth_; }
    //@}

private slots:
    //! @name Slots
    //@{
    void SlotSetLineWidth();
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
    const std::string name_;
    ColorButton* colorButton_;
    SizeButton*  sizeButton_;
    float  lineWidth_;
    //@}
};

#endif // __TerrainPreference_h_
