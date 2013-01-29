// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef UrbanColorEditor_h
#define UrbanColorEditor_h

#include "clients_kernel/ValueEditor.h"
#include "clients_kernel/UrbanColor_ABC.h"

// =============================================================================
/** @class  UrbanColorEditor
    @brief  Urban color editor
*/
// Created: LGY 2012-08-17
// =============================================================================
class UrbanColorEditor : public QColorDialog
                       , public kernel::ValueEditor< kernel::UrbanBlockColor >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             UrbanColorEditor( QWidget* parent );
    virtual ~UrbanColorEditor();
    //@}

    //! @name Operations
    //@{
    void SetValue( kernel::UrbanBlockColor& value );
    virtual kernel::UrbanBlockColor GetValue();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnAccept( const QColor& color );
    //@}

private:
    //! @name Operations
    //@{
    virtual void showEvent( QShowEvent* event );
    virtual void done( int result );
    //@}

private:
    //! @name Member data
    //@{
    QColor initial_;
    kernel::UrbanBlockColor value_;
    //@}
};

#endif // UrbanColorEditor_h
