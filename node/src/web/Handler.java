package web;

import java.io.File;
import java.io.IOException;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.log4j.Logger;
import org.eclipse.jetty.client.ContentExchange;
import org.eclipse.jetty.client.HttpClient;
import org.eclipse.jetty.client.HttpExchange;
import org.eclipse.jetty.util.thread.QueuedThreadPool;

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
    private final String type_;
    private final String name_;
    private final HttpClient client_;

    public Handler(final Agent.Configuration config) throws Exception {
        uuid_ = config.uuid;
        root_ = new File(config.www);
        type_ = config.type;
        name_ = config.name;
        if (!root_.isDirectory())
            throw new IOException(root_ + " is not a directory");
        cfg_ = new Configuration();
        cfg_.setDirectoryForTemplateLoading(root_);
        cfg_.setObjectWrapper(new DefaultObjectWrapper());
        if (config.isDebug)
            cfg_.setTemplateUpdateDelay(0);
        cfg_.setOutputEncoding("UTF-8");
        mimes_ = new MimeUtil2();
        mimes_.registerMimeDetector("eu.medsea.mimeutil.detector.ExtensionMimeDetector");
        client_ = new HttpClient();
        client_.setConnectBlocking(false);
        client_.setConnectorType(HttpClient.CONNECTOR_SELECT_CHANNEL);
        client_.setMaxConnectionsPerAddress(200);
        client_.setThreadPool(new QueuedThreadPool(250));
        client_.setTimeout(30 * 1000);
        client_.start();
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
        root.put("name", name_);
        root.put("type", type_);
        try {
            ctx.process(root, reply.getWriter());
        } catch (final TemplateException e) {
            log_.error(e.toString());
            throw new IOException("Unable to process template " + target);
        }
    }

    private static void tryAddHeader(final HttpExchange exchange, final HttpServletRequest req, final String name) {
        final Enumeration<String> tokens = req.getHeaders(name);
        while (tokens.hasMoreElements())
            exchange.addRequestHeader(name, tokens.nextElement());
    }

    private boolean isAuthenticated(final HttpServletRequest req, final int api) throws IOException {
        try {
            final ContentExchange exchange = new ContentExchange(true);
            final String next = "http://localhost:" + api + "/is_authenticated";
            exchange.setURL(next);
            tryAddHeader(exchange, req, "Remote-Address");
            tryAddHeader(exchange, req, "sid");
            client_.send(exchange);
            final int state = exchange.waitForDone();
            if (state != HttpExchange.STATUS_COMPLETED)
                return false;
            final int code = exchange.getResponseStatus();
            return code == HttpServletResponse.SC_OK;
        } catch (final InterruptedException e) {
            // NOTHING
        }
        return false;
    }

    @Override
    protected void doGet(final HttpServletRequest req, final HttpServletResponse res) throws ServletException, IOException {
        String uri = isAuthenticated(req, 40040) ? req.getRequestURI() : "login";
        if (uri.startsWith("/"))
            uri = uri.substring(1);
        if (uri.isEmpty())
            uri = type_ + "/index";
        final String target = identify(uri, res);
        serveTemplate(res, req, target);
    }
}
