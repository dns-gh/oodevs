package main;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

import javax.servlet.ServletConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.log4j.Logger;

public class Handler extends HttpServlet {

    private static final long serialVersionUID = 1L;
    private static final Logger log_ = Logger.getLogger(Handler.class);
    private final Map<String, ProxyHolder> targets_;
    private final Lock read_;
    private final Lock write_;
    private ServletConfig config_;

    public Handler(final Agent.Configuration config) throws Exception {
        targets_ = new HashMap<String, ProxyHolder>();
        final ReadWriteLock lock = new ReentrantReadWriteLock();
        read_ = lock.readLock();
        write_ = lock.writeLock();
        config_ = null;
    }

    @Override
    public void init(final ServletConfig config) {
        config_ = config;
    }

    @Override
    public void service(final ServletRequest req, final ServletResponse res) throws ServletException, IOException {
        String uri = ((HttpServletRequest) req).getRequestURI();
        ProxyHolder proxy = null;
        if (uri.startsWith("/"))
            uri = uri.substring(1);
        if (uri.equals("register_proxy"))
            insertProxy(req, res);
        else if (uri.equals("unregister_proxy"))
            removeProxy(req, res);
        else if (uri.equals("list_proxies"))
            listProxies(req, res);
        else if ((proxy = findProxy(req, uri)) != null)
            proxy.service(req, res);
        else
            ((HttpServletResponse) res).sendError(HttpServletResponse.SC_NOT_FOUND);
    }

    private static final String RequireParameter(final ServletRequest req, final String key) throws ServletException {
        final String value = req.getParameter(key);
        if (value == null)
            throw new ServletException("Missing parameter " + key);
        return value;
    }

    private ProxyHolder Insert(final String key, final ProxyHolder value) {
        write_.lock();
        try {
            return targets_.put(key, value);
        } finally {
            write_.unlock();
        }
    }

    private ProxyHolder Remove(final String key) {
        write_.lock();
        try {
            return targets_.remove(key);
        } finally {
            write_.unlock();
        }
    }

    private ProxyHolder Get(final String key) {
        read_.lock();
        try {
            return targets_.get(key);
        } finally {
            read_.unlock();
        }
    }

    private Set<Entry<String, ProxyHolder>> List() {
        read_.lock();
        try {
            return targets_.entrySet();
        } finally {
            read_.unlock();
        }
    }

    private void insertProxy(final ServletRequest req, final ServletResponse res) throws ServletException, IOException {
        final String prefix = RequireParameter(req, "prefix");
        final String host = RequireParameter(req, "host");
        final int port = Integer.parseInt(RequireParameter(req, "port"));
        final ProxyHolder proxy = new ProxyHolder(prefix, host, port, config_);
        final ProxyHolder previous = Insert(prefix, proxy);
        if (previous != null)
            previous.destroy();
        res.getWriter().print(proxy.toJson());
        log_.info("Added proxy from /" + prefix + " to " + host + ":" + port);
    }

    private ProxyHolder findProxy(final ServletRequest req, final String uri) {
        final int split = uri.indexOf('/');
        return split == -1 ? null : Get(uri.substring(0, split));
    }

    private void removeProxy(final ServletRequest req, final ServletResponse res) throws ServletException, IOException {
        final String prefix = RequireParameter(req, "prefix");
        final ProxyHolder target = Remove(prefix);
        if (target == null) {
            res.getWriter().print("Unable to remove unknown proxy");
            return;
        }
        res.getWriter().print(target.toString());
        target.destroy();
        log_.info("Removed proxy /" + prefix);
    }

    private void listProxies(final ServletRequest req, final ServletResponse res) throws IOException {
        String reply = "";
        for (final Entry<String, ProxyHolder> it : List())
            reply += it.getValue().toJson() + ",";
        reply = "[" + reply.substring(0, Math.max(0, reply.length() - 1)) + "]";
        res.getWriter().print(reply);
    }
}
