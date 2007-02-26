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

namespace gui
{
    class LightingProxy;
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
             LightingPanel( QWidget* parent, LightingProxy& lighting );
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
    void OnLightingType( int type );

    void DirectionChanged( const geometry::Vector3f& direction );
    void AmbientChanged( const QColor& color );
    void DiffuseChanged( const QColor& color );
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
    LightingProxy& lighting_;

    QGroupBox* fixedLightBox_;
    DirectionWidget* direction_;
    ColorButton* ambient_;
    ColorButton* diffuse_;
    //@}
};

}

#endif // __LightingPanel_h_
