// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DisasterPrototype_ABC_h_
#define __DisasterPrototype_ABC_h_

#include "ObjectAttributePrototype_ABC.h"
#include "ValuedComboBox.h"

namespace tools
{
    class GeneralConfig;
}

namespace gui
{
    class RichCheckBox;
    class RichLabel;

// =============================================================================
/** @class  DisasterPrototype_ABC
    @brief  Disaster prototype declaration
*/
// Created: LGY 2012-11-20
// =============================================================================
class DisasterPrototype_ABC : public ObjectAttributePrototype_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             DisasterPrototype_ABC( QWidget* parent, const tools::GeneralConfig& config );
    virtual ~DisasterPrototype_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity( const kernel::Team_ABC& ) const;
    //@}

private slots:
    //! @name 
    //@{
    void OnStateChanged( int state );
    //@}

private:
    //! @name Helpers
    //@{
    void FillInPaths( const tools::Path& root );
    //@}

protected:
    //! @name Member data
    //@{
    gui::RichLabel* sourceLabel_;
    ValuedComboBox< std::string >* propagationFiles_;
    RichCheckBox* checkbox_;
    QDateTimeEdit* date_;
    //@}
};

}

#endif // __DisasterPrototype_ABC_h_
