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
#include "clients_kernel/OptionsObserver_ABC.h"

class TerrainData;

namespace xml { class xistream; };

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
                        , public GraphicPreference_ABC
                        , public tools::Observer_ABC
                        , public kernel::OptionsObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             TerrainPreference( xml::xistream& xis, kernel::Controllers& controllers );
    virtual ~TerrainPreference();
    //@}

    //! @name Operations
    //@{
    virtual void Display( QWidget* parent );
    void SetLineWidth() const;
    void SetColor( float alpha ) const;
    virtual void Commit();
    virtual void Revert();

    void Save() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TerrainPreference( const TerrainPreference& );            //!< Copy constructor
    TerrainPreference& operator=( const TerrainPreference& ); //!< Assignement operator
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
    ColorButton* colorButton_;
    SizeButton*  sizeButton_;
    float  lineWidth_;
    QColor color_;
    //@}
};

}

#endif // __TerrainPreference_h_
