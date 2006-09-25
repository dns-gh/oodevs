// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __DiplomacyCell_h_
#define __DiplomacyCell_h_

#include <qtable.h>

class Diplomacy;

// =============================================================================
/** @class  DiplomacyCell
    @brief  DiplomacyCell
*/
// Created: SBO 2006-09-25
// =============================================================================
class DiplomacyCell : public QObject
                    , public QComboTableItem
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             DiplomacyCell( QTable* table, const QStringList& list );
    virtual ~DiplomacyCell();
    //@}

    //! @name Operations
    //@{
    void SetColor( const Diplomacy& diplomacy, const QColor& color );
    Diplomacy GetValue() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DiplomacyCell( const DiplomacyCell& );            //!< Copy constructor
    DiplomacyCell& operator=( const DiplomacyCell& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void paint( QPainter* p, const QColorGroup& cg, const QRect& cr, bool selected );
    virtual QWidget* createEditor() const;
    //@}

private slots:
    //! @name private slots
    //@{
    void OnTextChanged( const QString& );
    //@}
    
private:
    std::map< const QString, QColor > colors_;
    std::map< const QString, Diplomacy > diplomacies_;
    bool valueSet_;
};

#endif // __DiplomacyCell_h_
