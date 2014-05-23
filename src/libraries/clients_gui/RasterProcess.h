// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef CLIENTS_GUI_RASTER_PROCESS_H
#define CLIENTS_GUI_RASTER_PROCESS_H

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

namespace tools
{
    class Path;
    class ExerciseConfig;
}

class QProcess;

typedef boost::function<void (int, const tools::Path&, const std::string&)> RasterCallback;

// This should be an implementation detail but cmake current Qt moc'ing
// rules force me to put it in a header. I prefer to avoid adding another
// header at the price of putting this here.
class RasterProcess : public QProcess
{
    Q_OBJECT

public:
    RasterProcess( const RasterCallback& callback, const tools::Path& output,
           const tools::Path& logfile );
    virtual ~RasterProcess();

private slots:
    void OnExit( int errorCode, QProcess::ExitStatus exitStatus );
    void OnError( QProcess::ProcessError );

private:
    void Fire( int code, const tools::Path& output );

private:
    RasterCallback callback_;
    tools::Path output_;
    tools::Path logfile_;
    bool fired_;
};

// Execute raster_app.exe on the input file with specified pixel quality.
// Return the spawned QProcess instance, and eventually fire the callback
// with a zero-exit code on success and the path of generated texture layer,
// or a non-zero exit code. Generated texture files is only valid during
// the callback execution.
boost::shared_ptr< QProcess > RunRasterApp( const tools::Path& input,
    int pixelSize, const tools::ExerciseConfig& config, const RasterCallback& callback );

#endif  // CLIENTS_GUI_RASTER_PROCESS_H
