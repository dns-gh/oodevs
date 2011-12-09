// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ProfileConsistencyDialog_h_
#define __ProfileConsistencyDialog_h_

#include <boost/noncopyable.hpp>
#include <string>
#include <vector>

namespace kernel
{
    class Entity_ABC;
}

// =============================================================================
/** @class  ProfileConsistencyDialog
    @brief  Profile consistency dialog
*/
// Created: LGY 2011-12-09
// =============================================================================
class ProfileConsistencyDialog : public QDialog
                               , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ProfileConsistencyDialog( QWidget* parent );
    virtual ~ProfileConsistencyDialog();
    //@}

    //! @name Types
    //@{
    typedef std::pair< std::string, const kernel::Entity_ABC* > T_Error;
    typedef std::vector< T_Error >                              T_Errors;
    //@}

    //! @name Operations
    //@{
    int Exec( const T_Errors& errors );
    //@}

private:
    //! @name Member data
    //@{
    QTableView* tableView_;
    QStandardItemModel* dataModel_;
    //@}
};

#endif // __ProfileConsistencyDialog_h_
