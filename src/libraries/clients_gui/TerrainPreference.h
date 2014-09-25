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

namespace xml
{
    class xistream;
}

namespace kernel
{
    class Controllers;
    class Options;
}

namespace gui
{
    class ColorButton;
    class SizeButton;

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
    void Display( QVBoxLayout* parent, QCheckBox* box );
    void SetLineWidth() const;
    void SetColor( float alpha ) const;
    void Commit();
    void Revert();

    void Save() const;
    //@}

public slots:
    //! @name Operations
    //@{
    void SetEnabled( bool value );
    //@}

private slots:
    //! @name Operations
    //@{
    void OnStateChanged( int state );
    void OnSizeChanged( int size );
    void OnColorChanged( const QColor& color );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TerrainPreference( const TerrainPreference& );            //!< Copy constructor
    TerrainPreference& operator=( const TerrainPreference& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::Options& options_;
    std::string type_;
    std::string name_;
    bool previousShown_;
    bool shown_;
    ColorButton* colorButton_;
    SizeButton* sizeButton_;
    QGroupBox* groupBox_;
    QCheckBox* showCheckbox_;
    float lineWidth_;
    float previousLineWidth_;
    QColor color_;
    QColor previousColor_;
    //@}
};

}

#endif // __TerrainPreference_h_
