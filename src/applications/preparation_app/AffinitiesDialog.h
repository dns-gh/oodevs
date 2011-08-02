// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef _AffinitiesDialog_h
#define _AffinitiesDialog_h

#include "Preparation/Types.h"
#include "clients_kernel/SafePointer.h"
#include "tools/ElementObserver_ABC.h"
#include <map>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/tuple/tuple.hpp>

namespace gui
{
    class DecimalSpinBoxAndSlider;
}

namespace kernel
{
    class Controllers;
    class Team_ABC;
    class Entity_ABC;
}

// =============================================================================
/** @class  AffinitiesDialog
    @brief  AffinitiesDialog
*/
// Created: LGY 2011-03-16
// =============================================================================
class AffinitiesDialog : public QDialog
                       , private boost::noncopyable
                       , public tools::Observer_ABC
                       , public tools::ElementObserver_ABC< kernel::Team_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             AffinitiesDialog( QWidget* parent, kernel::Controllers& controllers, bool optional );
    virtual ~AffinitiesDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyCreated( const kernel::Team_ABC& team );
    virtual void NotifyUpdated( const kernel::Team_ABC& team );
    virtual void NotifyDeleted( const kernel::Team_ABC& team );
    //@}

private slots:
    //! @name Slots
    //@{
    void Show();
    void Activated( bool value );
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
    typedef std::map< unsigned long, EntityAffinity > T_Affinities;
    //@}

protected:
    //! @name Member data
    //@{
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    QVBoxLayout* pMainLayout_;
    Q3HBox* pButtonLayout_;
    QCheckBox* pCheckBox_;
    Q3Grid* pGrid_;
    T_Teams teams_;
    T_Affinities affinities_;
    T_QAffinities spinboxs_;
    //@}
};

#endif // _AffinitiesDialog_h
