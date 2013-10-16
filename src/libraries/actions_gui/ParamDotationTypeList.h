// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ParamDotationTypeList_h_
#define __ParamDotationTypeList_h_

#include "Param_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include <tools/Resolver.h>

namespace kernel
{
    class DotationType;
    class OrderParameter;
}

namespace actions
{
    namespace gui
    {
        class InterfaceBuilder_ABC;

// =============================================================================
/** @class  ParamDotationTypeList
    @brief  ParamDotationTypeList
*/
// Created: SBO 2006-08-09
// =============================================================================
class ParamDotationTypeList : public Param_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ParamDotationTypeList( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamDotationTypeList();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* BuildInterface( const QString& objectName, QWidget* parent );
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    virtual bool InternalCheckValidity() const;
    //@}

private slots:
    //! @name Slots
    //@{
    void Clicked( const QModelIndex& index );
    //@}

private:
    //! @name Helpers
    //@{
    void AddItem( const QString& parent, const QString& child, unsigned id );
    //@}

private:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::DotationType >& resolver_;
    QTreeView* list_;
    QStandardItemModel model_;
    //@}
};

    }
}

#endif // __ParamDotationTypeList_h_
