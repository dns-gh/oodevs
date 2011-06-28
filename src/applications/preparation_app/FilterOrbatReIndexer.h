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
class QListView;

namespace xml
{
    class xistream;
    class xostream;
}

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
             FilterOrbatReIndexer( const tools::ExerciseConfig& config, Model& model );
    virtual ~FilterOrbatReIndexer();
    //@}

    //! @name Operations
    //@{
    virtual void Execute();
    virtual const std::string GetName() const;
    virtual const std::string GetDescription() const;
    virtual QWidget* CreateParametersWidget( QWidget* parent );
    virtual bool IsValid() const;
   //@}

private:
    //! @name Helpers
    //@{
    void LoadPreview();
    void ReadTeam( xml::xistream& xis );
    void ReadField( const std::string& name, xml::xistream& xis, xml::xostream& xos );
    void ReadAttribute( const std::string& name, xml::xistream& xis, xml::xostream& xos );
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
    const std::string  orbatFile_;
    QLineEdit*         filename_;
    QCheckBox*         objectsCheckBox_;
    QCheckBox*         populationsCheckBox_;
    QListView*         preview_;
    unsigned int       shift_;
    //@}
};

#endif // __FilterOrbatReIndexer_h_
