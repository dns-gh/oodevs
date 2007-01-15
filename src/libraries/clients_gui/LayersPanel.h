// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LayersPanel_h_
#define __LayersPanel_h_

#include "PreferencePanel_ABC.h"

namespace gui
{
    class Layer2d_ABC;

// =============================================================================
/** @class  LayersPanel
    @brief  LayersPanel
*/
// Created: AGE 2007-01-04
// =============================================================================
class LayersPanel : public PreferencePanel_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    explicit LayersPanel( QWidget* parent );
    virtual ~LayersPanel();
    //@}

    //! @name Operations
    //@{
    void AddLayer( const QString& name, Layer2d_ABC& layer );

    virtual void Commit();
    virtual void Reset();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnValueChanged( int );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LayersPanel( const LayersPanel& );            //!< Copy constructor
    LayersPanel& operator=( const LayersPanel& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::pair< Layer2d_ABC*, QSlider* > T_Layer;
    typedef std::vector< T_Layer >              T_Layers;
    typedef std::vector< float >                T_Alphas;
    //@}

private:
    //! @name Member data
    //@{
    T_Layers layers_;
    T_Alphas current_;
    T_Alphas new_;
    //@}
};

}

#endif // __LayersPanel_h_
