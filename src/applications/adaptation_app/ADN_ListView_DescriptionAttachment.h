// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ListView_DescriptionAttachment_h_
#define __ADN_ListView_DescriptionAttachment_h_

#include <boost/noncopyable.hpp>
#include "ADN_EditLine_ABC.h"
#include "ADN_ListView.h"

// =============================================================================
/** @class  ADN_ListView_DescriptionAttachment
    @brief  ADN_ListView_DescriptionAttachment
*/
// Created: NPT 2013-01-16
// =============================================================================
class ADN_ListView_DescriptionAttachment : public ADN_ListView
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ADN_ListView_DescriptionAttachment( E_MissionType missionType, ADN_EditLine_ABC* missionName );
    virtual ~ADN_ListView_DescriptionAttachment();
    //@}

    //! @name Operations
    //@{
    virtual void contextMenuEvent( QContextMenuEvent* event );
    bool IsFileInList( const QString& fileName );
    //@}

    //! @name slots
    //@{
public slots:
    void AddFile();
    void CopyName();
    void RemoveFile();
    //@}

private:
    //! @name members
    //@{
    E_MissionType missionType_;
    ADN_EditLine_ABC* missionName_;
    //@}
};

#endif // __ADN_ListView_DescriptionAttachment_h_
