// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-09-13 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __ADN_TableItem_TimeField_h_
#define __ADN_TableItem_TimeField_h_

#include "ADN_Table.h"
#include "ADN_TableItem_ABC.h"

#pragma warning( push, 0 )
#include <Qt3Support/q3table.h>
#pragma warning( pop )

class ADN_TimeField;
class QRegExpValidator;

// =============================================================================
/** @class  ADN_TableItem_TimeField
    @brief  ADN_TableItem_TimeField
    @par    Using example
    @code
    ADN_TableItem_TimeField;
    @endcode
*/
// Created: SBO 2005-09-13
// =============================================================================
class ADN_TableItem_TimeField : public ADN_TableItem_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_TableItem_TimeField( ADN_Table* parent, void *data, Q3TableItem::EditType nEditType = OnTyping );
    virtual ~ADN_TableItem_TimeField();
    //@}

    //! @name Operations
    //@{
    QRegExpValidator& GetValidator() const;
    void SetValidator( QRegExpValidator* pValidator );
    //@}

public slots:
    void OnValueChanged();

protected:
    //! @name Operations
    //@{
    virtual QWidget* createEditor() const;
    virtual void     setContentFromEditor( QWidget* pWidget );
    virtual void     DoValueChanged();
    virtual void     SetAlignment( int nAlignment );
    virtual int      alignment() const;
    //@}

private:
    //! @name Member data
    //@{
    mutable ADN_TimeField* pTimeField_;
    QRegExpValidator*      pValidator_;
    int                    nAlignment_;
    //@}

};

#endif // __ADN_TableItem_TimeField_h_
