package web;

import java.io.File;
import java.io.IOException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.io.FileUtils;
import org.apache.log4j.Logger;
import org.eclipse.jetty.server.Request;
import org.eclipse.jetty.server.handler.AbstractHandler;
import org.thymeleaf.TemplateEngine;
import org.thymeleaf.context.Context;
import org.thymeleaf.templateresolver.FileTemplateResolver;

public class Handler extends AbstractHandler {

    @SuppressWarnings("unused")
    private static final Logger log_ = Logger.getLogger(Handler.class);
    private final TemplateEngine engine_;
    private final File root_;

    public Handler(final String root) throws Exception {
        root_ = new File(root);
        if (!root_.isDirectory())
            throw new Exception(root_ + " is not a directory");
        final FileTemplateResolver resolver = new FileTemplateResolver();
        resolver.setTemplateMode("XHTML");
        resolver.setPrefix(root_.getAbsolutePath());
        resolver.setSuffix(".html");
        engine_ = new TemplateEngine();
        engine_.setTemplateResolver(resolver);
    }

    private static void serveFile(final HttpServletResponse response, final File file) throws IOException {
        response.setContentType("bwahaha");
        response.setStatus(HttpServletResponse.SC_OK);
        FileUtils.copyFile(file, response.getOutputStream());
    }

    private void serveTemplate(final HttpServletResponse response, final String target) throws IOException {
        response.setContentType("text/html");
        response.setStatus(HttpServletResponse.SC_OK);
        final Context ctx = new Context();
        engine_.process(target, ctx, response.getWriter());
    }

    @Override
    public void handle(final String uri, final Request base, final HttpServletRequest request, final HttpServletResponse response) throws IOException {
        File target;
        if (base.getMethod() != "GET")
            response.sendError(HttpServletResponse.SC_BAD_REQUEST, "invalid method " + base.getMethod());
        else if ((target = new File(root_, uri)).isFile())
            serveFile(response, target);
        else if ((target = new File(root_, uri + ".html")).isFile())
            serveTemplate(response, uri);
        else
            response.sendError(HttpServletResponse.SC_NOT_FOUND);
        base.setHandled(true);
    }
}
