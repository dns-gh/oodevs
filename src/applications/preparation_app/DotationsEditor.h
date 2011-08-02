// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DotationsEditor_h_
#define __DotationsEditor_h_

#include "tools/Resolver.h"
#include "clients_kernel/ValueEditor.h"

namespace kernel
{
    class DotationType;
}

class Dotation;
class DotationsItem;

// =============================================================================
/** @class  DotationsEditor
    @brief  DotationsEditor
*/
// Created: SBO 2006-11-10
// =============================================================================
class DotationsEditor : public QDialog
                      , public kernel::ValueEditor< DotationsItem* >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             DotationsEditor( QDialog*& self, QWidget* parent, const tools::Resolver_ABC< kernel::DotationType, std::string >& dotationTypes, DotationsItem*& value );
    virtual ~DotationsEditor();
    //@}

    //! @name Operations
    //@{
    void SetCurrentItem( DotationsItem*& dotations );
    virtual DotationsItem* GetValue();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnAccept();
    void OnReject();
    void OnValueChanged( int row, int col );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DotationsEditor( const DotationsEditor& );            //!< Copy constructor
    DotationsEditor& operator=( const DotationsEditor& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual QSize sizeHint() const;
    void AddItem( const Dotation* dotation = 0 );
    //@}

private:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::DotationType, std::string >& dotationTypes_;
    DotationsItem** value_;
    Q3Table* table_;
    QStringList types_;
    QDialog*& self_;
    //@}
};

#endif // __DotationsEditor_h_
