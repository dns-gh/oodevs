    // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamLimaList_h_
#define __ParamLimaList_h_

#include "ListParameter.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controller;
    class ActionController;
    class OrderParameter;
    class CoordinateConverter_ABC;
    class TacticalLine_ABC;
}

namespace actions
{
    namespace gui
    {
        class LimaParameter;

// =============================================================================
/** @class  ParamLimaList
    @brief  Lima Parameter
*/
// Created: SBO 2006-11-14
// =============================================================================
class ParamLimaList : public ListParameter
                    , public tools::ElementObserver_ABC< kernel::TacticalLine_ABC >
                    , public kernel::ContextMenuObserver_ABC< kernel::TacticalLine_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamLimaList( QObject* parent, const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, kernel::ActionController& actions, kernel::Controller& controller, const QDateTime& currentDate );
    virtual ~ParamLimaList();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* BuildInterface( QWidget* parent );
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void MenuItemValidated( int index );
    //@}

private:
    //! @name Helpers
    //@{
    virtual Param_ABC* CreateElement();
    virtual void DeleteElement( Param_ABC& param );

    virtual void NotifyContextMenu( const kernel::TacticalLine_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyDeleted( const kernel::TacticalLine_ABC& entity );
    //@}

    //! @name Types
    //@{
    typedef std::map< const kernel::TacticalLine_ABC*, LimaParameter* > T_Limas;
    typedef T_Limas::const_iterator                                   CIT_Limas;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::CoordinateConverter_ABC& converter_;
    kernel::OrderParameter parameter_;
    const QDateTime currentDate_;
    unsigned int count_;
    T_Limas limas_;
    const kernel::TacticalLine_ABC* potential_;
    //@}
};

    }
}

#endif // __ParamLimaList_h_
