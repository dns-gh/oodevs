package web;

import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.log4j.Logger;

import eu.medsea.mimeutil.MimeType;
import eu.medsea.mimeutil.MimeUtil2;
import freemarker.template.Configuration;
import freemarker.template.DefaultObjectWrapper;
import freemarker.template.Template;
import freemarker.template.TemplateException;

public class Handler extends HttpServlet {

    private static final long serialVersionUID = 1564095421395727712L;
    private static final Logger log_ = Logger.getLogger(Handler.class);
    private final Configuration cfg_;
    private final String uuid_;
    private final File root_;
    private final MimeUtil2 mimes_;

    public Handler(final String uuid, final String root, final boolean isDebug) throws Exception {
        uuid_ = uuid;
        root_ = new File(root);
        if (!root_.isDirectory())
            throw new IOException(root_ + " is not a directory");
        cfg_ = new Configuration();
        cfg_.setDirectoryForTemplateLoading(root_);
        cfg_.setObjectWrapper(new DefaultObjectWrapper());
        if (isDebug)
            cfg_.setTemplateUpdateDelay(0);
        cfg_.setOutputEncoding("UTF-8");
        mimes_ = new MimeUtil2();
        mimes_.registerMimeDetector("eu.medsea.mimeutil.detector.ExtensionMimeDetector");
    }

    private String getContentType(final File file) {
        @SuppressWarnings("unchecked")
        final Iterator<MimeType> it = mimes_.getMimeTypes(file).iterator();
        return it.hasNext() ? it.next().toString() : "";
    }

    private String identify(String uri, final HttpServletResponse reply) {

        File target = new File(root_, uri);
        if (!target.isFile()) {
            uri += ".html";
            target = new File(root_, uri);
        }
        reply.setContentType(getContentType(target));
        return uri;
    }

    private void serveTemplate(final HttpServletResponse reply, final HttpServletRequest request, final String target) throws IOException {
        reply.setStatus(HttpServletResponse.SC_OK);
        final Template ctx = cfg_.getTemplate(target);
        final Map<String, String> root = new HashMap<String, String>();
        for (final Map.Entry<String, String[]> it : request.getParameterMap().entrySet())
            root.put(it.getKey(), it.getValue()[0]);
        root.put("uuid", uuid_);
        try {
            ctx.process(root, reply.getWriter());
        } catch (final TemplateException e) {
            log_.error(e.toString());
            throw new IOException("Unable to process template " + target);
        }
    }

    @Override
    protected void doGet(final HttpServletRequest request, final HttpServletResponse reply) throws ServletException, IOException {
        String uri = request.getRequestURI();
        if (uri.startsWith("/"))
            uri = uri.substring(1);
        if (uri.isEmpty())
            uri = "index";
        final String target = identify(uri, reply);
        serveTemplate(reply, request, target);
    }
}
