// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LightingPanel_h_
#define __LightingPanel_h_

#include "PreferencePanel_ABC.h"

class FixedLighting;

namespace gui
{
    class DirectionWidget;
    class ColorButton;

// =============================================================================
/** @class  LightingPanel
    @brief  LightingPanel
*/
// Created: SBO 2007-01-03
// =============================================================================
class LightingPanel : public PreferencePanel_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             LightingPanel( QWidget* parent, FixedLighting& lighting );
    virtual ~LightingPanel();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    virtual void Reset();
    //@}

private slots:
    //! @name Slots
    //@{
    void DirectionChanged( const geometry::Vector3f& direction );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LightingPanel( const LightingPanel& );            //!< Copy constructor
    LightingPanel& operator=( const LightingPanel& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    FixedLighting& lighting_;
    QButtonGroup* lightingType_;
    DirectionWidget* direction_;
    ColorButton* ambient_;
    ColorButton* diffuse_;
    //@}
};

}

#endif // __LightingPanel_h_
