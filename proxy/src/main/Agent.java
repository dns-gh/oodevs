package main;

import java.io.File;
import java.util.ArrayList;
import java.util.EnumSet;
import java.util.List;

import javax.servlet.DispatcherType;

import org.apache.log4j.Logger;
import org.eclipse.jetty.server.Connector;
import org.eclipse.jetty.server.Server;
import org.eclipse.jetty.server.nio.SelectChannelConnector;
import org.eclipse.jetty.server.ssl.SslSelectChannelConnector;
import org.eclipse.jetty.servlet.FilterHolder;
import org.eclipse.jetty.servlet.ServletContextHandler;
import org.eclipse.jetty.servlet.ServletHolder;
import org.eclipse.jetty.servlets.GzipFilter;

public class Agent {

    private static final Logger log_ = Logger.getLogger(Agent.class);
    private final Server server_;

    public static class Configuration {
        public int port;
        public boolean isDebug;
        public int ssl;
        public String store;
        public String type;
        public String password;
    };

    public Agent(final Configuration config) throws Exception {

        final ServletContextHandler ctx = new ServletContextHandler(ServletContextHandler.NO_SECURITY | ServletContextHandler.NO_SESSIONS);

        final ServletHolder handler = new ServletHolder(new Handler(config));
        ctx.addServlet(handler, "/");

        final EnumSet<DispatcherType> all = EnumSet.of(DispatcherType.ASYNC, DispatcherType.ERROR, DispatcherType.FORWARD, DispatcherType.INCLUDE,
                DispatcherType.REQUEST);
        final FilterHolder zipper = new FilterHolder(new GzipFilter());
        ctx.addFilter(zipper, "*", all);

        server_ = new Server();
        server_.setHandler(ctx);

        final List<Connector> links = new ArrayList<Connector>();

        final SelectChannelConnector plain = new SelectChannelConnector();
        plain.setPort(config.port);
        links.add(plain);

        final File sslFile = new File(config.store);
        if (sslFile.isFile()) {
            final SslSelectChannelConnector ssl = new SslSelectChannelConnector();
            ssl.setPort(config.ssl);
            final org.eclipse.jetty.util.ssl.SslContextFactory scf = ssl.getSslContextFactory();
            scf.setExcludeProtocols("SSLv2Hello");
            scf.setKeyStoreType(config.type);
            String path = config.store;
            if (!new File(path).isAbsolute())
                path = sslFile.getParentFile() + File.separator + path;
            scf.setKeyStorePath(path);
            scf.setKeyStorePassword(config.password);
            links.add(ssl);
        }

        server_.setConnectors(links.toArray(new Connector[] {}));
    }

    public void exec() {
        try {
            server_.start();
            server_.join();
        } catch (final Exception err) {
            log_.error(err.toString());
            log_.error("Internal server error");
        }
    }
}
