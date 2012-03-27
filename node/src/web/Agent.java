package web;

import org.apache.log4j.Logger;
import org.eclipse.jetty.client.HttpClient;
import org.eclipse.jetty.server.Server;
import org.eclipse.jetty.util.thread.QueuedThreadPool;

public class Agent {

    private static final Logger log_ = Logger.getLogger(Agent.class);

    private final Server server_;
    private final HttpClient host_;
    private final Handler handler_;

    public Agent(final String root, final int webPort, final int hostPort, final boolean isDebug) throws Exception {
        server_ = new Server(webPort);
        host_ = new HttpClient();
        host_.setConnectorType(HttpClient.CONNECTOR_SELECT_CHANNEL);
        host_.setMaxConnectionsPerAddress(200);
        host_.setThreadPool(new QueuedThreadPool(250));
        host_.setTimeout(30000);
        host_.start();
        handler_ = new Handler(host_, hostPort, root, isDebug);
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
