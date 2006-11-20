// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LimitParameter_h_
#define __LimitParameter_h_

#include "gaming/ASN_Types.h"
#include "Param_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"

namespace gui
{
    class RichLabel;
}

class Limit;

// =============================================================================
/** @class  LimitParameter
    @brief  LimitParameter
*/
// Created: SBO 2006-11-14
// =============================================================================
class LimitParameter : public QHBox
                     , public Param_ABC
                     , public kernel::ContextMenuObserver_ABC< Limit >
                     , public kernel::ElementObserver_ABC< Limit >

{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LimitParameter( QWidget* pParent, ASN1T_Line& limit, const QString& label, const QString& menu );
    virtual ~LimitParameter();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity();
    virtual void Commit();
    void CommitTo( ASN1T_Line& asn );
    virtual void Draw( const geometry::Point2f& point, const geometry::Rectangle2f& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void MenuItemValidated();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LimitParameter( const LimitParameter& );            //!< Copy constructor
    LimitParameter& operator=( const LimitParameter& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void Display( const QString& what );
    virtual void NotifyContextMenu( const Limit& entity, kernel::ContextMenu& menu );
    virtual void NotifyUpdated( const Limit& ) {};
    virtual void NotifyDeleted( const Limit& entity );
    //@}

private:
    //! @name Member data
    //@{
    ASN1T_Line&     result_;
    std::string     menu_;
    gui::RichLabel* pLabel_;
    QLabel*         entityLabel_; // $$$$ AGE 2006-03-14: LabelDisplayer ?
    const Limit*    potential_;
    const Limit*    selected_;
    //@}
};

#endif // __LimitParameter_h_
