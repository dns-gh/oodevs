// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamAtlasNature_h_
#define __ParamAtlasNature_h_

#include "Param_ABC.h"
#include "clients_kernel/AtlasNature.h"

namespace kernel
{
    class OrderParameter;
    class AtlasNatures;
}

namespace actions
{
    namespace gui
    {
        class InterfaceBuilder_ABC;

// =============================================================================
/** @class  ParamAtlasNature
    @brief  ParamAtlasNature
*/
// Created: AGE 2006-03-15
// =============================================================================
class ParamAtlasNature : public Param_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ParamAtlasNature( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamAtlasNature();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* BuildInterface( const QString& objectName, QWidget* parent );
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    virtual void Visit( const actions::parameters::AtlasNature& param );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnClicked( int id );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< const kernel::AtlasNature* > T_NatureFields;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::AtlasNatures& natures_;
    kernel::AtlasNature nature_;
    T_NatureFields fields_;
    std::vector< QCheckBox* > checkboxes_;
    //@}
};

    }
}

#endif // __ParamAtlasNature_h_
