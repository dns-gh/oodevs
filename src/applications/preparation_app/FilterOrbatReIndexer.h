// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FilterOrbatReIndexer_h_
#define __FilterOrbatReIndexer_h_

#include "Filter.h"

namespace xml
{
    class xistream;
    class xostream;
}

namespace gui
{
    class RichLineEdit;
    class RichCheckBox;
}

class FilterPartiesListView;
class Model;

// =============================================================================
/** @class  FilterOrbatReIndexer
    @brief  FilterOrbatReIndexer
*/
// Created: SBO 2008-04-08
// =============================================================================
class FilterOrbatReIndexer : public Filter
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             FilterOrbatReIndexer( QWidget* mainwindow, const tools::ExerciseConfig& config, Model& model );
    virtual ~FilterOrbatReIndexer();
    //@}

    //! @name Operations
    //@{
    virtual void Execute();
    virtual const std::string GetName() const;
    virtual const std::string GetDescription() const;
    virtual QWidget* CreateParametersWidget( const QString& objectName, QWidget* parent );
   //@}

signals:
    //! @name Signals
    //@{
    void DoConsistencyCheck();
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool IsValid() const;
    void ReadField( const std::string& name, xml::xistream& xis, xml::xostream& xos );
    void ReadAttribute( const std::string& name, xml::xistream& xis, xml::xostream& xos );

    void ReadDiplomacy( xml::xistream& xis, xml::xostream& xos );
    void ReadDiplomacyLink( xml::xistream& xis, xml::xostream& xos );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnBrowse();
    //@}

private:
    //! @name Member data
    //@{
    Model&             model_;
    const tools::Path  orbatFile_;
    gui::RichLineEdit* filename_;
    bool               isUrbanBlock_;

    gui::RichCheckBox* objectsCheckBox_;
    gui::RichCheckBox* crowdsCheckBox_;
    gui::RichCheckBox* populationsCheckBox_;
    gui::RichCheckBox* initialStateCheckBox_;
    gui::RichCheckBox* logisticLinksCheckBox_;
    gui::RichCheckBox* stocksCheckBox_;
    gui::RichCheckBox* diplomacyCheckBox_;

    FilterPartiesListView* partiesListView_;

    unsigned long          shift_;
    //@}
};

#endif // __FilterOrbatReIndexer_h_
