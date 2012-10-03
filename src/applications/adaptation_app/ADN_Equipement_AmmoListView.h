// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Equipement_AmmoListView_h_
#define __ADN_Equipement_AmmoListView_h_

#include "ADN_ListView.h"
#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  ADN_Equipement_AmmoListView
    @brief  ADN_Equipement_AmmoListView
*/
// Created: APE 2004-12-29
// =============================================================================
class ADN_Equipement_AmmoListView : public ADN_ListView
                                  , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Equipement_AmmoListView( QWidget* pParent );
    virtual ~ADN_Equipement_AmmoListView();
    //@}

private:
    //! @name Operations
    //@{
    void        ConnectItem  ( bool           bConnect );
    void        OnContextMenu( const QPoint&  pt       );
    std::string GetToolTipFor( const QModelIndex& index );
    //@}
};

#endif // __ADN_Equipement_AmmoListView_h_
