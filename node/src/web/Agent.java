package web;

import java.io.IOException;

import org.apache.log4j.Logger;
import org.eclipse.jetty.server.Server;

public class Agent {

    private static final Logger log_ = Logger.getLogger(Agent.class);

    private final Server server_;
    private final Handler handler_;

    public Agent(final String root, final int webPort, final int hostPort, final boolean isDebug) throws IOException {
        server_ = new Server(webPort);
        handler_ = new Handler(root, isDebug);
        server_.setHandler(handler_);
    }

    public void exec() {
        try {
            server_.start();
            server_.join();
        } catch (final Exception err) {
            log_.error("unable to start server: " + err.toString());
        }
    }
}
