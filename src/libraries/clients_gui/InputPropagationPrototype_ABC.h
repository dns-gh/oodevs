// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InputPropagationPrototype_ABC_h_
#define __InputPropagationPrototype_ABC_h_

#include "ObjectAttributePrototype_ABC.h"
#include "ValuedComboBox.h"

namespace tools
{
    class GeneralConfig;
}

namespace gui
{
    class RichLabel;

// =============================================================================
/** @class  PropagationPrototype_ABC
    @brief  PropagationPrototype_ABC
*/
// Created: JCR 2008-06-30
// =============================================================================
class InputPropagationPrototype_ABC : public ObjectAttributePrototype_ABC
{
    Q_OBJECT;  // $$$$ _RC_ JSR 2011-02-24: semble inutile?

public:
    //! @name Constructors/Destructor
    //@{
    explicit InputPropagationPrototype_ABC( QWidget* parent, const tools::GeneralConfig& config );
    virtual ~InputPropagationPrototype_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
    //@}

private:
    //! @name
    //@{
    void FillInPaths();
    //@}

protected:
    //! @name Member data
    //@{
    gui::RichLabel* sourceLabel_;
    const std::string root_;
    ValuedComboBox< std::string >* propagationFiles_;
    ValuedComboBox< std::string >* dataField_;
    QCheckBox* exportData_;
    //@}
};

}

#endif // __InputPropagationPrototype_ABC_h_
