// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef _AgentAffinitiesItem_h
#define _AgentAffinitiesItem_h

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include <map>
#include <vector>
#include <boost/tuple/tuple.hpp>
#include <boost/noncopyable.hpp>

namespace gui
{
    class DecimalSpinBoxAndSlider;
}

namespace kernel
{
    class Agent_ABC;
    class Controllers;
    class Team_ABC;
}

// =============================================================================
/** @class  AgentAffinitiesDialog
    @brief  Agent Affinities dialog
*/
// Created: LGY 2011-03-14
// =============================================================================
class AgentAffinitiesDialog : public QDialog
                            , private boost::noncopyable
                            , public tools::Observer_ABC
                            , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC > 
                            , public tools::ElementObserver_ABC< kernel::Team_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             AgentAffinitiesDialog( QWidget* parent, kernel::Controllers& controllers );
    virtual ~AgentAffinitiesDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyCreated( const kernel::Team_ABC& team );
    virtual void NotifyUpdated( const kernel::Team_ABC& team );
    virtual void NotifyDeleted( const kernel::Team_ABC& team );
    //@}

private slots:
    //! @name Slots
    //@{
    void Show();
    void Actived( bool value );
    void Validate();
    void Reject();
    void closeEvent( QCloseEvent* pEvent );
    //@}

private:
    //! @name Helpers
    //@{
    void Clear();
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< unsigned long, std::string > T_Teams;
    typedef boost::tuples::tuple< unsigned long, QLabel*, gui::DecimalSpinBoxAndSlider* > T_QAffinity;
    typedef std::vector< T_QAffinity >                                                    T_QAffinities;
    typedef std::map< unsigned long, float > T_Affinities;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::Agent_ABC* selected_;
    QVBoxLayout* pMainLayout_;
    QHBox* pButtonLayout_;
    QCheckBox* pCheckBox_;
    QGrid* pGrid_;
    T_Teams teams_;
    T_Affinities affinities_;
    T_QAffinities spinboxs_;
    //@}
};


#endif // _AgentAffinitiesItem_h
