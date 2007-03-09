// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SymbolIcons_h_
#define __SymbolIcons_h_

#include "IconHandler_ABC.h"

namespace gui
{
    class GlWidget;

// =============================================================================
/** @class  SymbolIcons
    @brief  SymbolIcons
*/
// Created: AGE 2006-11-22
// =============================================================================
class SymbolIcons : public QObject, private IconHandler_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    explicit SymbolIcons( QObject* parent );
    virtual ~SymbolIcons();
    //@}

    //! @name Operations
    //@{
    virtual QPixmap GetSymbol( const std::string& symbol, const QColor& color = Qt::white );
    virtual QPixmap GetSymbol( const std::string& symbol, const std::string& level, const QColor& color = Qt::white );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnWidget2dChanged( gui::GlWidget* );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SymbolIcons( const SymbolIcons& );            //!< Copy constructor
    SymbolIcons& operator=( const SymbolIcons& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    struct Key
    {
        Key( const std::string& symbol, const std::string& level, const QColor& color )
            : symbol_( symbol )
            , level_( level )
            , color_( color )
        {}

        bool operator<( const Key& rhs ) const
        {
            return make_pair( make_pair( symbol_, level_ ), color_.rgb() )
                < make_pair( make_pair( rhs.symbol_, rhs.level_ ), rhs.color_.rgb() );
        }
        std::string symbol_, level_;
        QColor color_;
    };

    typedef std::map< Key, QPixmap >            T_Icons;
    typedef std::set< Key >                     T_PendingIcons;
    typedef T_PendingIcons::const_iterator      CIT_PendingIcons;
    //@}

    //! @name Helpers
    //@{
    virtual void AddIcon( const std::string& symbol, const std::string& level, const QColor& color, const QPixmap& icon );
    //@}

private:
    //! @name Member data
    //@{
    GlWidget* widget_;
    T_PendingIcons pending_;
    T_Icons icons_;
    //@}
};

}

#endif // __SymbolIcons_h_
