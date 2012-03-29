package web;

import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.log4j.Logger;

import freemarker.template.Configuration;
import freemarker.template.DefaultObjectWrapper;
import freemarker.template.Template;
import freemarker.template.TemplateException;

public class Handler extends HttpServlet {

    private static final long serialVersionUID = 1564095421395727712L;
    @SuppressWarnings("unused")
    private static final Logger log_ = Logger.getLogger(Handler.class);
    private final Configuration cfg_;
    private final File root_;

    public Handler(final String root, final boolean isDebug) throws Exception {
        root_ = new File(root);
        if (!root_.isDirectory())
            throw new IOException(root_ + " is not a directory");
        cfg_ = new Configuration();
        cfg_.setDirectoryForTemplateLoading(root_);
        cfg_.setObjectWrapper(new DefaultObjectWrapper());
        if (isDebug)
            cfg_.setTemplateUpdateDelay(0);
        cfg_.setOutputEncoding("UTF-8");
    }

    private void serveTemplate(final HttpServletResponse response, final HttpServletRequest request, final String target) throws IOException {
        response.setContentType("text/html");
        response.setStatus(HttpServletResponse.SC_OK);
        final Template ctx = cfg_.getTemplate(target);
        final Map<String, String> root = new HashMap<String, String>();
        for (final Map.Entry<String, String[]> it : request.getParameterMap().entrySet())
            root.put(it.getKey(), it.getValue()[0]);
        try {
            ctx.process(root, response.getWriter());
        } catch (final TemplateException e) {
            throw new IOException("Unable to process template " + target + ": " + e);
        }
    }

    @Override
    protected void doGet(final HttpServletRequest request, final HttpServletResponse reply) throws ServletException, IOException {
        String uri = request.getRequestURI();
        if (uri.startsWith("/"))
            uri = uri.substring(1);
        if (uri.isEmpty())
            uri = "index";
        if (new File(root_, uri + ".html").isFile())
            serveTemplate(reply, request, uri + ".html");
        else
            reply.sendError(HttpServletResponse.SC_NOT_FOUND);
    }
}
