package process;

import java.io.Closeable;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.Timer;
import java.util.TimerTask;

import org.apache.log4j.Logger;

public class ProcessHandler {

    private static final Logger logger_ = Logger
            .getLogger(ProcessHandler.class);

    private final String name_;
    private final ProcessBuilder builder_;
    private Process process_;
    private DestroyThread destroyer_;
    private OutputSink stdout_;
    private OutputSink stderr_;

    public ProcessHandler(final String name, final String... args) {
        name_ = name;
        builder_ = new ProcessBuilder(args);
        process_ = null;
        stdout_ = null;
        stderr_ = null;
    }

    private OutputSink setSink(final String name, final InputStream input) {
        if (input == null)
            return null;
        final OutputSink sink = new OutputSink(name_ + "_" + name, input);
        sink.start();
        return sink;
    }

    public void start(final String runDirectory) throws IOException {
        logger_.info(name_ + " start");
        builder_.directory(new File(runDirectory));
        process_ = builder_.start();
        destroyer_ = new DestroyThread(name_ + "_destroyer", process_);
        Runtime.getRuntime().addShutdownHook(destroyer_);
        stdout_ = setSink("stdout", process_.getInputStream());
        stderr_ = setSink("stderr", process_.getErrorStream());
    }

    private class DestroyThread extends Thread {
        private final Process process_;

        DestroyThread(final String name, final Process process) {
            super(name);
            setDaemon(true);
            process_ = process;
        }

        @Override
        public void run() {
            process_.destroy();
        }
    }

    private class InterruptTimerTask extends TimerTask {

        private final Thread thread_;

        public InterruptTimerTask(final Thread thread) {
            thread_ = thread;
        }

        @Override
        public void run() {
            thread_.interrupt();
        }
    }

    public int join(final int msTimeout) {
        Timer timer = null;
        if (msTimeout > 0) {
            timer = new Timer(true);
            final InterruptTimerTask interrupter = new InterruptTimerTask(
                    Thread.currentThread());
            timer.schedule(interrupter, msTimeout);
        }
        try {
            final int reply = process_.waitFor();
            stdout_.join();
            stderr_.join();
            return reply;
        } catch (final InterruptedException e) {
            logger_.warn(name_ + " interrupted");
        } finally {
            if (timer != null)
                timer.cancel();
            // http://bugs.sun.com/view_bug.do?bug_id=6420270
            Thread.interrupted();
            destroy();
        }
        return -1;
    }

    public void stop() {
        destroy();
    }

    private static void closeStream(final Closeable stream) {
        if (stream == null)
            return;
        try {
            stream.close();
        } catch (final IOException e) {
            // NOTHING
        }
    }

    private void destroy() {
        if (process_ == null)
            return;
        logger_.info(name_ + " end");
        // explicitly release resources now
        process_.destroy();
        Runtime.getRuntime().removeShutdownHook(destroyer_);
        closeStream(process_.getErrorStream());
        closeStream(process_.getInputStream());
        closeStream(process_.getOutputStream());

    }
}
