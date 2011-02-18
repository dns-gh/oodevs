// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __LimitsLayer_h_
#define __LimitsLayer_h_

#include "clients_gui/TacticalLinesLayer.h"
#include "tools/ElementObserver_ABC.h"

class TacticalLineFactory;

// =============================================================================
/** @class  LimitsLayer
    @brief  LimitsLayer
*/
// Created: AGE 2006-03-24
// =============================================================================
class LimitsLayer : public gui::TacticalLinesLayer
                  , public tools::SelectionObserver_Base< kernel::Automat_ABC >
                  , public tools::SelectionObserver_Base< kernel::Formation_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             LimitsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::ParametersLayer& parameters, TacticalLineFactory& factory, gui::View_ABC& view, const kernel::Profile_ABC& profile );
    virtual ~LimitsLayer();
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool CanCreateLine();
    virtual void Delete( const kernel::TacticalLine_ABC& line );
    virtual bool ShouldDisplay( const kernel::Entity_ABC& );
    virtual void CreateLimit( const T_PointVector& points );
    virtual void CreateLima( const T_PointVector& points );

    virtual void BeforeSelection();
    virtual void Select( const kernel::TacticalLine_ABC& entity );
    virtual void Select( const kernel::Automat_ABC& entity );
    virtual void Select( const kernel::Formation_ABC& entity );
    virtual void AfterSelection();
    //@}

private:
    //! @name Member data
    //@{
    const kernel::GlTools_ABC& tools_;
    TacticalLineFactory& factory_;
    kernel::SafePointer< kernel::Entity_ABC > selectedEntity_;
    //@}
};

#endif // __LimitsLayer_h_
