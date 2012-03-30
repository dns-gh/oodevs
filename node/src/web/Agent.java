package web;

import java.util.EnumSet;

import javax.servlet.DispatcherType;

import org.apache.log4j.Logger;
import org.eclipse.jetty.server.Server;
import org.eclipse.jetty.servlet.DefaultServlet;
import org.eclipse.jetty.servlet.FilterHolder;
import org.eclipse.jetty.servlet.ServletContextHandler;
import org.eclipse.jetty.servlet.ServletHolder;
import org.eclipse.jetty.servlets.GzipFilter;
import org.eclipse.jetty.servlets.ProxyServlet;

public class Agent {

    private static final Logger log_ = Logger.getLogger(Agent.class);
    private final Server server_;

    public Agent(final String root, final int webPort, final int hostPort, final boolean isDebug) throws Exception {

        final ServletContextHandler ctx = new ServletContextHandler(ServletContextHandler.NO_SECURITY | ServletContextHandler.NO_SESSIONS);
        ctx.setResourceBase(root);

        final ServletHolder files = new ServletHolder(new DefaultServlet());
        files.setInitParameter("dirAllowed", "false");
        ctx.addServlet(files, "/css/*");
        ctx.addServlet(files, "/img/*");
        ctx.addServlet(files, "/js/*");

        final ServletHolder proxy = new ServletHolder(new ProxyServlet.Transparent("/api", "localhost", hostPort));
        ctx.addServlet(proxy, "/api/*");

        final ServletHolder handler = new ServletHolder(new Handler(root, isDebug));
        ctx.addServlet(handler, "/");

        final EnumSet<DispatcherType> all = EnumSet.of(DispatcherType.ASYNC, DispatcherType.ERROR, DispatcherType.FORWARD, DispatcherType.INCLUDE,
                DispatcherType.REQUEST);
        final FilterHolder zipper = new FilterHolder(new GzipFilter());
        ctx.addFilter(zipper, "*", all);

        server_ = new Server(webPort);
        server_.setHandler(ctx);
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
