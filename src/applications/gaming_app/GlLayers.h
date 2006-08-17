// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GlLayers_h_
#define __GlLayers_h_

#include "clients_gui/GlProxy.h"

class Controllers;
class Model;
class StaticModel;
class ColorStrategy;
class ParametersLayer;
class GraphicSetup_ABC;
class AgentsLayer;

// =============================================================================
/** @class  GlLayers
    @brief  GlLayers
*/
// Created: AGE 2006-03-29
// =============================================================================
class GlLayers : public GlProxy
{

public:
    //! @name Constructors/Destructor
    //@{
             GlLayers( Controllers& controllers, const StaticModel& staticModel, Model& model, GraphicSetup_ABC& setup );
    virtual ~GlLayers();
    //@}

    //! @name Operations
    //@{
    void RegisterBaseLayers();
    //@}

    //! @name Accessors
    //@{
    // $$$$ AGE 2006-03-31: 
    ParametersLayer& GetParametersLayer() { return *parameters_; };
    AgentsLayer&     GetAgentLayer() { return *agents_; }; // $$$$ AGE 2006-05-18: used for aggregation.
    //@}

private:
    //! @name Copy/Assignement
    //@{
    GlLayers( const GlLayers& );            //!< Copy constructor
    GlLayers& operator=( const GlLayers& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    const StaticModel& static_;
    Model& model_;
    GraphicSetup_ABC& setup_;

    ColorStrategy* strategy_;
    ParametersLayer* parameters_;
    AgentsLayer* agents_;
    //@}
};

#endif // __GlLayers_h_
