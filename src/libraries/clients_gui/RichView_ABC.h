// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __gui_RichView_ABC_h_
#define __gui_RichView_ABC_h_

#include "CheckComboBox.h"
#include "NumericLimitsEditor.h"
#include "WidgetMenu.h"
#include "MT_Tools/MT_Logger.h"

namespace gui
{
    class RichTreeView;
    class Filterable_ABC;
    class SearchLineEdit;

// =============================================================================
/** @class  RichView_ABC
    @brief  RichView_ABC
*/
// Created: ABR 2012-03-27
// =============================================================================
class RichView_ABC : public QWidget
{
    Q_OBJECT

public:
    //! @name Types
    //@{
    enum E_Options
    {
        eOptions_None                   = 0x00,

        eOptions_SearchLineEdit         = 0x01,
        eOptions_FilterLineEdit         = 0x02,
        eOptions_FilterBox              = 0x04,
        eOptions_FilterMenu             = 0x08,
        eOptions_ClearButton            = 0x10,

        eOptions_All                    = 0xFF,
    };
    enum E_FilterAreas
    {
        eFilterAreas_Box  = 0x01,
        eFilterAreas_Menu = 0x02
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit RichView_ABC( int options, QWidget* parent );
    virtual ~RichView_ABC();
    //@}

    //! @name Operations
    //@{
    void Load();
    void Purge();
    bool HasOption( int option ) const;
    bool IsFiltered() const;
    //@}

    //! @name Filters operations
    //@{
    void AddCheckComboBoxFilter( int col,
                                 int areas,
                                 const QString& text,
                                 bool useNone,
                                 bool useCheckAll,
                                 const CheckComboBox::T_Extractor& extractor,
                                 const CheckComboBox::T_Filler& filler );
    template< typename NumericType, typename SpinBox >
    void AddNumericFilter( int col,
                           int areas,
                           const QString& text,
                           const typename NumericLimitsEditor< NumericType, SpinBox >::T_Extractor& extractor,
                           NumericType min,
                           NumericType max );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnClearFilters();
    void OnFiltersChanged( QWidget* widget = 0 );
    //@}

signals:
    //! @name Signals
    //@{
    void FiltersStatusChanged( bool filtered );
    //@}

protected:
    //! @name Helpers
    //@{
    void CreateGUI();
    //@}

private:
    //! @name Helpers
    //@{
    bool ApplyAllFilter( const StandardModel& model, QStandardItem& item ) const;

    bool IsValidArea( int area ) const;
    WidgetMenu* GetMenu( int col );
    void AddWidgetToFilterBox( int col, const QString& text, const std::shared_ptr< Filter_ABC >& filter, QWidget* widget );
    void AddWidgetToMenu( int col, const QString& text, const std::shared_ptr< Filter_ABC >& filter, QWidget* widget );

    std::shared_ptr< CheckComboBox > CreateCheckCombo( QWidget* parent,
                                                       bool useNone,
                                                       bool useCheckAll,
                                                       const CheckComboBox::T_Extractor& extractor,
                                                       const CheckComboBox::T_Filler& filler );
    template< typename NumericType, typename SpinBox >
    std::shared_ptr< NumericLimitsEditor< NumericType, SpinBox > > CreateNumericEditor( QWidget* parent,
                                                                                        const typename NumericLimitsEditor< NumericType, SpinBox >::T_Extractor& extractor,
                                                                                        NumericType min,
                                                                                        NumericType max );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnHeaderContextMenuRequested( const QPoint& pos );
    //@}

protected:
    //! @name Member data
    //@{
    int options_;
    QStandardItemModel* model_;
    QAbstractItemView* view_;
    Filterable_ABC* filterable_;

    std::map< int, std::vector< std::shared_ptr< Filter_ABC > > > filters_;
    std::map< int, std::unique_ptr< WidgetMenu > > menus_;
    std::unique_ptr< QWidget > searchLineWidget_;
    std::unique_ptr< QWidget > filterBox_;
    std::unique_ptr< QBoxLayout > mainLayout_;
    std::unique_ptr< QGridLayout > filtersLayout_;
    std::unique_ptr< QSignalMapper > signalMapper_;
    //@}
};

} //! namespace gui

#include "RichView_ABC.inl"

#endif // __gui_RichView_ABC_h_
