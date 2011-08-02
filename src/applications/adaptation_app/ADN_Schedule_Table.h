// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef _ADN_Schedule_Table_h
#define _ADN_Schedule_Table_h

// =============================================================================
/** @class  ADN_Schedule_Table
    @brief  ADN schedule table
*/
// Created: LGY 2011-01-17
// =============================================================================
class ADN_Schedule_Table : public Q3Table
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Schedule_Table( QWidget* pParent );
    virtual ~ADN_Schedule_Table();
    //@}

public slots:
    //! @name Slots
    //@{
    void OnContextMenu( int row, int col, const QPoint& point );
    void OnPeopleChanged( void* pData );
    void OnValueChanged( int row, int col );
    void OnTimeChanged( const QTime& time );
    //@}

private:
    //! @name Helpers
    //@{
    QStringList CreateWeek() const;
    QStringList CreateAccommodations() const;
    void AddEvent( int index, unsigned int day, const std::string& from,
                   const std::string& to, const std::string& motivation );
    void AddRow( int rows, unsigned int day = 0u, const std::string& from = "00:00", const std::string& to = "00:00",
                 const std::string& accommodation = "" );
    void Remove( int rows );
    //@}

private:
    //! @name Member data
    //@{
    void* pCurData_;
    //@}
};

#endif // _ADN_Schedule_Table_h
