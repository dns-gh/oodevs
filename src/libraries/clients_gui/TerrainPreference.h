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

#include "clients_kernel/OptionsObserver_ABC.h"

class TerrainData;

namespace xml
{
    class xistream;
}

namespace kernel
{
    class Controllers;
    class OptionsController;
}

namespace gui
{
    class ColorButton;
    class SizeButton;
    class RichCheckBox;

// =============================================================================
/** @class  TerrainPreference
    @brief  Terrain preference
*/
// Created: SBO 2006-04-04
// =============================================================================
class TerrainPreference : public QWidget
                        , public tools::Observer_ABC
                        , public kernel::OptionsObserver_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TerrainPreference( xml::xistream& xis, kernel::Controllers& controllers );
    virtual ~TerrainPreference();
    //@}

    //! @name Operations
    //@{
    void Display( QWidget* parent );
    void SetLineWidth() const;
    void SetColor( float alpha ) const;
    void Revert();
    void Commit();

    void Save() const;
    //@}

public slots:
    //! @name Slots
    //@{
    void OnCategoryChecked();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::OptionsController& options_;
    std::string type_;
    std::string name_;
    bool currentShown_;
    bool previousShown_;
    ColorButton* colorButton_;
    SizeButton*  sizeButton_;
    RichCheckBox* showCheckbox_;
    float  lineWidth_;
    QColor color_;
    //@}
};

}

#endif // __TerrainPreference_h_
