package web;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.io.FileUtils;
import org.apache.commons.lang3.StringUtils;
import org.apache.log4j.Logger;
import org.eclipse.jetty.client.ContentExchange;
import org.eclipse.jetty.client.HttpClient;
import org.eclipse.jetty.client.HttpExchange;
import org.eclipse.jetty.server.Request;
import org.eclipse.jetty.server.handler.AbstractHandler;
import org.eclipse.jetty.util.UrlEncoded;

import eu.medsea.mimeutil.MimeType;
import eu.medsea.mimeutil.MimeUtil2;
import freemarker.template.Configuration;
import freemarker.template.DefaultObjectWrapper;
import freemarker.template.Template;
import freemarker.template.TemplateException;

public class Handler extends AbstractHandler {

    private static final Logger log_ = Logger.getLogger(Handler.class);
    @SuppressWarnings("serial")
    private static final Set<String> forwards_ = new HashSet<String>() {
        {
            add("create_session");
        }
    };
    private final Configuration cfg_;
    private final File root_;
    private final MimeUtil2 mimes_;
    private final HttpClient host_;
    private final int port_;

    public Handler(final HttpClient host, final int port, final String root, final boolean isDebug) throws Exception {
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
        host_ = host;
        port_ = port;
    }

    private String getContentType(final File file) {
        @SuppressWarnings("unchecked")
        final Iterator<MimeType> it = mimes_.getMimeTypes(file).iterator();
        return it.hasNext() ? it.next().toString() : "";
    }

    private void serveFile(final HttpServletResponse response, final File file) throws IOException {
        response.setContentType(getContentType(file));
        response.setStatus(HttpServletResponse.SC_OK);
        response.setDateHeader("Date", new Date().getTime());
        response.setDateHeader("Last-Modified", file.lastModified());
        response.setHeader("Etag", String.valueOf(file.lastModified()));
        FileUtils.copyFile(file, response.getOutputStream());
    }

    @SuppressWarnings("unchecked")
    private static Set<Entry<String, String[]>> getParams(final Request request) {
        return request.getParameterMap().entrySet();
    }

    private void serveTemplate(final HttpServletResponse response, final Request request, final String target) throws IOException {
        response.setContentType("text/html");
        response.setStatus(HttpServletResponse.SC_OK);
        final Template ctx = cfg_.getTemplate(target);
        final Map<String, String> root = new HashMap<String, String>();
        for (final Map.Entry<String, String[]> it : getParams(request))
            root.put(it.getKey(), it.getValue()[0]);
        try {
            ctx.process(root, response.getWriter());
        } catch (final TemplateException e) {
            throw new IOException("Unable to process template " + target + ": " + e);
        }
    }

    private void forwardRequest(final HttpServletResponse response, final Request request, final String uri) throws IOException {
        String dst = "http://localhost:" + port_ + "/" + uri;
        final List<String> params = new ArrayList<String>();
        for (final Map.Entry<String, String[]> it : getParams(request))
            params.add(UrlEncoded.encodeString(it.getKey()) + "=" + UrlEncoded.encodeString(it.getValue()[0]));
        if (!params.isEmpty())
            dst += "?" + StringUtils.join(params, "&");

        final ContentExchange exchange = new ContentExchange(true);
        exchange.setURL(dst);
        exchange.setMethod(request.getMethod());
        host_.send(exchange);
        int state = HttpExchange.STATUS_EXCEPTED;
        try {
            state = exchange.waitForDone();
        } catch (final InterruptedException err) {
        }
        if (state != HttpExchange.STATUS_COMPLETED)
            throw new IOException("unable to forward query " + dst + " (" + state + ")");

        response.setContentType("text/plain");
        response.setStatus(exchange.getResponseStatus());
        final String reply = exchange.getResponseContent();
        log_.debug(reply);
        response.getWriter().print(reply);
    }

    @Override
    public void handle(String uri, final Request base, final HttpServletRequest request, final HttpServletResponse response) throws IOException {
        if (uri.equals("/"))
            uri = "/index";
        if (uri.startsWith("/"))
            uri = uri.substring(1);
        File target;
        if (base.getMethod() != "GET")
            response.sendError(HttpServletResponse.SC_BAD_REQUEST, "invalid method " + base.getMethod());
        else if (forwards_.contains(uri))
            forwardRequest(response, base, uri);
        else if ((target = new File(root_, uri)).isFile())
            serveFile(response, target);
        else if ((target = new File(root_, uri + ".html")).isFile())
            serveTemplate(response, base, uri + ".html");
        else
            response.sendError(HttpServletResponse.SC_NOT_FOUND);
        base.setHandled(true);
    }
}
