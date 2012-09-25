// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __FilterPartiesListView_h_
#define __FilterPartiesListView_h_

// =============================================================================
/** @class  FilterPartiesListView
    @brief  FilterPartiesListView
*/
// Created: ABR 2012-05-29
// =============================================================================
class FilterPartiesListView : public QTreeView
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit FilterPartiesListView( QWidget* parent, bool checkedByDefault = true );
    virtual ~FilterPartiesListView();
    //@}

    //! @name Operations
    //@{
    bool IsPartyChecked( unsigned long partyID );
    bool ParseOrbatFile( const std::string& fileName );
    QString GetTeamList();
    //@}

signals:
    //! @name Signals
    //@{
    void ValueChanged();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnItemClicked( const QModelIndex& index );
    //@}

    //! @name Helpers
    //@{
    void ReadTeam( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    bool    checkedByDefault_;
    QPixmap checkedPixmap_;
    QPixmap uncheckedPixmap_;
    QStandardItemModel model_;
    //@}
};

#endif // __FilterPartiesListView_h_
