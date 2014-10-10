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

namespace kernel
{
    class OptionsController;
}

namespace gui
{
    class ColorButton;
    struct TerrainSetting;
    class SizeButton;

// =============================================================================
/** @class  TerrainPreference
    @brief  Terrain preference
*/
// Created: SBO 2006-04-04
// =============================================================================
class TerrainPreference : public QWidget
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TerrainPreference( kernel::OptionsController& options,
                                const std::shared_ptr< TerrainSetting >& settings,
                                QWidget* parent = 0 );
    virtual ~TerrainPreference();
    //@}

    //! @name Operations
    //@{
    bool IsChecked() const;
    //@}

public slots:
    //! @name Operations
    //@{
    void SetEnabled( bool value );
    //@}

private:
    //! @name Member data
    //@{
    kernel::OptionsController& options_;
    ColorButton* colorButton_;
    SizeButton* sizeButton_;
    QCheckBox* showCheckbox_;
    //@}
};

} //! namespace gui

#endif // __TerrainPreference_h_
