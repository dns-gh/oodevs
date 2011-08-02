// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __frontend_app_CreatePackagePanel_h_
#define __frontend_app_CreatePackagePanel_h_

#include "Panel_ABC.h"
#include <Qt3Support/q3action.h>

namespace frontend
{
    class Config;
}
namespace zip
{
    class ozipfile;
}

class InfoBubble;
class QLineEdit;
class Q3ListView;
class Q3ProgressBar;
class QPushButton;
class Q3TextEdit;
class Q3ListBoxItem;
class Q3CheckListItem;

// =============================================================================
/** @class  CreatePackagePanel
    @brief  CreatePackagePanel
*/
// Created: JCR 2009-11-09
// =============================================================================
class CreatePackagePanel : public Panel_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             CreatePackagePanel( Q3WidgetStack* widget, Q3Action& action, const frontend::Config& config, ActionsContext& context );
    virtual ~CreatePackagePanel();
    //@}

private slots:
    //! @name Operations
    //@{
    void CreatePackage();
    void OnSelectionChanged( Q3ListBoxItem* item );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CreatePackagePanel( const CreatePackagePanel& );            //!< Copy constructor
    CreatePackagePanel& operator=( const CreatePackagePanel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Update();
    void UpdateExercises();
    std::string GetDestinationDirectory() const;

    bool BrowseClicked();
    Q3ListViewItem* BuildExerciseFeatures( const std::string& exercise );
    Q3ListViewItem* BuildExerciseData( const std::string& exercise );
    void BuildCategory( Q3ListViewItem* parent, const QStringList& list, const QString& base, const std::string& category );
    void WriteContent( zip::ozipfile& archive ) const;
    //@}

    typedef std::pair< std::string, std::string > T_Package; // <path, filename>

private:
    //! @name Member data
    //@{
    const frontend::Config& config_;
    Q3ListBox*  list_;
    T_Package package_;
    Q3TextEdit* description_;
    Q3ListView* content_;
    Q3ProgressBar* progress_;
    QPushButton* okay_;
    InfoBubble* bubble_;
    //@}
};

#endif // __frontend_app_CreatePackagePanel_h_
