package web;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.log4j.Logger;
import org.eclipse.jetty.server.Request;
import org.eclipse.jetty.server.handler.AbstractHandler;

public class Handler extends AbstractHandler {

    @SuppressWarnings("unused")
    private static final Logger log_ = Logger.getLogger(Handler.class);

    @Override
    public void handle(final String target, final Request request, final HttpServletRequest servletRequest, final HttpServletResponse reply)
            throws IOException, ServletException {
        reply.setContentType("text/html");
        reply.setStatus(HttpServletResponse.SC_OK);
        reply.getWriter().println("<h1>Hello</h1>");
        request.setHandled(true);
    }

}
