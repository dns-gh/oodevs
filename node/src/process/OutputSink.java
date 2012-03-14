package process;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import org.apache.log4j.Logger;

public class OutputSink extends Thread {

    private static final Logger logger_ = Logger.getLogger(OutputSink.class);

    private final BufferedInputStream input_;

    OutputSink(final String name, final InputStream input) {
        super(name);
        setDaemon(true);
        input_ = new BufferedInputStream(input);
    }

    @Override
    public void run() {
        final InputStreamReader reader = new InputStreamReader(input_);
        final BufferedReader buffer = new BufferedReader(reader);
        try {
            while (buffer.readLine() != null)
                continue;
        } catch (final IOException e) {
            logger_.error(getName() + " aborted");
        }
    }
}
