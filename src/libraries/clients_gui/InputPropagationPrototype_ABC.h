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

namespace gui
{
    
// =============================================================================
/** @class  PropagationPrototype_ABC
    @brief  PropagationPrototype_ABC
*/
// Created: JCR 2008-06-30
// =============================================================================
class InputPropagationPrototype_ABC : public ObjectAttributePrototype_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    explicit InputPropagationPrototype_ABC( QWidget* parent );
    virtual ~InputPropagationPrototype_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
    //@}

private slots:
    //! @name slots
    //@{
    void LoadPath();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    InputPropagationPrototype_ABC( const InputPropagationPrototype_ABC& );            //!< Copy constructor
    InputPropagationPrototype_ABC& operator=( const InputPropagationPrototype_ABC& ); //!< Assignement operator
    //@}

protected:
    //! @name Member data
    //@{
    std::string source_;    
    ValuedComboBox< std::string >* dataField_;
    QPushButton* pPathButton_;
    QCheckBox* exportData_;
    //@}
};

}

#endif // __InputPropagationPrototype_ABC_h_
