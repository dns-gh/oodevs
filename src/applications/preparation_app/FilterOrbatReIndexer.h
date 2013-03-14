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

class QCheckBox;
class QLineEdit;

namespace xml
{
    class xistream;
    class xostream;
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
    virtual QWidget* CreateParametersWidget( QWidget* parent );
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
    QLineEdit*         filename_;
    bool               isUrbanBlock_;

    QCheckBox*         objectsCheckBox_;
    QCheckBox*         crowdsCheckBox_;
    QCheckBox*         populationsCheckBox_;
    QCheckBox*         initialStateCheckBox_;
    QCheckBox*         logisticLinksCheckBox_;
    QCheckBox*         stocksCheckBox_;
    QCheckBox*         diplomacyCheckBox_;

    FilterPartiesListView* partiesListView_;

    unsigned long          shift_;
    //@}
};

#endif // __FilterOrbatReIndexer_h_
