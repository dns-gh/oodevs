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

#include "ADN_ListView.h"

class ADN_Type_LocalizedString;

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
             ADN_ListView_DescriptionAttachment( E_MissionType missionType );
    virtual ~ADN_ListView_DescriptionAttachment();
    //@}

    //! @name Operations
    //@{
    virtual void contextMenuEvent( QContextMenuEvent* event );
    bool IsFileInList( const QString& fileName );
    //@}

public slots:
    //! @name slots
    //@{
    void AddFile();
    void CopyName();
    void RemoveFile();
    void OnItemSelected( void* );
    //@}

private:
    //! @name Helpers
    //@{
    tools::Path GetImageDir( std::string key ) const;
    //@}

private:
    //! @name members
    //@{
    E_MissionType missionType_;
    ADN_Type_LocalizedString* missionName_;
    //@}
};

#endif // __ADN_ListView_DescriptionAttachment_h_
