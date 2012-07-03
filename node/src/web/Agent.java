package web;

import java.util.EnumSet;

import javax.servlet.DispatcherType;

import org.apache.log4j.Logger;
import org.eclipse.jetty.server.Server;
import org.eclipse.jetty.server.nio.SelectChannelConnector;
import org.eclipse.jetty.servlet.DefaultServlet;
import org.eclipse.jetty.servlet.FilterHolder;
import org.eclipse.jetty.servlet.ServletContextHandler;
import org.eclipse.jetty.servlet.ServletHolder;
import org.eclipse.jetty.servlets.GzipFilter;

public class Agent {

    private static final Logger log_ = Logger.getLogger(Agent.class);
    private final Server server_;

    public static class Configuration {
        public String uuid;
        public String www;
        public String type;
        public String name;
        public int port;
        public int host;
        public boolean isDebug;
    };

    public Agent(final Configuration config) throws Exception {

        final ServletContextHandler ctx = new ServletContextHandler(ServletContextHandler.NO_SECURITY | ServletContextHandler.NO_SESSIONS);
        ctx.setResourceBase(config.www);

        final ServletHolder files = new ServletHolder(new DefaultServlet());
        files.setInitParameter("dirAllowed", "false");
        files.setInitParameter("cacheControl", "public");
        files.setInitParameter("acceptRanges", "true");
        files.setInitParameter("useFileMappedBuffer", "true");
        ctx.addServlet(files, "/css/*");
        ctx.addServlet(files, "/img/*");
        ctx.addServlet(files, "/js/*");
        ctx.addServlet(files, "/favicon.ico");

        final ServletHolder handler = new ServletHolder(new Handler(config));
        ctx.addServlet(handler, "/");

        final EnumSet<DispatcherType> all = EnumSet.of(DispatcherType.ASYNC, DispatcherType.ERROR, DispatcherType.FORWARD, DispatcherType.INCLUDE,
                DispatcherType.REQUEST);
        final FilterHolder zipper = new FilterHolder(new GzipFilter());
        ctx.addFilter(zipper, "*", all);

        server_ = new Server();
        server_.setHandler(ctx);

        final SelectChannelConnector connector = new SelectChannelConnector();
        connector.setPort(config.port);
        server_.addConnector(connector);
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
