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

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  FilterPartiesListView
    @brief  FilterPartiesListView
*/
// Created: ABR 2012-05-29
// =============================================================================
class FilterPartiesListView : public Q3ListView
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
    void OnItemClicked( Q3ListViewItem* item, const QPoint& point, int column );
    //@}

private:
    //! @name Types
    //@{
    enum E_Column { eCheckbox = 0, eHiddenCheckbox = 1, eHiddenPartyID = 2, ePartyName  = 3 };
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
    //@}
};

#endif // __FilterPartiesListView_h_
