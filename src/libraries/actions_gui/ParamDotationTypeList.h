// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamDotationTypeList_h_
#define __ParamDotationTypeList_h_

#include "Param_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "tools/Resolver.h"

namespace kernel
{
    class DotationType;
    class OrderParameter;
}

namespace actions
{
    namespace gui
    {

// =============================================================================
/** @class  ParamDotationTypeList
    @brief  ParamDotationTypeList
*/
// Created: SBO 2006-08-09
// =============================================================================
class ParamDotationTypeList : public QObject
                            , public Param_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamDotationTypeList( QObject* parent, const kernel::OrderParameter& parameter, const tools::Resolver_ABC< kernel::DotationType >& resolver );
    virtual ~ParamDotationTypeList();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* BuildInterface( QWidget* parent );
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void Clicked( Q3ListViewItem* item );
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool IsOptional() const;
    void AddItem( const QString& parent, const QString& child, unsigned id );
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    const tools::Resolver_ABC< kernel::DotationType >& resolver_;
    Q3ListView* list_;
    //@}
};

    }
}

#endif // __ParamDotationTypeList_h_
