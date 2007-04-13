// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AutomatsLayer_h_
#define __AutomatsLayer_h_

#include "EntityLayer.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace gui
{
    class AgentsLayer;

// =============================================================================
/** @class  AutomatsLayer
    @brief  AutomatsLayer
*/
// Created: SBO 2007-04-12
// =============================================================================
class AutomatsLayer : public QObject
                    , public EntityLayer< kernel::Automat_ABC >
                    , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             AutomatsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view, const kernel::Profile_ABC& profile, gui::AgentsLayer& agents );
    virtual ~AutomatsLayer();
    //@}

    //! @name Operations
    //@{
    void Aggregate   ( const kernel::Automat_ABC& automat );
    void Disaggregate( const kernel::Automat_ABC& automat );
    //@}

private slots:
    //! @name Slots
    //@{
    void Aggregate   ();
    void Disaggregate();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AutomatsLayer( const AutomatsLayer& );            //!< Copy constructor
    AutomatsLayer& operator=( const AutomatsLayer& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual bool ShouldDisplay( const kernel::Entity_ABC& );
    virtual void NotifyContextMenu( const kernel::Automat_ABC&, kernel::ContextMenu& );
    virtual void Select( const kernel::Entity_ABC&, bool );
    virtual void ContextMenu( const kernel::Entity_ABC&, const geometry::Point2f&, const QPoint& );
    virtual void NotifySelected( const kernel::Automat_ABC* automat );
    void Toggle( const kernel::Automat_ABC& automat, bool aggregate );
    //@}

    //! @name Types
    //@{
    typedef std::set< const kernel::Entity_ABC* > T_Automats;
    //@}

private:
    //! @name Member data
    //@{
    gui::AgentsLayer& agents_;
    kernel::SafePointer< kernel::Automat_ABC > selected_;
    T_Automats aggregated_;
    //@}
};

}

#endif // __AutomatsLayer_h_
