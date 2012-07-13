package web;

import java.io.File;
import java.io.IOException;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import javax.servlet.ServletException;
import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.log4j.Logger;
import org.eclipse.jetty.client.ContentExchange;
import org.eclipse.jetty.client.HttpClient;
import org.eclipse.jetty.client.HttpExchange;
import org.eclipse.jetty.util.thread.QueuedThreadPool;
import org.json.simple.JSONObject;
import org.json.simple.JSONValue;

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
    private final int host_;
    private final boolean debug_;

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
        host_ = config.host;
        debug_ = config.isDebug;
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
        reply.setContentType(getContentType(target) + "; charset=utf-8");
        return uri;
    }

    private void serveTemplate(final HttpServletResponse reply, final HttpServletRequest request, final String target, final JSONObject user)
            throws IOException {
        reply.setStatus(HttpServletResponse.SC_OK);
        final Template ctx = cfg_.getTemplate(target);
        final Map<String, Object> root = new HashMap<String, Object>();
        for (final Map.Entry<String, String[]> it : request.getParameterMap().entrySet())
            root.put(it.getKey(), it.getValue()[0]);
        root.put("uuid", uuid_);
        root.put("name", name_);
        root.put("type", type_);
        root.put("debug", debug_);
        if (user != null) {
            final Map<String, String> sub = new HashMap<String, String>();
            sub.put("id", user.get("id").toString());
            sub.put("username", user.get("username").toString());
            sub.put("name", user.get("name").toString());
            sub.put("type", user.get("type").toString());
            sub.put("temporary", user.get("temporary").toString());
            sub.put("language", user.get("language").toString());
            root.put("user", sub);
            root.put("sid", user.get("sid").toString());
        }
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

    private static String GetSessionId(final HttpServletRequest req) {
        final Cookie[] cookies = req.getCookies();
        if (cookies != null)
            for (final Cookie it : cookies)
                if (it.getName().equals("sid"))
                    return it.getValue();
        final String[] params = req.getParameterValues("sid");
        if (params != null)
            for (final String it : params)
                return it;
        return null;
    }

    private String isAuthenticated(final HttpServletRequest req) throws IOException {
        try {
            final String sid = GetSessionId(req);
            if (sid == null)
                return null;
            final ContentExchange exchange = new ContentExchange(true);
            final String uri = "http://localhost:" + host_ + "/is_authenticated?sid=" + sid;
            exchange.setURL(uri);
            tryAddHeader(exchange, req, "Remote-Address");
            client_.send(exchange);
            final int state = exchange.waitForDone();
            if (state != HttpExchange.STATUS_COMPLETED)
                return null;
            final int code = exchange.getResponseStatus();
            if (code != HttpServletResponse.SC_OK)
                return null;
            return exchange.getResponseContent();
        } catch (final InterruptedException e) {
            // NOTHING
        }
        return null;
    }

    private void serve(String uri, final JSONObject user, final HttpServletRequest req, final HttpServletResponse res) throws ServletException, IOException {
        if (uri.startsWith("/"))
            uri = uri.substring(1);
        if (uri.isEmpty())
            uri = type_ + "/index";
        final String target = identify(uri, res);
        serveTemplate(res, req, target, user);
    }

    @Override
    protected void doGet(final HttpServletRequest req, final HttpServletResponse res) throws ServletException, IOException {
        final String user = isAuthenticated(req);
        final JSONObject juser = user == null ? null : (JSONObject) JSONValue.parse(user);
        if (user == null)
            serve("login", null, req, res);
        else if (Boolean.parseBoolean(juser.get("temporary").toString()))
            serve("update_login", juser, req, res);
        else
            serve(req.getRequestURI(), juser, req, res);
    }
}
