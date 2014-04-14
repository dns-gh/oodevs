// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_ListView_FragOrderTypes_h_
#define __ADN_ListView_FragOrderTypes_h_

#include "ADN_ListView.h"
#include "ADN_Missions_Data.h"
#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  ADN_ListView_FragOrderTypes
    @brief  ADN_ListView_FragOrderTypes
*/
// Created: SBO 2006-12-06
// =============================================================================
class ADN_ListView_FragOrderTypes : public ADN_ListView
                                  , private boost::noncopyable
{
       Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             ADN_ListView_FragOrderTypes( QWidget* pParent, ADN_Missions_Data::T_Mission_Vector& orders );
    virtual ~ADN_ListView_FragOrderTypes();
    //@}

private:
    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    void OnContextMenu( const QPoint& pt );
    std::string GetToolTipFor( const QModelIndex& index );
    bool ContextMenuDelete();
    //@}

signals:
    //! @name Signals
    //@{
    void NotifyElementDeleted( boost::shared_ptr< kernel::LocalizedString > name, E_MissionType type );
    //@}

private:
    //! @name Member data
    //@{
    ADN_Missions_Data::T_Mission_Vector& orders_;
    //@}
};

#endif // __ADN_ListView_FragOrderTypes_h_
